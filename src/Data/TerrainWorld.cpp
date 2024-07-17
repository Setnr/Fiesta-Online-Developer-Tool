#include "TerrainWorld.h"
#include <filesystem>
#include <thread>
#include <future>
#include "../Logger/Logger.h"
#include <NiPick.h>
NiPoint3 TerrainWorld::ms_kUpDir = NiPoint3(0.0, 0.0, 1.0);
NiPoint3 TerrainWorld::ms_kDownDir = NiPoint3(0.0, 0.0, -1.0);
NiPoint3 TerrainWorld::ms_kNorthDir = NiPoint3(0.0, 1.0, 0.0);
NiPoint3 TerrainWorld::ms_kEastDir = NiPoint3(1.0, 0.0, 0.0);
NiPoint3 TerrainWorld::ms_kWestDir = NiPoint3(-1.0, 0.0, 0.0);
NiPoint3 TerrainWorld::ms_kDefaultDirectionalLightDir = NiPoint3(0.5, 0.69999999, -0.40000001);

bool TerrainWorld::LoadTerrain()
{
	if (!_InitFile.Load())
		return false;
	if (_InitFile.FileType == "")return true;
	for (int i = 0; i < m_spGroundTerrain->GetChildCount(); i++)
		m_spGroundTerrain->DetachChildAt(i);
	m_spGroundTerrain->CompactChildArray();
	std::string HTDFilePath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.HeightFileName);
	std::ifstream HTDFile;


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
	for (auto CurrentLayer : _InitFile.LayerList)
	{
		CreateTerrainLayer(CurrentLayer);
	}
}

NiNodePtr TerrainWorld::GetLayerNode(std::string LayerName)
{
	NiSortAdjustNodePtr Terrain = GetTerrainScene();
	/*
	* Look if Layer Allready exists if yes detach it and recreate it
	*/

	NiNodePtr LayerNode = NiNew NiNode;
	LayerNode->SetName(LayerName.c_str());
	bool SkipAttach = false;
	Terrain->CompactChildArray();
	for (unsigned int i = 0; i < Terrain->GetChildCount(); i++)
	{
		NiAVObjectPtr child = Terrain->GetAt(i);
		if (child && child->GetName().Equals(LayerName.c_str())&& NiIsKindOf(NiNode, child))
		{
			Terrain->SetAt(i, LayerNode);
			SkipAttach = true;;
		}
	}
	if(!SkipAttach)
		Terrain->AttachChild(LayerNode);
	return LayerNode;
}

void TerrainWorld::CreateTerrainLayer(std::shared_ptr<TerrainLayer> CurrentLayer) 
{
	NiNodePtr LayerNode = GetLayerNode(CurrentLayer->Name);

	auto ColorArray = (TerrainLayer::RGBAColor*)CurrentLayer->pixldata->GetPixels();
	int PixelCounter = 0;
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
	struct PointUsed 
	{
		short BL = -1;
		short BR = -1;
		short TL = -1;
		short TR = -1;
		bool Used = false;
	};
	std::vector<std::vector<PointUsed>> PointUsage(VertexMap.size());
	for (int i = 0; i < VertexMap.size(); i++) {
		PointUsage[i] = std::vector<PointUsed>(VertexMap[i].size());
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

						PointUsed* info = &PointUsage[ActiveW][ActiveH];
						info->Used = true;
						bool AddBL = true;
						bool AddBR = true;
						bool AddTL = true;
						bool AddTR = true;

						if (h != 0 || w != 0)
						{
							if (w != 0)
							{
								PointUsed* PointInfosLeft = &PointUsage[ActiveW - 1][ActiveH];
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
								PointUsed* LowerInfos = &PointUsage[ActiveW][ActiveH - 1];
								if (LowerInfos->Used)
								{
									info->BL = LowerInfos->TL;
									info->BR = LowerInfos->TR;
									AddBL = false;
									AddBR = false;
								}
								if (w != 0)
								{
									PointUsed* LowerInfosL = &PointUsage[ActiveW - 1][ActiveH - 1];
									if (LowerInfosL->Used)
									{
										info->BL = LowerInfosL->TR;
										AddBL = false;
									}
								}
								if (ActiveW + 1 < _InitFile.HeightMap_width)
								{
									PointUsed* LowerInfosR = &PointUsage[ActiveW + 1][ActiveH - 1];
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
							VerticesList.push_back(NiPoint3(ActiveW * _InitFile.OneBlock_width, ActiveH * _InitFile.OneBlock_height, VertexMap[ActiveW][ActiveH].Height));
							WHList.push_back({ ActiveW,ActiveH });
							NormalList.push_back(TerrainWorld::ms_kUpDir);
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
							NormalList.push_back(TerrainWorld::ms_kUpDir);
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
							NormalList.push_back(TerrainWorld::ms_kUpDir);
							ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
							TextureList1.push_back(NiPoint2(PixelW * FirstWScale, (PixelH + 1) * FirstHScale));
							TextureList2.push_back(NiPoint2(PixelW * SecondWScale, (PixelH + 1) * SecondHScale));
						}
						if (AddTR)
						{
							info->TR = TriCt;
							TriCt++;
							VerticesList.push_back(NiPoint3((ActiveW + 1) * _InitFile.OneBlock_width, (ActiveH + 1) * _InitFile.OneBlock_height, VertexMap[ActiveW + 1][ActiveH + 1].Height));
							WHList.push_back({ ActiveW + 1,ActiveH + 1 });
							NormalList.push_back(TerrainWorld::ms_kUpDir);
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
			LayerNode->AttachChild(Shape);
		}
	}

	LayerNode->UpdateEffects();
	LayerNode->UpdateProperties();
	LayerNode->Update(0.0);
}

NiPoint3 TerrainWorld::GetSpawnPoint()
{
	NiPick _Pick;
	_Pick.SetTarget(GetWorldScene());
	_Pick.SetPickType(NiPick::FIND_ALL);
	_Pick.SetSortType(NiPick::SORT);
	_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
	_Pick.SetFrontOnly(false);
	_Pick.SetReturnNormal(true);
	_Pick.SetObserveAppCullFlag(true);
	NiPoint3 kOrigin(0.f, 0.f, 0.f);
	if (_Pick.PickObjects(kOrigin, TerrainWorld::ms_kUpDir, true))
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