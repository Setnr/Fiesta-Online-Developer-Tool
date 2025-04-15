#include "IngameWorld.h"
#include <PgUtil.h>
#include <filesystem>
#include <Logger.h>
#include <NiImageConverter.h>
bool IngameWorld::InitScene()
{
	m_spLightArea = NiNew NiNode;
	m_spFog = NiNew NiNode;
	m_spWorldScene = NiNew NiNode;
	m_spCharScene = NiNew NiNode;
	m_spDropItemScene = NiNew NiNode;
	m_spGroundScene = NiNew NiNode;
	m_spShadowScene = NiNew NiNode;;
	m_spGroundObject = NiNew NiNode;
	m_spDirectionalLightScene = NiNew NiNode;
	m_spMinihouseScene = NiNew NiNode;
	m_spNormalLightScene = NiNew NiNode;
	m_spAbstateCharScene = NiNew NiNode;
	m_spWeatherEffectScene = NiNew NiNode;
	m_spGroundTerrain = NiNew NiSortAdjustNode;
	m_spAllGroundScene = NiNew NiNode;
	m_spBuildingScene = NiNew NiSortAdjustNode;
	m_spBDAni = NiNew NiNode;
	m_spBDNotAni = NiNew NiNode;
	m_spSkyScene = NiNew NiNode;
	m_spWaterScene = NiNew NiNode;
	m_spEffectScene = NiNew NiNode;
	m_spGroundCollidee = NiNew NiNode;
	m_spGroundObjectCollidee = NiNew NiNode;
	m_spBuildingCollidee = NiNew NiNode;
	m_spUseCameraCollidee = NiNew NiNode;



	m_spLightArea->SetName("m_spLightArea");
	m_spFog->SetName("m_spFog");
	m_spWorldScene->SetName("m_spWorldScene");
	m_spCharScene->SetName("m_spCharScene");
	m_spDropItemScene->SetName("m_spDropItemScene");
	m_spGroundScene->SetName("m_spGroundScene");
	m_spShadowScene->SetName("m_spShadowScene");
	m_spGroundObject->SetName("m_spGroundObject");
	m_spDirectionalLightScene->SetName("m_spDirectionalLightScene");
	m_spMinihouseScene->SetName("m_spMinihouseScene");
	m_spNormalLightScene->SetName("m_spNormalLightScene");
	m_spAbstateCharScene->SetName("m_spAbstateCharScene");
	m_spWeatherEffectScene->SetName("m_spWeatherEffectScene");
	m_spGroundTerrain->SetName("m_spGroundTerrain");
	m_spAllGroundScene->SetName("m_spAllGroundScene");
	m_spBuildingScene->SetName("m_spBuildingScene");
	m_spSkyScene->SetName("m_spSkyScene");
	m_spWaterScene->SetName("m_spWaterScene");
	m_spGroundCollidee->SetName("m_spGroundCollidee");
	m_spGroundObjectCollidee->SetName("m_spGroundObjectCollidee");
	m_spBuildingCollidee->SetName("m_spBuildingCollidee");

	m_spBuildingScene->SetName("[PN]");


	m_spBuildingScene->AttachChild(m_spGroundScene);
	m_spBuildingScene->AttachChild(m_spBDNotAni);
	m_spBuildingScene->AttachChild(m_spBDAni);
	m_spGroundScene->AttachChild(m_spGroundObject);
	//m_spGroundScene->AttachChild(m_spGroundObjectCollidee);
	m_spGroundScene->AttachChild(m_spGroundTerrain);
	m_spAllGroundScene->AttachChild(m_spBuildingScene);
	m_spNormalLightScene->AttachChild(m_spAllGroundScene);
	m_spNormalLightScene->AttachChild(m_spDirectionalLightScene);
	m_spDirectionalLightScene->AttachChild(m_spCharScene);
	m_spNormalLightScene->AttachChild(m_spShadowScene);
	m_spNormalLightScene->AttachChild(m_spDropItemScene);
	m_spDirectionalLightScene->AttachChild(m_spMinihouseScene);
	m_spNormalLightScene->AttachChild(m_spWaterScene);
	m_spNormalLightScene->AttachChild(m_spEffectScene);
	m_spWorldScene->AttachChild(m_spNormalLightScene);
	m_spWorldScene->AttachChild(m_spAbstateCharScene);
	m_spWorldScene->AttachChild(m_spLightArea);
	m_spWorldScene->AttachChild(m_spFog);

	NiAlphaPropertyPtr alpha = NiNew NiAlphaProperty;
	NiVertexColorPropertyPtr Vertex = NiNew NiVertexColorProperty;
	NiFogPropertyPtr Fog = NiNew NiFogProperty;
	m_spShadowScene->AttachProperty(alpha);
	m_spShadowScene->AttachProperty(Vertex);
	m_spShadowScene->AttachProperty(Fog);
	Vertex = NiNew NiVertexColorProperty;
	NiMaterialPropertyPtr Material = NiNew NiMaterialProperty;
	Fog = NiNew NiFogProperty;
	m_spWorldScene->AttachProperty(Vertex);
	m_spWorldScene->AttachProperty(Material);
	m_spWorldScene->AttachProperty(Fog);

	NiZBufferPropertyPtr ZBuff = NiNew NiZBufferProperty;
	m_spSkyScene->AttachProperty(ZBuff);
	return true;
}
bool IngameWorld::InitCamera()
{
	m_spCamera = NiNew NiCamera;
	m_bCameraOrtho = false;
	NiFrustum SkyFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 10000.0, m_bCameraOrtho);
	m_kSkyFrustum = SkyFrustum;
	NiFrustum WorldFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 6000.0, m_bCameraOrtho);
	m_kWorldFrustum = WorldFrustum;
	m_spCamera->SetViewFrustum(m_kWorldFrustum); //World::SetCameraFOV


	m_fCameraFOV = 50.0f;
	float fTop = m_fCameraFOV / 180.0 * 0.5 * NI_PI;
	float fTopa = tan(fTop);
	float v4 = fTopa;
	float fRight = fTopa;
	float v5 = 1600; /*TODO DYNAMIC*/
	float v6 = 900;
	float fTopb;
	if (900 >= (double)1600)
	{
		fTopb = v4 * (v6 / v5);
		v4 = fTopb;
	}
	else
	{
		fRight = v5 / v6 * v4;
	}
	float fTopc = -fRight;
	float v7 = fTopc;
	this->m_kWorldFrustum.m_fLeft = fTopc;
	this->m_kWorldFrustum.m_fRight = fRight;
	this->m_kWorldFrustum.m_fTop = v4;
	float fTopd = -v4;
	this->m_kWorldFrustum.m_fBottom = fTopd;
	this->m_kSkyFrustum.m_fBottom = fTopd;
	this->m_kSkyFrustum.m_fLeft = v7;
	this->m_kSkyFrustum.m_fRight = fRight;
	this->m_kSkyFrustum.m_fTop = v4;
	return true;
}
bool IngameWorld::InitSkyCtrl()
{
	return 1;
}
bool IngameWorld::InitLightFog()
{
	m_spAmbientLight = NiNew NiAmbientLight;
	m_spAmbientLight->SetAmbientColor(NiColor(0.2, .2, .2));
	m_spAmbientLight->SetDiffuseColor(NiColor::WHITE);
	m_spAmbientLight->SetSpecularColor(NiColor::BLACK);
	m_spAmbientLight->AttachAffectedNode(m_spNormalLightScene);
	m_spNormalLightScene->AttachChild(m_spAmbientLight);
	m_spDirectionalLight = NiNew NiDirectionalLight;
	m_spDirectionalLight->SetDimmer(1.0);
	m_spDirectionalLight->SetAmbientColor(NiColor::BLACK);
	m_spDirectionalLight->SetDiffuseColor(NiColor::WHITE);
	m_spDirectionalLight->SetSpecularColor(NiColor::BLACK);
	m_spDirectionalLightScene->AttachChild(m_spDirectionalLight);
	m_spDirectionalLight->AttachAffectedNode(m_spDirectionalLightScene);
	m_spDirectionalLight->SetRotate(1.f, 0.5, 0.7, -0.4);
	m_spMapDirectionalLight = NiNew NiDirectionalLight;
	m_spMapDirectionalLight->SetAmbientColor(NiColor(.75, .75, .75));
	m_spMapDirectionalLight->SetDiffuseColor(NiColor::WHITE);
	m_spMapDirectionalLight->SetSpecularColor(NiColor::BLACK);
	m_spMapDirectionalLight->SetSwitch(false); //This one kinda sucks xD
	m_spMapDirectionalLight->SetDimmer(1.0f);

	m_spAllGroundScene->AttachChild(m_spMapDirectionalLight);
	m_spMapDirectionalLight->AttachAffectedNode(m_spAllGroundScene);
	m_spAllGroundScene->UpdateEffects();


	return 1;
}
bool IngameWorld::LoadHTD()
{
	auto Path = PgUtil::PathFromClientFolder(_INI->GetHTDPath());
	if (!std::filesystem::exists(Path))
	{
		_HTD = NULL;
		return false;
	}
	_HTD = NiNew HeightTerrainData();
	if (!_HTD->Load(Path, _INI->GetMapWidth(), _INI->GetMapHeight()))
	{
		LogError("Failed to Load HTD for " + _MapInfo->MapName);
		return false;
	}
	return true;
}
bool IngameWorld::InitShadow()
{
	return 1;
}
void IngameWorld::CreateAndAttachTerrain() 
{
	m_spGroundTerrain->RemoveAllChildren();
	for (auto CurrentLayer : _INI->GetLayers())
	{
		CreateTerrainLayer(CurrentLayer);
	}
}
void IngameWorld::CreateTerrainLayer(std::shared_ptr<TerrainLayerData> CurrentLayer) 
{
	if (!HasHTD())
		return;
	struct PointUsed
	{
		short BL = -1;
		short BR = -1;
		short TL = -1;
		short TR = -1;
		bool Used = false;
	};
	struct Triangle
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	std::vector<PointUsed> PointUsage(_INI->GetMapWidth()* _INI->GetMapHeight());

	for (int BlockX = 0; BlockX < (_INI->GetMapWidth() - 1) / _INI->GetQuadsWide(); BlockX++) //19
	{
		for (int BlockY = 0; BlockY < (_INI->GetMapHeight() - 1) / _INI->GetQuadsHigh(); BlockY++) //19
		{
			std::vector<NiPoint3> VerticesList;
			std::vector<NiPoint3> NormalList;
			std::vector<NiColorA> ColorList;
			std::vector<NiPoint2> TextureList1;
			std::vector<NiPoint2> TextureList2;
			std::vector<Triangle> TriangleList;
			int TriCt = 0;
			for (int h = 0; h < _INI->GetQuadsHigh(); h++)
			{
				for (int w = 0; w < _INI->GetQuadsWide(); w++)
				{
					int ActiveW = BlockX * _INI->GetQuadsWide() + w;
					int ActiveH = BlockY * _INI->GetQuadsHigh() + h;
					int PixelW = ActiveW - CurrentLayer->StartPos_X;
					int PixelH = ActiveH - CurrentLayer->StartPos_Y;
					if (!(PixelW >= 0 && PixelW < CurrentLayer->Width))
						continue;
					if (!(PixelH >= 0 && PixelH < CurrentLayer->Height))
						continue;

					//if (PixelInfo->PixelColor > TerrainLayer::RGBColor(static_cast<char>(CurrentLayer->UVScaleBlend)))
					{

						PointUsed* info = &PointUsage[ActiveW + _INI->GetMapWidth() * ActiveH];
						info->Used = true;
						bool AddBL = true;
						bool AddBR = true;
						bool AddTL = true;
						bool AddTR = true;

						if (h != 0 || w != 0)
						{
							if (w != 0)
							{
								PointUsed* PointInfosLeft = &PointUsage[ActiveW + _INI->GetMapWidth() * ActiveH - 1];
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
								PointUsed* LowerInfos = &PointUsage[ActiveW + _INI->GetMapWidth() * (ActiveH - 1)];
								if (LowerInfos->Used)
								{
									info->BL = LowerInfos->TL;
									info->BR = LowerInfos->TR;
									AddBL = false;
									AddBR = false;
								}
								if (w != 0)
								{
									PointUsed* LowerInfosL = &PointUsage[ActiveW + _INI->GetMapWidth() * (ActiveH - 1) - 1];
									if (LowerInfosL->Used)
									{
										info->BL = LowerInfosL->TR;
										AddBL = false;
									}
								}
								if (ActiveW + 1 < _INI->GetMapWidth())
								{
									PointUsed* LowerInfosR = &PointUsage[ActiveW + _INI->GetMapWidth() * (ActiveH - 1) + 1];
									if (LowerInfosR->Used)
									{
										info->BR = LowerInfosR->TL;
										AddBR = false;
									}
								}
							}
						}
						float FirstWScale = 1 / CurrentLayer->UVScaleDiffuse; // (static_cast<float>(_INI->GetQuadsWide()) / static_cast<float>(_INI->GetMapWidth() - 1));
						float FirstHScale = 1 / CurrentLayer->UVScaleDiffuse; // (static_cast<float>(_INI->GetQuadsHigh()) / static_cast<float>(_INI->GetMapHeight() - 1));
						float SecondWScale = (1 / static_cast<float>(CurrentLayer->Width - 1));
						float SecondHScale = (1 / static_cast<float>(CurrentLayer->Height - 1));

						if (AddBL)
						{
							info->BL = TriCt;
							TriCt++;
							VerticesList.push_back(NiPoint3(ActiveW * _INI->GetOneBlockWidht(), ActiveH * _INI->GetOneBlockHeight(), _HTD->GetHTD(ActiveW,ActiveH) + _HTD->GetHTDG(ActiveW, ActiveH)));
							NormalList.push_back(NiPoint3::UNIT_Z);
							ColorList.push_back(_INI->GetColor(ActiveW,ActiveH));
							TextureList1.push_back(NiPoint2(PixelW * FirstWScale, PixelH * FirstHScale));
							TextureList2.push_back(NiPoint2(PixelW * SecondWScale, PixelH * SecondHScale));
						}
						if (AddBR)
						{
							info->BR = TriCt;
							TriCt++;
							VerticesList.push_back(NiPoint3((ActiveW + 1) * _INI->GetOneBlockWidht(), ActiveH * _INI->GetOneBlockHeight(), _HTD->GetHTD(ActiveW + 1, ActiveH) + _HTD->GetHTDG(ActiveW + 1, ActiveH)));
							NormalList.push_back(NiPoint3::UNIT_Z);
							ColorList.push_back(_INI->GetColor(ActiveW, ActiveH));
							TextureList1.push_back(NiPoint2((PixelW + 1) * FirstWScale, PixelH * FirstHScale));
							TextureList2.push_back(NiPoint2((PixelW + 1) * SecondWScale, PixelH * SecondHScale));
						}
						if (AddTL)
						{
							info->TL = TriCt;
							TriCt++; 
							VerticesList.push_back(NiPoint3(ActiveW * _INI->GetOneBlockWidht(), (ActiveH + 1) * _INI->GetOneBlockHeight(), _HTD->GetHTD(ActiveW, ActiveH+1) + _HTD->GetHTDG(ActiveW, ActiveH+1)));
							NormalList.push_back(NiPoint3::UNIT_Z);
							ColorList.push_back(_INI->GetColor(ActiveW, ActiveH));
							TextureList1.push_back(NiPoint2(PixelW * FirstWScale, (PixelH + 1) * FirstHScale));
							TextureList2.push_back(NiPoint2(PixelW * SecondWScale, (PixelH + 1) * SecondHScale));
						}
						if (AddTR)
						{
							info->TR = TriCt;
							TriCt++;
							VerticesList.push_back(NiPoint3((ActiveW + 1) * _INI->GetOneBlockWidht(), (ActiveH + 1) * _INI->GetOneBlockHeight(), _HTD->GetHTD(ActiveW + 1, ActiveH + 1) + _HTD->GetHTDG(ActiveW + 1, ActiveH + 1)));
							NormalList.push_back(NiPoint3::UNIT_Z);
							ColorList.push_back(_INI->GetColor(ActiveW, ActiveH));
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


			Shape->SetSortObject(false);
			NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
			//alphaprop->SetAlphaBlending(false);
			//alphaprop->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
			//alphaprop->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
			//alphaprop->SetAlphaTesting(true);
			//alphaprop->SetTestMode(NiAlphaProperty::TestFunction::TEST_ALWAYS);


			NiTexturingProperty::Map* NormalMap = NiNew NiTexturingProperty::Map(NULL, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP_MIPNEAREST, 0);

			NormalMap->SetFilterMode(NiTexturingProperty::FILTER_BILERP_MIPNEAREST);
			NormalMap->SetClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
			NormalMap->SetTexture(NULL);

			NiTexturingPropertyPtr pkTP = NiNew NiTexturingProperty();
			NiSourceTexturePtr BaseTexture = CurrentLayer->BaseTexture;
			NiSourceTexturePtr BlendTexture = CurrentLayer->BlendTexture;
			NiTexturingProperty::ShaderMap* BaseTextureMap = NiNew NiTexturingProperty::ShaderMap(BaseTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
			BaseTextureMap->SetID(0);
			NiTexturingProperty::ShaderMap* BlendTextureMap = NiNew NiTexturingProperty::ShaderMap(BlendTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
			BlendTextureMap->SetID(1);
			BaseTextureMap->SetTexture(BaseTexture);
			BlendTextureMap->SetTexture(BlendTexture);
			pkTP->SetShaderMap(0, BaseTextureMap);
			pkTP->SetShaderMap(1, BlendTextureMap);
			pkTP->SetBaseClampMode(NiTexturingProperty::WRAP_S_WRAP_T);


			pkTP->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
			pkTP->SetBaseMap(NormalMap);
			Shape->AttachProperty(pkTP);
			NiVertexColorPropertyPtr vertex = NiNew NiVertexColorProperty;
			vertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E_A_D);
			vertex->SetSourceMode(NiVertexColorProperty::SOURCE_AMB_DIFF);
			Shape->AttachProperty(vertex);

			Shape->AttachProperty(alphaprop);

			Shape->CalculateNormals();

			Shape->ApplyAndSetActiveMaterial("PgTerrain");
			Shape->RemoveMaterial("");
			Shape->Update(0.0);
			Shape->UpdateEffects();
			Shape->UpdateProperties();
			Shape->Update(0.0);

			Shape->SetConsistency(NiGeometryData::Consistency::MUTABLE);
			m_spGroundTerrain->AttachChild(Shape);
		}
	}
}