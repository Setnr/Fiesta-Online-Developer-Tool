#pragma once
#include <NiMain.h>
#include <iostream>
#include <vector>
#include <NiNode.h>
#include <NiCustom/NiFog.h>
#include <NiCustom/NiPickable.h>
#include <SHN/SHNStruct.h>
#include <set>

NiSmartPointer(ShineMapData);
class ShineMapData : public NiRefObject
{
public:
	ShineMapData() = default;
	bool Load(MapInfo* Info);
	bool Save(MapInfo* Info);
	void CreateEmpty();
	std::vector<NiNodePtr>& GetSkyList() { return SkyList; }
	std::vector<NiNodePtr>& GetWaterList() { return WaterList; }
	std::vector<NiNodePtr>& GetGlobalGroundObjectList() { return GlobalGroundObjectList; }
	std::vector<NiAVObjectPtr>& GetCollisionObjectListList() { return CollisionObjectList; }
	std::set<NiNodePtr>& GetGroundObjectListList() { return GroundObjectList; }
	void SetFogColor(NiColor kColor) { Fog.Color = kColor; }
	NiColor GetFogColor() { return Fog.Color; }
	void SetFogDepth(float depth) { Fog.Depth = depth; }
	float GetFogDepth() { return Fog.Depth; }
	void SetGlobalLightNode(NiNodePtr Node) { GlobalLightNif = Node; }
	NiNodePtr GetGlobalLightNode() { return GlobalLightNif; }
	void SetFarFrustum(float frustum) { Frustum = frustum; }
	float GetFarFrustum() { return Frustum; }
	void SetDirectionalLightAmbientColor(NiColor kColor) { DirectionalLightAmbientColor = kColor; }
	NiColor GetDirectionalLightAmbientColor() { return DirectionalLightAmbientColor; }
	void SetDirectionalLightDiffuseColor(NiColor kColor) { DirectionalLightDiffuseColor = kColor; }
	NiColor GetDirectionalLightDiffuseColor() { return DirectionalLightDiffuseColor; }
	void SetBackgroundColor(NiColor kColor) { BackgroundColor = kColor; }
	NiColor GetBackgroundColor() { return BackgroundColor; }
	void SetGlobalLight(NiColor kColor) { GlobalLight = kColor; }
	NiColor GetGlobalLight() { return GlobalLight; }
	void RemoveSky(NiNodePtr Node)
	{
		for (auto s = SkyList.begin(); s!=SkyList.end(); s++)
		{
			if (*s == Node)
			{
				SkyList.erase(s);
				return;

			}
		}
	}
	void AddSky(NiNodePtr Node) { SkyList.push_back(Node); }
	void AddWater(NiNodePtr Node) { WaterList.push_back(Node); }
	void AddGroundObject(NiNodePtr Node) { GlobalGroundObjectList.push_back(Node); }
	void RemoveWater(NiNodePtr Node)
	{
		for (auto s = WaterList.begin(); s != WaterList.end(); s++)
		{
			if (*s == Node)
			{
				WaterList.erase(s);
				return;

			}
		}
	}
	void RemoveGroundObject(NiNodePtr Node)
	{
		for (auto s = GlobalGroundObjectList.begin(); s != GlobalGroundObjectList.end(); s++)
		{
			if (*s == Node)
			{
				GlobalGroundObjectList.erase(s);
				return;

			}
		}
	}
	void AddObject(NiNodePtr Node) { GroundObjectList.insert(Node); }
	void RemoveObject(NiNodePtr Node) { auto it = GroundObjectList.find(Node); GroundObjectList.erase(it); }
	
private:
	void ExtractCollision(NiNodePtr obj);
	std::string Version;
	std::vector<NiNodePtr> SkyList;
	std::vector<NiNodePtr> WaterList;
	std::vector<NiNodePtr> GlobalGroundObjectList;
	std::vector<NiAVObjectPtr> CollisionObjectList;
	std::vector<NiAVObjectPtr> CameraCollisionObjectList;
	NiNodePtr GlobalLightNif;
	NiColor GlobalLight;
	NiFog Fog;
	NiColor BackgroundColor;
	float Frustum;
	std::set<NiNodePtr> GroundObjectList;
	
	NiColor DirectionalLightAmbientColor;
	NiColor DirectionalLightDiffuseColor;
}; 

