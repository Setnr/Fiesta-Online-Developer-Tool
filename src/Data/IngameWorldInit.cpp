#include "IngameWorld.h"
#include "../Logger/Logger.h"
#include <filesystem>
#include <thread>
#include <future>
#include <NiPick.h>

#include "SHMD/SHMD.h"

NiPoint3 World::ms_kUpDir = NiPoint3(0.0, 0.0, 1.0);
NiPoint3 World::ms_kDownDir = NiPoint3(0.0, 0.0, -1.0);
NiPoint3 World::ms_kNorthDir = NiPoint3(0.0, 1.0, 0.0);
NiPoint3 World::ms_kEastDir = NiPoint3(1.0, 0.0, 0.0);
NiPoint3 World::ms_kWestDir = NiPoint3(-1.0, 0.0, 0.0);
NiPoint3 World::ms_kDefaultDirectionalLightDir = NiPoint3(0.5, 0.69999999, -0.40000001);

bool World::LoadTerrain() 
{
	if (!_InitFile.Load())
		return false;
	if (_InitFile.FileType == "")return true;
	for (int i = 0; i < m_spGroundTerrain->GetChildCount(); i++)
		m_spGroundTerrain->DetachChildAt(i);
	m_spGroundTerrain->CompactChildArray();
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
	{
		VertexMap.push_back(std::vector<PointInfos>(_InitFile.HeightMap_height));
		_HTD.push_back(std::vector<HTDHelper>(_InitFile.HeightMap_height));
	}

	for (int h = 0; h < _InitFile.HeightMap_height; h++)
	{
		for (int w = 0; w < _InitFile.HeightMap_width; w++)
		{
			HTDFile.read((char*)&VertexMap[w][h].Height, sizeof(VertexMap[w][h].Height));
			_HTD[w][h].Height = VertexMap[w][h].Height;
			_HTD[w][h].Vec = std::vector<NiPoint3*>();
			_HTD[w][h].Shape = std::vector<NiGeometryData*>();
		}
	}
	HTDFile.close();
	std::string HTDGFilePath = HTDFilePath + "g";
	if (std::filesystem::exists(HTDGFilePath))
	{
		std::ifstream HTDGFile;
		HTDGFile.open(HTDGFilePath, std::ios::binary);
		if (!HTDGFile.is_open())
		{
			NiMessageBox::DisplayMessage("Failed to open HTDG", "Error");
			return false;
		}
		int HTDGPointCounter;
		HTDGFile.read((char*)&HTDGPointCounter, sizeof(HTDGPointCounter));
		if (_InitFile.HeightMap_height * _InitFile.HeightMap_width != HTDGPointCounter)
		{
			LogError("HTDG Size MissMatch");
		}
		else
		{
			float Value = 0.0f;
			for (int h = 0; h < _InitFile.HeightMap_height; h++)
			{
				for (int w = 0; w < _InitFile.HeightMap_width; w++)
				{
					HTDGFile.read((char*)&Value, sizeof(Value));
					VertexMap[w][h].Height += Value;
				}
			}
		}
		HTDGFile.close();
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
			VertexMap[w][VertexShadowImage->GetHeight() - h - 1].VertexColor = NiColorA(static_cast<float>(VertexColorArray[PixelCounter].r) / 255.0f, static_cast<float>(VertexColorArray[PixelCounter].g) / 255.f, static_cast<float>(VertexColorArray[PixelCounter].b) / 255.f, 1.0f);
			PixelCounter++;
		}
	}
	int Layer = 0;
	for (auto CurrentLayer : _InitFile.LayerList)
	{
		auto ColorArray = (TerrainLayer::RGBAColor*)CurrentLayer->pixldata->GetPixels();
		PixelCounter = 0;
		for (int h = 0; h < CurrentLayer->pixldata->GetHeight(); h++)
		{
			for (int w = 0; w < CurrentLayer->pixldata->GetWidth(); w++)
			{
				if (h < CurrentLayer->Height && w < CurrentLayer->Width)
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
				std::vector<std::pair<int, int>> WHList;
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
								if (w != 0)
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
									if (ActiveW + 1 < _InitFile.HeightMap_width)
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
								WHList.push_back({ ActiveW,ActiveH });
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
								WHList.push_back({ ActiveW + 1,ActiveH });
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
								WHList.push_back({ ActiveW,ActiveH + 1 });
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
								WHList.push_back({ ActiveW + 1,ActiveH + 1});
								NormalList.push_back(World::ms_kUpDir);
								ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
								TextureList1.push_back(NiPoint2((PixelW + 1) * FirstWScale, (PixelH + 1) * FirstHScale));
								TextureList2.push_back(NiPoint2((PixelW + 1) * SecondWScale, (PixelH + 1) * SecondHScale));
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

				Shape->SetConsistency(NiGeometryData::Consistency::MUTABLE);

				for (int i = 0; i < WHList.size(); i++)
				{
					_HTD[WHList[i].first][WHList[i].second].Vec.push_back(&pkVertix[i]);
					_HTD[WHList[i].first][WHList[i].second].Shape.push_back(Shape->GetModelData());
				}

				AttachGroundTerrain(Shape);
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
		Layer++;
	}
}

#define EditorSceneError(Msg) {LogError(Msg); return false;}

bool World::LoadSHMD() 
{
	std::ifstream SHMD;
	std::string _FilePath = PgUtil::CreateMapFolderPath(_Info->KingdomMap, _Info->MapFolderName, "shmd");
	SHMD.open(_FilePath, std::ios::in);

	if (!SHMD.is_open())
	{
		EditorSceneError("Failed to open SHMDFile");
	}

	if (!CheckSHMDVersion(SHMD))
		EditorSceneError("SHMD has wrong FileFormat");

	if (!LoadSky(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Sky");
	if (!LoadWater(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Water");
	if (!LoadGroundObject(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GroundObject");
	std::string GlobalLightPath = _FilePath.substr(0, _FilePath.length() - std::string(_Info->MapFolderName).length()) + "GlobalLight.nif";
	if (!LoadGlobalLight(SHMD, GlobalLightPath, this))
		EditorSceneError("SHMD - Failed to Load GlobalLight");
	if (!LoadFog(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Fog");
	if (!LoadBackGroundColor(SHMD, this))
		EditorSceneError("SHMD - Failed to Load BackGroundColor");
	if (!LoadFrustum(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Frustum");

	std::vector<std::pair<std::string, std::vector<ObjectPosition>>> ObjectList;
	if (!LoadGlobalObjects(SHMD, ObjectList))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightAmbient(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightDiffuse(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");


	std::vector<std::shared_future<void>> _ThreadList;
	for (auto obj : ObjectList)
	{
		auto future = std::async(std::launch::async, [this, obj]
			{
				NiNodePtr MainObj;
				NiPickablePtr Obj;
				for (auto pos = obj.second.begin(); pos < obj.second.end(); pos++)
				{
					std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(obj.first);
					if (!MainObj)
					{
						MainObj = PgUtil::LoadNifFile(Path.c_str(), NULL, PICKABLEOBJECTS);

					}
					Obj = (NiPickable*)MainObj->Clone();
					NiNodePtr ptr = &*Obj;
					this->AttachGroundObj(ptr);

					Obj->SetSHMDPath(obj.first);
					Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);
					Obj->SetTranslate(pos->pos);
					NiMatrix3 m;
					pos->quater.ToRotation(m);
					Obj->SetRotate(m);
					Obj->SetScale(pos->Scale);

					Obj->DisableSorting();
					Obj->SetSelectiveUpdateRigid(true);

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
			Sleep(1);
		}
	}
}
bool World::LoadSHBD() 
{
	float BlockWidth = _InitFile.OneBlock_width;
	float BlockHeight = _InitFile.OneBlock_height;

	SHBDNode = NiNew NiNode;
	std::vector<char> SHBDData = _SHBD.GetDataRefrence();
	PixelSize = _SHBD.GetMapSize() * BlockWidth / _SHBD.GetSHBDSize();

	const NiPixelFormat* SHBDTexturePixelFormat = &NiPixelFormat::RGBA32;
	Blocked = (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Blocked |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Blocked |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	Walkable = (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Walkable |= (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	Walkable |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Walkable |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	BrushColor = (0xFF << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	BrushColor |= (0xA2 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	BrushColor |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	BrushColor |= (0xFF << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	std::vector<NiPoint3> NormalList = { World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir };
	std::vector<NiColorA> ColorList = { NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f),NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f) };
	std::vector<NiPoint2> TextureList1 = { NiPoint2(0.f,0.f),NiPoint2(1.f,0.f),NiPoint2(0.f,1.f),NiPoint2(1.f,1.f) };
	std::vector<NiPoint2> TextureList2 = { NiPoint2(0.f,0.f),NiPoint2(0.5f,0.f),NiPoint2(0.f,0.5f),NiPoint2(0.5f,0.5f) }; //2048 
	for (int i = 0; i < TextureList2.size(); i++)
		TextureList1.push_back(TextureList2.at(i));
	struct Triangle
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	std::vector<Triangle> TriangleList;

	TriangleList.push_back(Triangle(0, 1, 2));
	TriangleList.push_back(Triangle(2, 1, 3));

	NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
	alphaprop->SetAlphaBlending(true);
	alphaprop->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);


	int TextureCt = _SHBD.GetSHBDSize() / TextureSize;
	if (_SHBD.GetSHBDSize() % TextureSize != 0)
		TextureCt++;

	std::vector<std::shared_future<std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>>>> _ThreadList;
	for (int TextureH = 0; TextureH < TextureCt; TextureH++)
	{
		auto future = std::async(std::launch::async, 
			[this, TextureCt, TextureH, NormalList,ColorList,TextureList1, TriangleList,alphaprop,SHBDData]
			{
				NiTexture::FormatPrefs kPrefs;
				kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
				kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
				kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

				std::vector<NiPixelDataPtr> TextureList;
				std::vector<NiTriShapePtr> Shapes;
				for (int TextureW = 0; TextureW < TextureCt; TextureW++)
				{
					NiPoint3 BL(TextureW * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
					NiPoint3 BR((TextureW + 1) * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
					NiPoint3 TL(TextureW * TextureSize * PixelSize, (TextureH + 1) * TextureSize * PixelSize, 1.f);
					NiPoint3 TR((TextureW + 1) * TextureSize * PixelSize, (TextureH + 1) * TextureSize * PixelSize, 1.f);

					std::vector<NiPoint3> VerticesList = { BL,BR,TL,TR };

					NiPoint3* pkVertix = NiNew NiPoint3[4];
					memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));

					NiPoint3* pkNormal = NiNew NiPoint3[(int)NormalList.size()];
					NiColorA* pkColor = NiNew NiColorA[(int)ColorList.size()];
					NiPoint2* pkTexture = NiNew NiPoint2[(int)TextureList1.size()];

					unsigned short* pusTriList = (unsigned short*)NiAlloc(char, 12 * (TriangleList.size() / 2));// NiNew NiPoint3[TriangleList.size() / 2];

					memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
					memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
					memcpy(pkTexture, &TextureList1[0], (int)TextureList1.size() * sizeof(NiPoint2));
					memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));


					NiTriShapeDataPtr data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 2, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
					NiTriShapePtr BlockShape = NiNew NiTriShape(data);
					BlockShape->AttachProperty(alphaprop);
					BlockShape->CalculateNormals();

					NiPixelData* pixel = NiNew NiPixelData(TextureSize, TextureSize, NiPixelFormat::RGBA32);

					auto pixeloffset = (unsigned int*)pixel->GetPixels();
					TextureList.push_back(pixel);
					for (int h = 0; h < TextureSize; h++)
					{
						for (int w = 0; w < TextureSize; w++)
						{
							unsigned int* NewPtr = (pixeloffset + (h * pixel->GetWidth()) + w);
							int hges = h + TextureH * TextureSize;
							int wges = w + TextureW * TextureSize;

							int index = hges * _SHBD.GetSHBDSize() + wges;

							size_t charIndex = index / 8;
							if (charIndex < SHBDData.size())
							{
								size_t bitIndex = index % 8;

								if ((SHBDData[charIndex] >> bitIndex) & 0x1)
									*pixeloffset = Blocked;
								else
									*pixeloffset = Walkable;
							}

							pixeloffset++;
						}
					}

					NiSourceTexturePtr texture = NiSourceTexture::Create(pixel,kPrefs);
					texture->SetStatic(false);
					NiTexturingPropertyPtr Texture = NiNew NiTexturingProperty;
					Texture->SetBaseTexture(texture);
					Texture->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
					Texture->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);


					BlockShape->AttachProperty(Texture);
					Shapes.push_back(BlockShape);
					//SHBDNode->AttachChild(BlockShape);
				}
				return std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>> (TextureList, Shapes);
			});
		_ThreadList.push_back(future.share());
	}
	{
		using namespace std::chrono_literals;
		auto it = _ThreadList.begin();
		while (it != _ThreadList.end())
		{
			auto status = it->wait_for(10ms);
			if (status == std::future_status::ready)
			{
				TextureConnector.push_back(it->get().first);
				for (auto shape : it->get().second)
					SHBDNode->AttachChild(shape);
				it++;
				continue;
			}
			Sleep(1);
		}
	}
	SHBDNode->Update(0.0f);
	ResetSHBD(GetSpawnPoint());
	return true;
}

void World::ResetSHBD(NiPoint3 SpawnPoint)
{
	SHBDNode->SetTranslate(NiPoint3(0.f, 0.f, SpawnPoint.z - 10.f));
	SHBDNode->UpdateEffects();
	SHBDNode->UpdateProperties();
	SHBDNode->Update(0.0);
}

NiPoint3 World::GetSpawnPoint() 
{
	NiPick _Pick;
	_Pick.SetTarget(GetWorldScene());
	_Pick.SetPickType(NiPick::FIND_ALL);
	_Pick.SetSortType(NiPick::SORT);
	_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
	_Pick.SetFrontOnly(false);
	_Pick.SetReturnNormal(true);
	_Pick.SetObserveAppCullFlag(true);
	NiPoint3 kOrigin(_Info->RegenX, _Info->RegenY, 0.f);
	if (_Pick.PickObjects(kOrigin, World::ms_kUpDir, true))
	{
		NiPick::Results& results = _Pick.GetResults();
		for (int i = 0; i < results.GetSize(); i++)
		{
			auto result = results.GetAt(i);
			if (result->GetIntersection().z > kOrigin.z)
				kOrigin.z = result->GetIntersection().z;
		}

	}
	kOrigin.z += 100.f;
	return kOrigin;
}