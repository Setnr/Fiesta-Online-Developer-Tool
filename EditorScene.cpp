#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
#include <NiApplication.h>
#include <NiD3DShaderFactory.h>
#include <iostream>
#include <fstream>
#include <thread>

void LoadTerrainThread(EditorScene* Scene)
{
	if (!Scene->LoadTerrain()) {
		NiMessageBox::DisplayMessage("Failed to Load Terrain for Current Scene", "Error");
	}
}

EditorScene::EditorScene(std::string FilePath, std::string FileName) : _InitFile(FilePath.substr(0, FilePath.length() - 5) + ".ini")
{

	_FilePath = FilePath;
	_FileName = FileName;

	std::thread _Thread(LoadTerrainThread,this);



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

#define EditorSceneError(Msg) {NiMessageBox::DisplayMessage(Msg, "Error"); return;}
	std::ifstream SHMD;
	SHMD.open(FilePath, std::ios::in);

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
	if(!LoadGlobalLight(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalLight");
	if (!LoadFog(SHMD))
		EditorSceneError("SHMD - Failed to Load Fog");
	if (!LoadBackGroundColor(SHMD))
		EditorSceneError("SHMD - Failed to Load BackGroundColor");
	if (!LoadFrustum(SHMD))
		EditorSceneError("SHMD - Failed to Load Frustum");
	if(!LoadGlobalObjects(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightAmbient(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightDiffuse(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");

	Camera = kWorld.GetCamera();
	Camera->SetTranslate(NiPoint3(5576, 5768, 2500));
	Pitch = 1.57f * 2.0f;
	Yaw = -1.57f;
	Roll = 1.57f;
	NiMatrix3 rotation;
	rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

	Camera->SetRotate(rotation);

	_Thread.join();

	if(TerrainParent)
	{
		terrain->SetName("terrain");
		kWorld.AttachGroundTerrain(TerrainParent);
	}

	//NiNodePtr NiN;
	//NiString CleanPath = _FilePath.GetSubstring(0, ".");
	//NiString IniFile = CleanPath;
	//IniFile.Insert(".sdt", CleanPath.Length());
	//PgUtil::LoadTerrainNif(IniFile,&NiN, 0);
	//FixSupTexturing(NiN);
	//kWorld.AttachGroundTerrain(NiN);

	BaseNode->AttachChild(kWorld.GetWorldScene());
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera->Update(0.0f);


	NiStream kStream2;
	kStream2.InsertObject(kWorld.GetTerrainScene());
	kStream2.InsertObject(kWorld.GetSkyNode());
	kStream2.Save("./TESTING123.nif");	

	//NiStream kStream;
	//kStream.InsertObject(NiN);
	//kStream.Save("./STDNIF.nif");
	return;
}

bool EditorScene::LoadTerrain()
{
	terrain = NULL;
	TerrainParent = NULL;

	if (!_InitFile.Load())
		return false;

	std::string HTDFilePath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.HeightFileName);;
	std::ifstream HTDFile;
	struct PointInfos
	{
		int BL = -1;
		int BR = -1;
		int TL = -1;
		int TR = -1;
		float Height;
		bool Used = false;
	};
	struct Triangle 
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
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

	for (auto CurrentLayer : _InitFile.LayerList) 
	{
		for (int BlockX = 0; BlockX < (_InitFile.HeightMap_width - 1) / _InitFile.QuadsHigh; BlockX++)
		{
			for (int BlockY = 0; BlockY < (_InitFile.HeightMap_height - 1) / _InitFile.QuadsWide; BlockY++)
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
						if (w == 64)
							BlockY = BlockY + 1 - 1;
						auto ColorArray = (TerrainLayer::RGBAColor*)CurrentLayer->pixldata->GetPixels();
					
						int PreBlockXPart = BlockX * _InitFile.QuadsWide;
						int CurrentBlockXPart = w;
						int XPart = PreBlockXPart + CurrentBlockXPart;

						int PreBlockYPart = BlockY * _InitFile.QuadsHigh * CurrentLayer->pixldata->GetWidth();
						int CurrentBlockYPart = CurrentLayer->pixldata->GetWidth() * h;
						int YPart = PreBlockYPart + CurrentBlockYPart;

						int PointOffset = XPart + YPart;
						if (ColorArray[PointOffset] > TerrainLayer::RGBColor(static_cast<char>(CurrentLayer->UVScaleDiffuse)))
						{
							int ActiveW = BlockX * _InitFile.QuadsWide + w;
							int ActiveH = BlockY * _InitFile.QuadsHigh + h;
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
							float FirstWScale = (static_cast<float>(_InitFile.QuadsWide) / static_cast<float>(_InitFile.HeightMap_width - 1));
							float FirstHScale = (static_cast<float>(_InitFile.QuadsHigh) / static_cast<float>(_InitFile.HeightMap_height - 1));
							float SecondWScale = (1 / static_cast<float>(_InitFile.HeightMap_width - 1));
							float SecondHScale = (1 / static_cast<float>(_InitFile.HeightMap_width - 1));
							if (AddBL)
							{
								info->BL = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3(ActiveW * _InitFile.OneBlock_width, ActiveH * _InitFile.OneBlock_height, info->Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(NiColorA(1.00f, 1.00f, 1.00f, 1.0f));
								TextureList1.push_back(NiPoint2(ActiveW * FirstWScale, ActiveH * FirstHScale));
								TextureList2.push_back(NiPoint2(ActiveW * SecondWScale, ActiveH * SecondHScale));
							}
							if (AddBR)
							{
								info->BR = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3((ActiveW + 1) * _InitFile.OneBlock_width, ActiveH * _InitFile.OneBlock_height, VertexMap[ActiveW + 1][ActiveH].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(NiColorA(1.00f, 1.00f, 1.00f, 1.0f));
								TextureList1.push_back(NiPoint2((ActiveW + 1) * FirstWScale, ActiveH * FirstHScale));
								TextureList2.push_back(NiPoint2((ActiveW + 1) * SecondWScale, ActiveH * SecondHScale));
							}
							if (AddTL)
							{
								info->TL = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3(ActiveW * _InitFile.OneBlock_width, (ActiveH + 1) * _InitFile.OneBlock_height, VertexMap[ActiveW][ActiveH + 1].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(NiColorA(1.00f, 1.00f, 1.00f, 1.0f));
								TextureList1.push_back(NiPoint2(ActiveW * FirstWScale, (ActiveH + 1) * FirstHScale));
								TextureList2.push_back(NiPoint2(ActiveW * SecondWScale, (ActiveH + 1) * SecondHScale));
							}
							if (AddTR)
							{
								info->TR = TriCt;
								TriCt++;
								VerticesList.push_back(NiPoint3((ActiveW + 1) * _InitFile.OneBlock_width, (ActiveH + 1) * _InitFile.OneBlock_height, VertexMap[ActiveW + 1][ActiveH + 1].Height));
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(NiColorA(1.00f, 1.00f, 1.00f, 1.0f));
								TextureList1.push_back(NiPoint2((ActiveW + 1) * FirstWScale, (ActiveH + 1) * FirstHScale));
								TextureList2.push_back(NiPoint2((ActiveW + 1)  * SecondWScale, (ActiveH + 1) * SecondHScale));
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
				unsigned short* pusTriList = (unsigned short*)NiNew NiPoint3[TriangleList.size() / 2];

				memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));
				memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
				memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
				memcpy(pkTexture, &TextureList1[0], (int)TextureList1.size() * sizeof(NiPoint2));
				memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));

				NiTriShapeData* data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 2, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
				NiTriShape* Shape = NiNew NiTriShape(data);

				NiSourceTexturePtr BaseTexture = CurrentLayer->BaseTexture;
				NiSourceTexturePtr BlendTexture = CurrentLayer->BlendTexture;


				NiTexturingProperty::ShaderMap* BaseTextureMap = NiNew NiTexturingProperty::ShaderMap(BaseTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
				BaseTextureMap->SetID(0);
				NiTexturingProperty::ShaderMap* BlendTextureMap = NiNew NiTexturingProperty::ShaderMap(BlendTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
				BlendTextureMap->SetID(1);

				NiTexturingProperty* pkTP = NiNew NiTexturingProperty();
				NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();

				alphaprop->SetDestBlendMode(NiAlphaProperty::ALPHA_ZERO);

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
				if (!terrain)
				{
					terrain = NiNew NiNode;
					TerrainParent = NiNew NiSortAdjustNode;
					NiAlphaProperty* alphapro = NiNew NiAlphaProperty();
					TerrainParent->AttachChild(terrain);
					TerrainParent->AttachProperty(alphapro);
				}
				terrain->AttachChild(Shape);
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