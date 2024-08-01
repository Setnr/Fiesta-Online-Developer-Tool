#pragma once
#include <NiMain.h>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <mutex>
#include "../Data/IniFile.h"
#include "PgUtil.h"
#include "../../Includes/EasyBMP/EasyBMP.h"

NiSmartPointer(TerrainWorld);
class TerrainWorld : public NiRefObject
{
public:
	TerrainWorld(std::string IniFile) : _InitFile(IniFile)
	{
		if (!InitScene())
			return;
		if (!InitCamera())
			return;
		if (!InitSkyCtrl())
			return;
		if (!InitLightFog())
			return;
		if (!InitShadow())
			return;

		this->GetCamera()->SetViewFrustum(this->GetWorldFrustum());

		if (!LoadTerrain())
			return;
	}
	TerrainWorld() : _InitFile(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\BaseIni.ini"))
	{
		if (!InitScene())
			return;
		if (!InitCamera())
			return;
		if (!InitSkyCtrl())
			return;
		if (!InitLightFog())
			return;
		if (!InitShadow())
			return;

		this->GetCamera()->SetViewFrustum(this->GetWorldFrustum());

		if (!LoadTerrain())
			return;
	}
#pragma region GlobalStatics
	static NiPoint3 ms_kUpDir; // idb
	static NiPoint3 ms_kDownDir; // idb
	static NiPoint3 ms_kNorthDir; // idb
	static NiPoint3 ms_kEastDir; // idb
	static NiPoint3 ms_kWestDir; // idb
	static NiPoint3 ms_kDefaultDirectionalLightDir; // idb
#pragma endregion
	struct PointInfos
	{	
		float HTDHeight;
		float HTDGHeight;
		TerrainLayer::RGBAColor PixelColor;
		NiColorA VertexColor;
		float GetHeight() { return HTDHeight + HTDGHeight; }
	};
#pragma region Init
	bool InitScene();
	bool InitCamera();
	bool InitSkyCtrl();
	bool InitLightFog();
	bool InitShadow() { return true; }
#pragma endregion
#pragma region SetterAndGetter
	void SetBackgroundColor(float r, float g, float b)
	{
		BackgroundColor = NiColor(r, g, b);
	}
	NiColor& GetBackgroundColor() {
		return BackgroundColor;
	}
	void AttachSky(NiNodePtr sky)
	{
		m_spSkyScene->AttachChild(sky, 1);
		m_spSkyScene->UpdateProperties();
		m_spSkyScene->UpdateEffects();
		m_spSkyScene->Update(0.0);
	}
	void AttachWater(NiNodePtr water)
	{
		m_spWaterScene->AttachChild(water, 1);
	}
	void AttachGroundObj(NiNodePtr& obj)
	{
		static std::mutex AttachGroundObjLock;
		std::lock_guard<std::mutex> lock(AttachGroundObjLock);
		m_spGroundObject->AttachChild(obj, 1);
	}
	void AttachGroundCollidee(NiNodePtr obj)
	{
		m_spGroundObjectCollidee->AttachChild(obj, 1);
	}
	NiNodePtr GetGroundCollidee()
	{
		return m_spGroundObjectCollidee;
	}
	void AttachGroundTerrain(NiTriShapePtr obj)
	{
		m_spGroundTerrain->AttachChild(obj, 1);
	}
	NiNodePtr GetWorldScene() {
		return m_spWorldScene;
	}
	NiSortAdjustNodePtr GetTerrainScene() {
		return m_spGroundTerrain;
	}
	void ClearTerrainScene() {
		m_spGroundScene->DetachChild(m_spGroundTerrain);
		m_spGroundTerrain = NiNew NiSortAdjustNode;
		m_spGroundScene->AttachChild(m_spGroundTerrain);

	}
	NiNodePtr GetSkyNode() {
		return m_spSkyScene;
	}
	NiNodePtr GetWaterNode()
	{
		return m_spWaterScene;
	}
	NiNodePtr GetGroundObjNode() {
		return m_spGroundObject;
	}
	void SetFogColor(NiColor kColor)
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		fog->SetFogColor(kColor);
		fog->SetFog(true);
	}
	const NiColor& GetFogColor()
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		return fog->GetFogColor();
	}
	float GetFogDepth()
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		return fog->GetDepth();
	}
	void SetFogDepth(float fDepth)
	{
		NiFogProperty* fog = (NiFogProperty*)m_spWorldScene->GetProperty(NiProperty::FOG);
		if (fDepth < 0.0)
			fDepth = 0.0;
		fog->SetDepth(fDepth);
	}
	void SetGlobalLight(NiNodePtr LightNode) {
		this->m_spLightArea->AttachChild(LightNode);
	}
	void SetFarFrumstum(float Frustum)
	{
		m_kWorldFrustum.m_fFar = Frustum;
	}
	NiFrustum& GetSkyFrustum() {
		return m_kSkyFrustum;
	}
	NiFrustum& GetWorldFrustum() {
		return m_kWorldFrustum;
	}
	void SetMapDirectionalLightAmbientColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetAmbientColor(kColor);
	}
	const NiColor& GetMapDirectionalLightAmbientColor()
	{
		return m_spMapDirectionalLight->GetAmbientColor();
	}
	void SetMapDirectionalLightDiffuseColor(NiColor kColor)
	{
		m_spMapDirectionalLight->SetDiffuseColor(kColor);
	}

	const NiColor& GetMapDirectionalLightDiffuseColor()
	{
		return m_spMapDirectionalLight->GetDiffuseColor();
	}
	void SetAmbientLightAmbientColor(NiColor kColor)
	{
		m_spAmbientLight->SetAmbientColor(kColor);
	}
	const NiColor& GetAmbientLightAmbientColor()
	{
		return m_spAmbientLight->GetAmbientColor();
	}
	NiCameraPtr GetCamera() { return m_spCamera; }

	float GetFOV() { return this->m_fCameraFOV; }
	
	bool WasLoadedSuccessfully() { return LoadedSuccessfully; }
	IniFile& GetIniFile() { return _InitFile; }
	void CreateTerrainLayer(std::shared_ptr<TerrainLayer> CurrentLayer);
	std::vector<std::vector<PointInfos>>& GetHTD() { return VertexMap; }
	virtual NiPoint3 GetSpawnPoint();
	virtual std::string GetFolderPath() 
	{
		return _InitFile.HeightFileName.substr(0, _InitFile.HeightFileName.find_last_of("\\"));
	}
#pragma endregion
	void ReloadTerrain() {
		ClearTerrainScene();
		for (auto CurrentLayer : _InitFile.LayerList)
		{
			CreateTerrainLayer(CurrentLayer);
		}
	}
	struct HTDPointData 
	{
		NiPoint3* NiPoint;
		NiGeometryData* NiGeometry;
		NiColorA* NiColor;
	};
	std::vector<HTDPointData> GetHTDPoints(int w, int h)
	{
		std::vector<HTDPointData> vec;
		int XBlocks = (_InitFile.HeightMap_width - 1) / _InitFile.QuadsWide;
		int YBlocks = (_InitFile.HeightMap_height - 1) / _InitFile.QuadsHigh;

		int SubLayerW = w / _InitFile.QuadsWide;
		int SubLayerH = h / _InitFile.QuadsHigh;

		int SubW = w % (_InitFile.QuadsWide );
		int SubH = h % (_InitFile.QuadsHigh);

		int CurWith = _InitFile.QuadsWide + 1;

		NiSortAdjustNodePtr Terrain = GetTerrainScene();
		for (int LayerID = 0; LayerID < Terrain->GetChildCount(); LayerID++)
		{
			NiAVObjectPtr obj = Terrain->GetAt(LayerID);
			if(NiIsKindOf(NiNode,obj) && obj)
			{
				NiNodePtr Layer = NiSmartPointerCast(NiNode, obj);
				struct POS
				{
					NiAVObjectPtr obj;
					int w;
					int h;
				};
				std::vector<POS> LayerList;
				if(SubW == 0 && SubLayerW >= 1)
				{
					LayerList.push_back({ Layer->GetAt(SubLayerH + (SubLayerW - 1) * YBlocks), _InitFile.QuadsWide, SubH });
				}
				if (SubH == 0 && SubLayerH >= 1)
				{
					LayerList.push_back({ Layer->GetAt(SubLayerH + SubLayerW * YBlocks - 1), SubW, _InitFile.QuadsHigh });
				}
				if (SubW == 0 && SubLayerW >= 1 && SubH == 0 && SubLayerH >= 1)
					LayerList.push_back({ Layer->GetAt(SubLayerH + (SubLayerW - 1) * YBlocks - 1), _InitFile.QuadsWide, _InitFile.QuadsHigh });
				LayerList.push_back({Layer->GetAt(SubLayerH + SubLayerW * YBlocks), SubW, SubH });
				for(auto _Layer : LayerList)
				{
					if (!_Layer.obj)
						continue;
					if (NiIsKindOf(NiTriShape, _Layer.obj))
					{
						NiTriShapePtr shape = NiSmartPointerCast(NiTriShape, _Layer.obj);
						NiGeometryData* data = shape->GetModelData();

						NiPoint3* Points = shape->GetVertices();
						NiPoint3* RelevantPoint = NULL;
						NiColorA* col = shape->GetColors();
						NiColorA* RelevantCol = NULL;
						if (_Layer.h <= 1)
						{

							if ((_Layer.h == 1 && _Layer.w == 0) || (_Layer.h == 0 && (_Layer.w == 0 || _Layer.w == 1)))
							{
								if (_Layer.h == 0 && _Layer.w == 0)
								{
									RelevantPoint = &Points[0];
									RelevantCol = &col[0];
								}
								if (_Layer.h == 0 && _Layer.w == 1)
								{
									RelevantPoint = &Points[1];
									RelevantCol = &col[1];
								}
								if (_Layer.h == 1 && _Layer.w == 0)
								{
									RelevantPoint = &Points[2];
									RelevantCol = &col[2];
								}
							}
							else
							{
								if (_Layer.h == 1)
								{
									RelevantPoint = &Points[(_Layer.w + 1) * 2 - 1];
									RelevantCol = &col[(_Layer.w + 1) * 2 - 1];
								}
								if (_Layer.h == 0)
								{
									RelevantPoint = &Points[(_Layer.w) * 2];
									RelevantCol = &col[(_Layer.w) * 2];
								}
							}

						}
						else
						{
							RelevantPoint = &Points[_Layer.w + _Layer.h * CurWith];
							RelevantCol = &col[_Layer.w + _Layer.h * CurWith];
						}
						vec.push_back({ RelevantPoint,data,RelevantCol });
					}
					else
					{
						LogError("Ah NiTriShape was expected and GetHTDPoints havent recieved one");
					}
				}
			}

		}
		return vec;
	}

	void SaveVertexColor(bool backup = false)
	{
		auto vertexpath = PgUtil::CreateFullFilePathFromBaseFolder(_InitFile.VertexColorTexture);
		if (backup)
			vertexpath += ".auto";
		try {
			BMP bmp;
			bmp.SetSize(VertexMap.size(), VertexMap[0].size());
			for (int w = 0; w < VertexMap.size(); w++)
			{
				for (int h = VertexMap[0].size() - 1; h >= 0; h--)
				{
					RGBApixel pixel;
					pixel.Red =static_cast<unsigned char> (VertexMap[w][h].VertexColor.r * 255.f);
					pixel.Green = static_cast<unsigned char> (VertexMap[w][h].VertexColor.g * 255.f);
					pixel.Blue = static_cast<unsigned char> (VertexMap[w][h].VertexColor.b * 255.f);
					pixel.Alpha = static_cast<unsigned char> (VertexMap[w][h].VertexColor.a * 255.f);
					bmp.SetPixel(w, VertexMap[0].size() - h - 1, pixel);
				}
			}
			bmp.WriteToFile(vertexpath.c_str());
			}
		catch (std::exception e)
		{
			LogError("Failed to safe: " + vertexpath);
			LogError("Error-Msg: " + e.what());
		}
		catch (...)
		{
			LogError("Failed to safe: " + vertexpath);
		}
	}
protected:
#pragma region WorldStructureNodes
	NiNodePtr m_spLightArea;
	NiNodePtr m_spFog;
	NiNodePtr m_spWorldScene;
	NiNodePtr m_spCharScene;
	NiNodePtr m_spDropItemScene;
	NiNodePtr m_spGroundScene;
	NiNodePtr m_spShadowScene;
	NiNodePtr m_spGroundObject;
	NiNodePtr m_spDirectionalLightScene;
	NiNodePtr m_spMinihouseScene;
	NiNodePtr m_spNormalLightScene;
	NiNodePtr m_spAbstateCharScene;
	NiNodePtr m_spWeatherEffectScene;
	NiSortAdjustNodePtr m_spGroundTerrain;
	NiNodePtr m_spAllGroundScene;
	NiSortAdjustNodePtr m_spBuildingScene;
	NiNodePtr m_spBDAni;
	NiNodePtr m_spBDNotAni;
	NiNodePtr m_spSkyScene;
	NiNodePtr m_spWaterScene;
	NiNodePtr m_spEffectScene;
	NiNodePtr m_spGroundCollidee;
	NiNodePtr m_spGroundObjectCollidee;
	NiNodePtr m_spBuildingCollidee;
	NiNodePtr m_spUseCameraCollidee;
	NiCameraPtr m_spCamera;
	float m_fCameraFOV;
	bool m_bCameraOrtho;
	NiFrustum m_kSkyFrustum;
	NiFrustum m_kWorldFrustum;
	NiAmbientLightPtr m_spAmbientLight;
	NiDirectionalLightPtr m_spDirectionalLight;
	NiDirectionalLightPtr m_spMapDirectionalLight;

	NiColor BackgroundColor;
#pragma endregion
	bool LoadedSuccessfully = false;

	bool LoadTerrain();
	IniFile _InitFile;	

	struct Triangle
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	std::vector<std::vector<PointInfos>> VertexMap;
	NiNodePtr GetLayerNode(std::string LayerName);

	NiAlphaPropertyPtr prop;
};