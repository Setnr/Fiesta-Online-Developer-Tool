#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
#include <NiApplication.h>
#include <NiD3DShaderFactory.h>

#include <NiPick.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <set>

#include <chrono>

#include "ImGui/ImGuizmo.h"
#include "ImGuiConstantFuncs.h"

#include "FiestaOnlineTool.h"
#include "SHNManager.h"
#include "NiFileLoader.h"

#include <NiStaticDataManager.h>
ImGuizmo::OPERATION OperationMode;

glm::vec4 ConvertQuatToAngleAxis(glm::quat q)
{
	if (q.w == 1.f)
	{
		return { 0.f, 1.f, 0.f, 0.f };
	}

	float_t angle_rad = acos(q.w) * 2;

	float_t x = q.x / sin(angle_rad / 2);
	float_t y = q.y / sin(angle_rad / 2);
	float_t z = q.z / sin(angle_rad / 2);

	return { angle_rad, x, y, z };
}

EditorScene::EditorScene(MapInfo* info) : _InitFile(PgUtil::CreateMapFolderPath(info->KingdomMap, info->MapFolderName, "ini"))
{
	ShowLoadMenu = false;
	auto start = std::chrono::steady_clock::now();
	std::string _FilePath = PgUtil::CreateMapFolderPath(info->KingdomMap, info->MapFolderName,"shmd");
	_Info = info;
	if (!kWorld.InitScene())
		return;
	if (!kWorld.InitCamera())
		return;
	if (!kWorld.InitSkyCtrl())
		return;
	if (!kWorld.InitLightFog())
		return;
	if (!kWorld.InitShadow())
		return;

	std::vector<std::shared_future<void>> _ThreadList;
	{
		auto future = std::async(std::launch::async, [this]
			{
				this->LoadTerrain();
			});
		_ThreadList.push_back(future.share());
	}

#define EditorSceneError(Msg) {NiMessageBox::DisplayMessage(Msg, "Error"); return;}
	std::ifstream SHMD;
	SHMD.open(_FilePath, std::ios::in);

	if (!SHMD.is_open()) 
	{
		EditorSceneError("Failed to open SHMDFile");
	}

	if (!CheckSHMDVersion(SHMD))
		EditorSceneError("SHMD has wrong FileFormat");

	if (!LoadSky(SHMD))
		EditorSceneError("SHMD - Failed to Load Sky");
	if (!LoadWater(SHMD))
		EditorSceneError("SHMD - Failed to Load Water");
	if (!LoadGroundObject(SHMD))
		EditorSceneError("SHMD - Failed to Load GroundObject");
	std::string GlobalLightPath = _FilePath.substr(0, _FilePath.length() - std::string(info->MapFolderName).length()) + "GlobalLight.nif";
	if(!LoadGlobalLight(SHMD, GlobalLightPath))
		EditorSceneError("SHMD - Failed to Load GlobalLight");
	if (!LoadFog(SHMD))
		EditorSceneError("SHMD - Failed to Load Fog");
	if (!LoadBackGroundColor(SHMD))
		EditorSceneError("SHMD - Failed to Load BackGroundColor");
	if (!LoadFrustum(SHMD))
		EditorSceneError("SHMD - Failed to Load Frustum");

	std::vector<std::pair<std::string, std::vector<ObjectPosition>>> ObjectList;
	if(!LoadGlobalObjects(SHMD, ObjectList))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightAmbient(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightDiffuse(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");

	
	for (auto obj : ObjectList) 
	{
		auto future = std::async(std::launch::async, [this, obj] 
			{
				NiNodePtr MainObj;
				NiPickablePtr Obj;
				for(auto pos = obj.second.begin(); pos < obj.second.end(); pos++)
				{
					std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(obj.first);
					if (!MainObj)
					{
						MainObj = PgUtil::LoadNifFile(Path.c_str(), NULL, PICKABLEOBJECTS);
						
					}
					Obj = (NiPickable*)MainObj->Clone();
					
					this->AttachGroundObj(Obj);
					
					Obj->SetName(obj.first.c_str());
					Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);
					Obj->SetTranslate(pos->pos);
					NiMatrix3 m;
					pos->quater.ToRotation(m);
					Obj->SetRotate(m);
					Obj->SetScale(pos->Scale);

					Obj->DisableSorting();
					Obj->SetSelectiveUpdateRigid(true);

					Sleep(2);
				}
			});
		_ThreadList.push_back(future.share());
	}

	{
		using namespace std::chrono_literals;
		while (_ThreadList.size())
		{
			auto status = _ThreadList.at(_ThreadList.size() - 1).wait_for(20ms);
			if (status == std::future_status::ready)
			{
				_ThreadList.pop_back();
				continue;
			}
			Sleep(15);


		}
	}
	BaseNode = kWorld.GetWorldScene();
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera = kWorld.GetCamera();
	NiPick _Pick;
	_Pick.SetTarget(kWorld.GetWorldScene());
	_Pick.SetPickType(NiPick::FIND_ALL);
	_Pick.SetSortType(NiPick::SORT);
	_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
	_Pick.SetFrontOnly(false);
	_Pick.SetReturnNormal(true);
	_Pick.SetObserveAppCullFlag(true);
	NiPoint3 kOrigin(info->RegenX, info->RegenY, 0.f);
	if (_Pick.PickObjects(kOrigin, World::ms_kUpDir,true))
	{
		NiPick::Results& results = _Pick.GetResults();
		for (int i = 0; i < results.GetSize(); i++)
		{
			auto result = results.GetAt(i);
			if(result->GetIntersection().z > kOrigin.z)
				kOrigin.z = result->GetIntersection().z;
		}
		
	}
	kOrigin.z += 100.f;
	Camera->SetTranslate(kOrigin);
	Pitch = 1.57f * 2.0f;
	Yaw = -1.57f;
	Roll = 1.57f;
	NiMatrix3 rotation;
	rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

	Camera->SetRotate(rotation);
	Camera->Update(0.0f);
	CanSwitch = true;
	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully Loaded " <<info->MapName << "(" 
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
	
	return;
}

bool EditorScene::LoadTerrain()
{
	if (!_InitFile.Load())
		return false;
	if (_InitFile.FileType == "")return true;
	std::string HTDFilePath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.HeightFileName);
	std::ifstream HTDFile;
	struct PointInfos
	{
		int BL = -1;
		int BR = -1;
		int TL = -1;
		int TR = -1;
		float Height;
		bool Used = false;
		TerrainLayer::RGBAColor PixelColor;
		NiColorA VertexColor;
	};
	struct Triangle 
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	/*
	This is a two dimensional Array, with the size of the Map
	It contains the Informations which Pixels are used (BL TL BR TR and Used) as 1 Information
	
	The second usage is to be a 2 dimensonal array saveing the Z-Coordinate of the NiPoint3 for the Vertexes

	for e.g 
	a 256 x 256 map has 256x256 Pixels each Pixel containing a TopLeftCorner, BottomLeftCorner ...
	Each Corner is 1 Vertix and has a increasing Number (following TriCt)
	a 256x256 Map has 257x257 Vertixes thats why HeightMap_width is used für the vector sizes

	Based on that i can relate to every Pixel with its x and y Koordinages
	aswell as its relative Vertixes with x,y & x+1,y & x,y+1 & x+1,y+1
	Because the NiTriStripsGeometry has a limited size (unsigned short) the whole Map gets splitted up
	this leads to in the following to Calculateing ActiveW and ActiveH based on the Current Block and 
	the current position within this Block, because of that we can use ActiveW and ActiveH as the PixelCoordinates
	in the 2D-Array

	We check every Pixel if its used and check if any Vertex of it, already has a ID (ID is saved in TL, TR...)
	We only need to Check the LeftPixel and the 3 Connected Pixels below the current Pixel
	if one of those Pixels is used we copy connected TL/BL and so on
	otherwise we create new ID´s

	finaly we add everything in the needed vectors
	convert the vectors in fix Arrays and create the TriStripsData aswell as the Texturing Propertys
	*/
	
	std::vector<std::vector<PointInfos>> VertexMap;
	int PointCounter;

	HTDFile.open(HTDFilePath, std::ios::binary);
	if (!HTDFile.is_open())
	{
		NiMessageBox::DisplayMessage("Failed to open HTD", "Error");
		return false;
	}
	HTDFile.read((char*)&PointCounter, sizeof(PointCounter));
	for (int w = 0; w < _InitFile.HeightMap_width; w++)
		VertexMap.push_back(std::vector<PointInfos>(_InitFile.HeightMap_height));
	
	for (int h = 0; h < _InitFile.HeightMap_height; h++) 
	{
		for (int w = 0; w < _InitFile.HeightMap_width; w++)
			HTDFile.read((char*)&VertexMap[w][h].Height, sizeof(VertexMap[w][h].Height));
	}

	NiImageConverter* conv = NiImageConverter::GetImageConverter();
	std::string VertexShadowPath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.VertexColorTexture);
	NiPixelDataPtr VertexShadowImage = conv->ReadImageFile(VertexShadowPath.c_str(), 0);

	auto VertexColorArray = (TerrainLayer::RGBColor*)VertexShadowImage->GetPixels();
	int PixelCounter = 0;
	for (int h = 0; h < VertexShadowImage->GetHeight(); h++)
	{
		for (int w = 0; w < VertexShadowImage->GetWidth(); w++)
		{
			VertexMap[w][h].VertexColor = NiColorA(static_cast<float>(VertexColorArray[PixelCounter].r) / 255.0f, static_cast<float>(VertexColorArray[PixelCounter].g) / 255.f, static_cast<float>(VertexColorArray[PixelCounter].b) / 255.f, 1.0f);
			PixelCounter++;
		}
	}

	for (auto CurrentLayer : _InitFile.LayerList) 
	{
		auto ColorArray = (TerrainLayer::RGBAColor*)CurrentLayer->pixldata->GetPixels();
		PixelCounter = 0;
		for(int h = 0; h < CurrentLayer->pixldata->GetHeight(); h++)
		{
			for (int w = 0; w < CurrentLayer->pixldata->GetWidth(); w++)
			{
				if(h < CurrentLayer->Height && w < CurrentLayer->Width)
				{
					VertexMap[w][h].PixelColor = ColorArray[PixelCounter];
					PixelCounter++;
				}
			}
		}

		for (int BlockX = 0; BlockX < (_InitFile.HeightMap_width - 1) / _InitFile.QuadsHigh; BlockX++) //19
		{
			for (int BlockY = 0; BlockY < (_InitFile.HeightMap_height - 1) / _InitFile.QuadsWide; BlockY++) //19
			{
				std::vector<NiPoint3> VerticesList;
				std::vector<NiPoint3> NormalList;
				std::vector<NiColorA> ColorList;
				std::vector<NiPoint2> TextureList1;
				std::vector<NiPoint2> TextureList2;
				std::vector<Triangle> TriangleList;
				int TriCt = 0;
				for (int h = 0; h < _InitFile.QuadsHigh; h++) 
				{
					for (int w = 0; w < _InitFile.QuadsWide; w++) 
					{
						int ActiveW = BlockX * _InitFile.QuadsWide + w;
						int ActiveH = BlockY * _InitFile.QuadsHigh + h;
						int PixelW = ActiveW - CurrentLayer->StartPos_X;
						int PixelH = ActiveH - CurrentLayer->StartPos_Y;
						if (!(PixelW >= 0 && PixelW < CurrentLayer->Width))
							continue;
						if (!(PixelH >= 0 && PixelH < CurrentLayer->Height))
							continue;

						PointInfos* PixelInfo = &VertexMap[PixelW][PixelH];
						if (PixelInfo->PixelColor > TerrainLayer::RGBColor(static_cast<char>(CurrentLayer->UVScaleBlend)))
						{
							
							PointInfos* info = &VertexMap[ActiveW][ActiveH];
							info->Used = true;
							bool AddBL = true;
							bool AddBR = true;
							bool AddTL = true;
							bool AddTR = true;

							if (h != 0 || w != 0) 
							{
								if(w != 0)
								{
									PointInfos* PointInfosLeft = &VertexMap[ActiveW - 1][ActiveH];
									if (PointInfosLeft->Used)
									{
										info->BL = PointInfosLeft->BR;
										info->TL = PointInfosLeft->TR;
										AddBL = false;
										AddTL = false;
									}
								}
								if (h != 0) 
								{
									PointInfos* LowerInfos = &VertexMap[ActiveW][ActiveH - 1];
									if (LowerInfos->Used)
									{
										info->BL = LowerInfos->TL;
										info->BR = LowerInfos->TR;
										AddBL = false;
										AddBR = false;
									}
									if (w != 0)
									{
										PointInfos* LowerInfosL = &VertexMap[ActiveW - 1][ActiveH - 1];
										if (LowerInfosL->Used)
										{
											info->BL = LowerInfosL->TR;
											AddBL = false;
										}
									}
									if (ActiveW + 1< _InitFile.HeightMap_width)
									{
										PointInfos* LowerInfosR = &VertexMap[ActiveW + 1][ActiveH - 1];
										if (LowerInfosR->Used)
										{
											info->BR = LowerInfosR->TL;
											AddBR = false;
										}
									}
								}
							}
							float FirstWScale = 1 / CurrentLayer->UVScaleDiffuse; // (static_cast<float>(_InitFile.QuadsWide) / static_cast<float>(_InitFile.HeightMap_width - 1));
							float FirstHScale = 1 / CurrentLayer->UVScaleDiffuse; // (static_cast<float>(_InitFile.QuadsHigh) / static_cast<float>(_InitFile.HeightMap_height - 1));
							float SecondWScale = (1 / static_cast<float>(CurrentLayer->Width - 1));
							float SecondHScale = (1 / static_cast<float>(CurrentLayer->Height - 1));
							
							if (AddBL)
							{
								info->BL = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3(ActiveW * _InitFile.OneBlock_width, ActiveH * _InitFile.OneBlock_height, info->Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
								TextureList1.push_back(NiPoint2(PixelW * FirstWScale, PixelH * FirstHScale));
								TextureList2.push_back(NiPoint2(PixelW * SecondWScale, PixelH * SecondHScale));
							}
							if (AddBR)
							{
								info->BR = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3((ActiveW + 1) * _InitFile.OneBlock_width, ActiveH * _InitFile.OneBlock_height, VertexMap[ActiveW + 1][ActiveH].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
								TextureList1.push_back(NiPoint2((PixelW + 1) * FirstWScale, PixelH * FirstHScale));
								TextureList2.push_back(NiPoint2((PixelW + 1) * SecondWScale, PixelH * SecondHScale));
							}
							if (AddTL)
							{
								info->TL = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3(ActiveW * _InitFile.OneBlock_width, (ActiveH + 1) * _InitFile.OneBlock_height, VertexMap[ActiveW][ActiveH + 1].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
								TextureList1.push_back(NiPoint2(PixelW * FirstWScale, (PixelH + 1) * FirstHScale));
								TextureList2.push_back(NiPoint2(PixelW * SecondWScale, (PixelH + 1) * SecondHScale));
							}
							if (AddTR)
							{
								info->TR = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3((ActiveW + 1) * _InitFile.OneBlock_width, (ActiveH + 1) * _InitFile.OneBlock_height, VertexMap[ActiveW + 1][ActiveH + 1].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
								TextureList1.push_back(NiPoint2((PixelW + 1) * FirstWScale, (PixelH + 1) * FirstHScale));
								TextureList2.push_back(NiPoint2((PixelW + 1)  * SecondWScale, (PixelH + 1) * SecondHScale));
							}
							TriangleList.push_back(Triangle(info->BL, info->BR, info->TL));
							TriangleList.push_back(Triangle(info->TL, info->BR, info->TR));
						}
					}
				}

				if (!VerticesList.size() > 0)
					continue;
				/*Create NiTriStripsData*/
				/*Create RenderMaps*/
				for (int i = 0; i < TextureList2.size(); i++)
					TextureList1.push_back(TextureList2.at(i));

				NiPoint3* pkVertix = NiNew NiPoint3[(int)VerticesList.size()];
				NiPoint3* pkNormal = NiNew NiPoint3[(int)NormalList.size()];
				NiColorA* pkColor = NiNew NiColorA[(int)ColorList.size()];
				NiPoint2* pkTexture = NiNew NiPoint2[(int)TextureList1.size()];
				
				unsigned short* pusTriList = (unsigned short*)NiAlloc(char, 12 * (TriangleList.size() / 2));// NiNew NiPoint3[TriangleList.size() / 2];
				
				memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));
				memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
				memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
				memcpy(pkTexture, &TextureList1[0], (int)TextureList1.size() * sizeof(NiPoint2));
				memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));

				NiTriShapeDataPtr data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 2, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
				NiTriShapePtr Shape = NiNew NiTriShape(data);

				NiSourceTexturePtr BaseTexture = CurrentLayer->BaseTexture;
				NiSourceTexturePtr BlendTexture = CurrentLayer->BlendTexture;
				Shape->SetSortObject(false);

				NiTexturingProperty::ShaderMap* BaseTextureMap = NiNew NiTexturingProperty::ShaderMap(BaseTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
				BaseTextureMap->SetID(0);
				NiTexturingProperty::ShaderMap* BlendTextureMap = NiNew NiTexturingProperty::ShaderMap(BlendTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
				BlendTextureMap->SetID(1);

				NiTexturingPropertyPtr pkTP = NiNew NiTexturingProperty();
				NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();

				//alphaprop->SetDestBlendMode(NiAlphaProperty::ALPHA_ZERO);

				BaseTextureMap->SetTexture(BaseTexture);
				BlendTextureMap->SetTexture(BlendTexture);

				pkTP->SetShaderMap(0, BaseTextureMap);
				pkTP->SetShaderMap(1, BlendTextureMap);
				NiTexturingProperty::Map* NormalMap = NiNew NiTexturingProperty::Map(NULL, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP_MIPNEAREST, 0);

				NormalMap->SetFilterMode(NiTexturingProperty::FILTER_BILERP_MIPNEAREST);
				NormalMap->SetClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
				NormalMap->SetTexture(NULL);

				pkTP->SetBaseClampMode(NiTexturingProperty::WRAP_S_WRAP_T);

				pkTP->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
				pkTP->SetBaseMap(NormalMap);
				Shape->AttachProperty(pkTP);
				Shape->AttachProperty(alphaprop);

				Shape->CalculateNormals();

				Shape->ApplyAndSetActiveMaterial("PgTerrain");
				Shape->Update(0.0);
				Shape->UpdateEffects();
				Shape->UpdateProperties();
				Shape->Update(0.0);

				{
					std::lock_guard<std::mutex> lock(WorldLock);
					kWorld.AttachGroundTerrain(Shape);
				}
			}				

		}
	
		for (int BlockX = 0; BlockX < (_InitFile.HeightMap_width - 1) / _InitFile.QuadsHigh; BlockX++)
		{
			for (int BlockY = 0; BlockY < (_InitFile.HeightMap_height - 1) / _InitFile.QuadsWide; BlockY++)
			{
				for (int h = 0; h < _InitFile.QuadsHigh; h++)
				{
					for (int w = 0; w < _InitFile.QuadsWide; w++)
					{
						int ActiveW = BlockX * _InitFile.QuadsHigh + w;
						int ActiveH = BlockY * _InitFile.QuadsWide + h;
						PointInfos* info = &VertexMap[ActiveW][ActiveH];
						info->BL = -1;
						info->BR = -1;
						info->TL = -1;
						info->TR = -1;
						info->Used = false;
					}
				}
			}
		}
	}
}

void EditorScene::Draw(NiRenderer* renderer)
{
	Camera->SetViewFrustum(kWorld.GetSkyFrustum());

	NiVisibleArray m_kVisible;
	NiCullingProcess m_spCuller(&m_kVisible);
	NiDrawScene(Camera, kWorld.GetSkyNode(), m_spCuller);

	Camera->SetViewFrustum(kWorld.GetWorldFrustum());
	NiVisibleArray m_kVisible2;
	NiCullingProcess m_spCuller2(&m_kVisible2);
	NiDrawScene(Camera, kWorld.GetWorldScene(), m_spCuller2);
}

void EditorScene::DrawImGui() 
{
	StartScene::DrawImGui();

	DrawSHMDEditor();

	DrawGizmo();
}

void EditorScene::CreateMenuBar()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Load Map"))
		{
			ShowLoadMenu = true;
		}
		if (ImGui::MenuItem("Save Map"))
		{
			SaveSHMD();
		}
		ImGui::EndMenu();
	}
}

void EditorScene::DrawGizmo() 
{
	
	if (!SelectedObj)
		return;

	/*
	Huge Credits to Maki for helping me with this Code
	by giving me his :)
	*/


	float_t matrix[16]{};
	float matrixScale[3] = { 1.f, 1.f, 1.f };
	glm::vec3 tmpRotation{};
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::BeginFrame();
	NiPickable* SelectedNode = SelectedObj;

	
	NiPoint3 target = Camera->GetTranslate() + Camera->GetWorldDirection() * 10.f;
	const auto eye = Camera->GetTranslate();
	const auto up = glm::vec3{ 0, 0, 1 };

	const auto view = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(target.x, target.y, target.z), up);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(kWorld.GetFOV()),(((float_t)io.DisplaySize.x / (float_t)io.DisplaySize.y)),Camera->GetViewFrustum().m_fNear,Camera->GetViewFrustum().m_fFar);

	ImGuizmo::RecomposeMatrixFromComponents(&SelectedNode->GetTranslate().x, &tmpRotation[0], matrixScale, (float*)matrix);

	ImGuizmo::SetRect((float_t)0, (float_t)0, (float_t)io.DisplaySize.x, (float_t)io.DisplaySize.y);
	ImGuizmo::Manipulate(&view[0][0], &projectionMatrix[0][0], OperationMode, ImGuizmo::MODE::WORLD, (float*)matrix, nullptr, nullptr);

	ImGuizmo::DecomposeMatrixToComponents((float*)matrix, (float*)&SelectedNode->GetTranslate().x, &tmpRotation[0], matrixScale);

	SelectedObjAngels -= tmpRotation;

	if (abs(SelectedObjAngels[0]) > 180.f)
	{
		SelectedObjAngels[0] = -SelectedObjAngels[0] + 2 * fmod(SelectedObjAngels[0], 180.f);
	}

	if (abs(SelectedObjAngels[1]) > 180.f)
	{
		SelectedObjAngels[1] = -SelectedObjAngels[1] + 2 * fmod(SelectedObjAngels[1], 180.f);
	}

	if (abs(SelectedObjAngels[2]) > 180.f)
	{
		SelectedObjAngels[2] = -SelectedObjAngels[2] + 2 * fmod(SelectedObjAngels[2], 180.f);
	}

	glm::vec4 angleAxis = ConvertQuatToAngleAxis(glm::quat(glm::radians(SelectedObjAngels)));
	NiMatrix3 m;
	m.MakeRotation(angleAxis[0], angleAxis[1], angleAxis[2], angleAxis[3]);
	
	float angle, x, y, z;
	SelectedNode->GetRotate().ExtractAngleAndAxis(angle, x, y, z);

	SelectedNode->SetRotate(m);
}

void EditorScene::DrawSHMDEditor()
{
	auto io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 295;
	int w = 578;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));
	

	ImGui::Begin("SHMD-Editor", NULL, flags); 
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Move with WASD");
		ImGui::TextUnformatted("Press Shift to Move faster");
		ImGui::TextUnformatted("Right Click to Rotate Cam");
		ImGui::TextUnformatted("Left Click to Select a World Object");
		ImGui::TextUnformatted("Middle Mouse to open Menu");
		ImGui::TextUnformatted("Copyright Gamebryo / Gamgio / IDK");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	DrawSHMDHeader("Sky", kWorld.GetSkyNode());
	DrawSHMDHeader("Water", kWorld.GetWaterNode());
	DrawSHMDHeader("GroundObject", kWorld.GetGroundObjNode());
	ImGui::ColorEdit3("Global Light", (float*) &kWorld.GetAmbientLightAmbientColor().r);
	ImGui::ColorEdit3("Fog Color", (float*)&kWorld.GetFogColor().r);
	
	float depth = kWorld.GetFogDepth();
	if (ImGui::DragFloat("Fog Alpha", &depth, 0.01, 0.f, 1.0f))
		kWorld.SetFogDepth(depth);
	ImGui::ColorEdit3("Background Color", (float*)&kWorld.GetBackgroundColor().r);
	ImGui::DragFloat("Frustum", &kWorld.GetWorldFrustum().m_fFar, 10.0f,0.0f);

	ImGui::ColorEdit3("AmbientLight Color", (float*)&kWorld.GetMapDirectionalLightAmbientColor().r);
	ImGui::ColorEdit3("DiffuseLight Color", (float*)&kWorld.GetMapDirectionalLightDiffuseColor().r);
	DrawSHMDHeader("Pickable Objects", kWorld.GetGroundCollidee());

	ImGui::End();

	if (SelectedObj)
	{
		{
			auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

			int h2 = 123 + h;
			int w2= 319;

			ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w2, io.DisplaySize.y - h2));
			ImGui::SetNextWindowSize(ImVec2(w2, h2 - h));

			ImGui::Begin("Object-Editor", NULL, flags);
			static int e = 0;
			ImGui::RadioButton("Translate", &e, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Rotate", &e, 1);
			switch (e)
			{
			case 0:
				OperationMode = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case 1:
			default:
				OperationMode = ImGuizmo::OPERATION::ROTATE;
				break;
			}

			ImGui::DragFloat3("Position", (float*)& SelectedObj->GetTranslate().x);
			ImGui::DragFloat3("Rotation", &SelectedObjAngels[0]);
			float Scale = SelectedObj->GetScale();
			if (ImGui::DragFloat("Scale", &Scale,0.01f,0.0f,5.0f))
				SelectedObj->SetScale(Scale);
			ImGui::End();
		}
	}
	static NiFileLoader loader;
	if (loader.DrawImGui()) 
	{
		NiNodePtr obj = loader.Load();
		if (NiIsKindOf(NiPickable, obj))
			SelectedObj = (NiPickable*)(NiNode*)obj;
		
	}

	if (ImGui::BeginPopupContextVoid(0, ImGuiPopupFlags_MouseButtonMiddle))
	{
		if (ImGui::Selectable("Add Sky"))
			loader.Prepare(kWorld.GetSkyNode());
		if (ImGui::Selectable("Add Water"))
			loader.Prepare(kWorld.GetWaterNode());
		if (ImGui::Selectable("Add Object"))
			loader.Prepare(kWorld.GetGroundObjNode());
		if (ImGui::Selectable("Add Pickable"))
		{
			NiPoint3 kOrigin, kDir;
			long X, Y;
			FiestaOnlineTool::GetMousePosition(X, Y);
			if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
			{
				NiPick _Pick;
				_Pick.SetPickType(NiPick::FIND_FIRST);
				_Pick.SetSortType(NiPick::SORT);
				_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
				_Pick.SetFrontOnly(true);
				_Pick.SetReturnNormal(true);
				_Pick.SetObserveAppCullFlag(true);
				_Pick.SetTarget(kWorld.GetTerrainScene());
				if (_Pick.PickObjects(kOrigin, kDir, true))
				{
					NiPick::Results& results = _Pick.GetResults();
					for (int i = 0; i < results.GetSize(); i++)
					{
						auto result = results.GetAt(i);
						loader.Prepare(kWorld.GetGroundCollidee(), PICKABLEOBJECTS, result->GetIntersection());
					}

				}
				else 
				{
					_Pick.SetTarget(kWorld.GetWorldScene());
					if (_Pick.PickObjects(kOrigin, kDir, true))
					{
						NiPick::Results& results = _Pick.GetResults();
						for (int i = 0; i < results.GetSize(); i++)
						{
							auto result = results.GetAt(i);
							loader.Prepare(kWorld.GetGroundCollidee(), PICKABLEOBJECTS, result->GetIntersection());
						}

					}
				}
			}
		}
		if(SelectedObj)
			if (ImGui::Selectable("Delete Selected Obj"))
			{
				SelectedObj->GetParent()->DetachChild(SelectedObj);
				SelectedObj = NULL;
			}
		ImGui::EndPopup();
	}
}

void EditorScene::DrawSHMDHeader(std::string Name, NiNodePtr Node)
{
	if (ImGui::CollapsingHeader(Name.c_str()))
	{
		bool compact = false;
		int ChildCount = Node->GetChildCount();
		for (int i = 0; i < ChildCount; i++)
		{
			auto Object = Node->GetAt(i);
			if (NiIsKindOf(NiNode, Object))
			{
				ImGui::Text(Object->GetName(), "");
				ImGui::SameLine();
				if (ImGui::Button(std::string("Delete##" + Name + std::to_string(i)).c_str()))
				{
					Node->DetachChildAt(i);
					compact = true;
				}
			}
		}
		if (compact)
			Node->CompactChildArray();
	}
}

void EditorScene::UpdateCamera(float fTime) 
{
	FiestaScene::UpdateCamera(fTime);
	NiPoint3 translate(Camera->GetTranslate());
	kWorld.GetSkyNode()->SetTranslate(translate);

	auto& io = ImGui::GetIO();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		NiPoint3 kOrigin, kDir;
		long X, Y;
		FiestaOnlineTool::GetMousePosition(X, Y);
		if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir)) 
		{
			NiPick _Pick;
			_Pick.SetPickType(NiPick::FIND_FIRST);
			_Pick.SetSortType(NiPick::SORT);
			_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
			_Pick.SetFrontOnly(true);
			_Pick.SetReturnNormal(true);
			_Pick.SetObserveAppCullFlag(true);
			_Pick.SetTarget(kWorld.GetWorldScene());
			if (_Pick.PickObjects(kOrigin, kDir, true)) 
			{
				NiPick::Results& results = _Pick.GetResults();

				std::set<NiPickable*> PossibleObjects;

				for (int i = 0; i < results.GetSize(); i++ ) 
				{
					auto record = results.GetAt(i);
					auto obj = record->GetAVObject();
					if (NiIsKindOf(NiPickable, obj))
					{
						PossibleObjects.insert((NiPickable*)obj);
					}
					else 
					{ 
						//We only earch Upwards, 
						//if we search Downwards we can click the terrain and select the first 
						//NiPickable which must not be the actually targeted Node
						//Picking only works when compiled with the define of PICKABLEOBJECTS being true
						while (obj = obj->GetParent())
						{
							if (NiIsKindOf(NiPickable, obj))
							{
								PossibleObjects.insert((NiPickable*)obj);
								break;
							}
						}
					}
				}
				if (auto search = PossibleObjects.find(SelectedObj); search == PossibleObjects.end()) 
				{
					if(PossibleObjects.size() != 0)
					{
						SelectedObj = *PossibleObjects.begin();
						float angle, x, y, z;
						SelectedObj->GetRotate().ExtractAngleAndAxis(angle, x, y, z);
						SelectedObjAngels = glm::degrees(glm::vec3{ eulerAngles(glm::angleAxis((float)angle, glm::vec3(x, y, z))) });

					}
				}
			}
		}
	}
}

void EditorScene::SaveSHMD() 
{
	auto start = std::chrono::steady_clock::now();
	std::string _FilePath = PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "sdt.shmd");
	std::ofstream file;
	file.open(_FilePath);
	file << std::fixed << std::setprecision(6);
	file << "shmd0_5" << std::endl;
	SaveSHMDEntry(file, kWorld.GetSkyNode(), "Sky");
	SaveSHMDEntry(file, kWorld.GetWaterNode(), "Water");
	SaveSHMDEntry(file, kWorld.GetGroundObjNode(), "GroundObject");
	SaveSHMDLight(file, kWorld.GetAmbientLightAmbientColor(), "GlobalLight");
	SaveSHMDFog(file, kWorld.GetFogDepth(), kWorld.GetFogColor());
	SaveSHMDLight(file, kWorld.GetBackgroundColor(), "BackGroundColor");
	SaveSHMDFrustum(file, kWorld.GetWorldFrustum());
	SaveSHMDGroundObjects(file, kWorld.GetGroundCollidee());
	SaveSHMDLight(file, kWorld.GetMapDirectionalLightAmbientColor(), "DirectionLightAmbient");
	SaveSHMDLight(file, kWorld.GetMapDirectionalLightDiffuseColor(), "DirectionLightDiffuse");
	file.close();
	auto diff = std::chrono::steady_clock::now() - start;
	std::ostringstream oss;
	oss << "Successfully safed " << _Info->MapName << "("
		<< std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
	LogInfo(oss.str());
}
void EditorScene::SaveSHMDEntry(std::ofstream& file, NiNodePtr objNode, const char* Name)
{
	objNode->CompactChildArray();
	file << Name << " " << std::to_string(objNode->GetChildCount()) << std::endl;
	for (int i = 0; i < objNode->GetChildCount(); i++)
	{
		file << objNode->GetAt(i)->GetName() << std::endl;
	}
}
void EditorScene::SaveSHMDLight(std::ofstream& file , NiColor color, const char* name) 
{
	file << name << " " << color.r << " " << color.g << " " << color.b << std::endl;
}
void EditorScene::SaveSHMDFog(std::ofstream&file , float depth, NiColor color) 
{
	file << "Fog " << depth << " " << color.r << " " << color.g << " " << color.b << std::endl;
}
void EditorScene::SaveSHMDFrustum(std::ofstream& file, NiFrustum frustum) 
{
	file << "Frustum " << frustum.m_fFar << std::endl;
}
void EditorScene::SaveSHMDGroundObjects(std::ofstream& file, NiNodePtr node) 
{
	node->CompactChildArray();
	std::map<std::string, std::vector<NiNodePtr>> ObjectMap;
	for (int i = 0; i < node->GetChildCount(); i++) 
	{
		auto child = node->GetAt(i);
		if (!NiIsKindOf(NiNode, child))
			continue;
		auto& Name = child->GetName();
		if (Name.Contains("resmap")) 
		{
			auto it = ObjectMap.find(std::string(Name));
			if (it == ObjectMap.end()) 
			{
				std::vector<NiNodePtr> vec;
				vec.push_back((NiNode*)child);
				ObjectMap.insert({ std::string(Name), vec });
			}
			else 
			{
				it->second.push_back((NiNode*)child);
			}
		}
	}
	for (auto Info : ObjectMap) 
	{
		file << Info.first << " " << std::to_string(Info.second.size()) << std::endl;
		for (auto obj : Info.second) 
		{
			NiPoint3 trans = obj->GetTranslate();
			NiMatrix3 matrix = obj->GetRotate();
			NiQuaternion quater;
			quater.FromRotation(matrix);
			float scale = obj->GetScale();

			file << trans.x << " " << trans.y << " " << trans.z << " " << quater.m_fX << " " << quater.m_fY << " " << quater.m_fZ << " " << quater.m_fW << " "<<scale << std::endl;
		}
	}
	file << "DataObjectLoadingEnd" << std::endl;
}