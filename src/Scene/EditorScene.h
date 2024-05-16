#pragma once
#include "StartScene.h"
#include "IngameWorld.h"
#include "IniFile.h"

#include <iostream>
#include <string>
#include <NiThread.h>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <mutex>

#include "NiPickable.h"

#include "ImGui/imgui.h"
#include "SHNStruct.h"
#include "SHBD.h"

NiSmartPointer(EditorScene);

class EditorScene : public StartScene
{
public:
	struct ObjectPosition 
	{
		NiPoint3 pos;
		NiQuaternion quater;
		float Scale;
	};

	EditorScene(MapInfo* info);
	~EditorScene() 
	{
		BaseNode = 0;
		SelectedObj = NULL;
	}
	bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea) 
	{
		m_spScene = BaseNode;
		m_spCamerea = Camera;
		return true;
	}
	void Draw(NiRenderer* renderer);
	void DrawImGui();
	void Update(float fTime)
	{
		kWorld.GetSkyNode()->Update(fTime);
		FiestaScene::Update(fTime);
	}
	bool LoadTerrain();
	void UpdateCamera(float fTime) override;
	virtual void CreateMenuBar();
	bool GetOneBlockSize(float* width, float* height)
	{
		*width = _InitFile.OneBlock_width;
		*height = _InitFile.OneBlock_height;
		if (_InitFile.OneBlock_width > 0.0f && _InitFile.OneBlock_height > 0.0f)
			return true;
		return false;
	}
	World kWorld;
private:
	void FlipVertical(NiPoint3* BlockPoints, int QuadsWide, int QuadsHigh)
	{
		int HalfW = QuadsWide / 2;
		int HalfH = QuadsHigh / 2;
		for (int h = 0; h <= HalfH; h++)
		{
			for (int w = 0; w < QuadsWide + 1; w++)
			{
				int secondh = QuadsHigh - h;
				NiPoint3 BottomPoint = BlockPoints[w + h * (QuadsWide + 1)];
				NiPoint3 TopPoint = BlockPoints[w + secondh * (QuadsWide + 1)];

				BlockPoints[w + h * (QuadsWide + 1)] = TopPoint;
				BlockPoints[w + secondh * (QuadsWide + 1)] = BottomPoint;
			}
		}
	}
	void RotateRight(NiPoint3* BlockPoints, int QuadsWide, int QuadsHigh)
	{
		for (int h = 0; h < QuadsHigh / 2; h++)
		{
			for (int w = 0; w <= QuadsWide / 2; w++)
			{
				int secondh = QuadsHigh - h;
				int secondw = QuadsWide - w;
				int TL = w + h * (QuadsWide + 1);
				int BR = secondw + secondh * (QuadsWide + 1);


				int TR = QuadsWide - h + (QuadsWide + 1) * w;
				int BL = h + secondw * (QuadsWide + 1);
				NiPoint3 TopLeft = BlockPoints[TL];
				NiPoint3 TopRight = BlockPoints[TR];
				NiPoint3 BotLeft = BlockPoints[BL];
				NiPoint3 BotRight = BlockPoints[BR];

				BlockPoints[TL] = BotLeft;
				BlockPoints[TR] = TopLeft;
				BlockPoints[BR] = TopRight;
				BlockPoints[BL] = BotRight;
			}
		}
	}
	
	bool CheckSHMDVersion(std::ifstream& SHMD);
	bool LoadSky(std::ifstream& SHMD);
	bool LoadWater(std::ifstream& SHMD);
	bool LoadGroundObject(std::ifstream& SHMD);
	bool LoadGlobalLight(std::ifstream& SHMD, std::string& GlobalLightPath);
	bool LoadFog(std::ifstream& SHMD);
	bool LoadBackGroundColor(std::ifstream& SHMD);
	bool LoadFrustum(std::ifstream& SHMD);
	bool LoadGlobalObjects(std::ifstream& SHMD, std::vector<std::pair<std::string, std::vector<EditorScene::ObjectPosition>>>& ObjectList);
	bool LoadOneObject(std::ifstream& SHMD, std::vector<ObjectPosition>& Positions);
	bool LoadDirectionLightAmbient(std::ifstream& SHMD);
	bool LoadDirectionLightDiffuse(std::ifstream& SHMD);
	void AttachGroundObj(NiNodePtr& Obj)
	{
		std::lock_guard<std::mutex> lock(WorldLock);		
		kWorld.AttachGroundCollidee(Obj);
	}
	void SaveSHMD();
	void SaveSHMDEntry(std::ofstream&, NiNodePtr, const char*);
	void SaveSHMDLight(std::ofstream&, NiColor, const char*);
	void SaveSHMDFog(std::ofstream&, float, NiColor);
	void SaveSHMDFrustum(std::ofstream&, NiFrustum);
	void SaveSHMDGroundObjects(std::ofstream&, NiNodePtr);
	void AttachGroundObj(NiPickablePtr& Obj)
	{
		NiNodePtr ptr = &*Obj;
		std::lock_guard<std::mutex> lock(WorldLock);
		kWorld.AttachGroundCollidee(ptr);
	}
	void DrawGizmo();
	void DrawSHMDEditor();
	void DrawSHMDHeader(std::string, NiNodePtr);
	NiColor BackgroundColor;

	MapInfo* _Info;
	IniFile _InitFile;

	std::mutex WorldLock;

	NiPickablePtr SelectedObj;
	glm::vec3 SelectedObjAngels;

	
};



