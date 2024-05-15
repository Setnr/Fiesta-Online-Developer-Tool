#pragma once

#include "EditorScene.h"

#include <map>
NiSmartPointer(SHBDScene);
class SHBDScene : public StartScene 
{
public:
	SHBDScene(MapInfo* info);
	//SHBDScene(EditorScenePtr _EditorScene);
	bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea)
	{
		_Editor->SetupScene(m_spScene, m_spCamerea);
		//Camera = m_spCamerea; //BothScenes Use the Same Camera And Stuff;
		return true;
	}
	void Update(float fTime)
	{
		_Editor->Update(fTime);
		StartScene::Update(fTime);
	}

	void UpdateCamera(float fTime);
	void Draw(NiRenderer* renderer);
	void DrawImGui();
	void CreateMenuBar();
private:
	EditorScenePtr _Editor;
	NiCameraPtr _SHBDCamera;
	NiNodePtr EditorScenePtr;
	std::vector<char> SHBDData;
	NiDynamicTexturePtr SHBDDataTexture;
	MapInfo* _Info;
	void CreateBrushTexture(NiPoint3& BrushPositon);
	void SaveSHBD();
	unsigned int Blocked;
	unsigned int Walkable;
	unsigned int BrushColor;

	struct SHBDDataStruct {
		unsigned int MapSize;
		unsigned int SHBDSize;
	} _SHBDData;
	float PixelSize;
	bool MoveStatus;
	int BrushSize;
	std::map<unsigned int*, std::vector<std::pair<unsigned int*, unsigned int>>> OffsetMap;
};