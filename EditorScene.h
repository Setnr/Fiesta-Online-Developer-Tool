#pragma once
#include "FiestaScene.h"
#include "IngameWorld.h"

#include <NiThread.h>

class IniFile
{
public:
	class Layer
	{
	public:
		Layer() {}

		NiString Name;
		NiString DiffuseFileName;
		NiString BlendFileName;
		float StartPos_X;
		float StartPos_Y;
		float Width;
		float Height;
		float UVScaleDiffuse;
		float UVScaleBlend;
	};
	NiString FileType;
	float Version;
	NiString HeightFileName;
	NiString VertexColorTexture;
	int HeightMap_width;
	int HeightMap_height;
	float OneBlock_width;
	float OneBlock_height;
	int QuadsWide;
	int QuadsHigh;

	std::vector<Layer*> LayerList;

	void Load(FILE* Ini);
	bool LoadLine(FILE* Ini, char* Type, char* DeadChar, char* Info);
	void LoadLayer(FILE* Ini);

	void Print()
	{
		UtilDebugString("HeightFileName: %s LayerCount: %i", HeightFileName, LayerList.size())
			for (int i = 0; i < LayerList.size(); i++)
				UtilDebugString("Name: %s", LayerList.at(i)->Name)
				
	}
};

class EditorScene : public FiestaScene
{
public:
	EditorScene(NiString FilePath, NiString FileName);
	bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea) 
	{
		m_spScene = BaseNode;
		m_spCamerea = Camera;
		return true;
	}
	void Draw(NiRenderer* renderer) 
	{
		Camera->SetViewFrustum(kWorld.GetSkyFrustum());
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		NiDrawScene(Camera, kWorld.GetSkyNode(), m_spCuller);

		Camera->SetViewFrustum(kWorld.GetWorldFrustum());
		NiVisibleArray m_kVisible2;
		NiCullingProcess m_spCuller2(&m_kVisible2);
		NiDrawScene(Camera, kWorld.GetWorldScene(), m_spCuller2);
		return;
	}
	void Update(float fTime)
	{
		kWorld.GetSkyNode()->Update(fTime);
		FiestaScene::Update(fTime);
	}
	NiNode* LoadTerrain();

private:
	bool GetObjectCountLine(FILE* file, char* acFileBuff, char* acTempText,const char* ObjName, int* Counter)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%s %d", acTempText, Counter);
		if (strcmp(acTempText, ObjName))
		{
			return false;
		}
		return true;
	}
	bool LoadObjectNifFile(FILE* file, char* acFileBuff, char* acTempText, char* acFileName)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%s", acTempText);
		sprintf(acFileName, "%s%s", "..\\..\\..\\", acTempText);
		return true;
	}
	bool LoadGlobalMapObject(FILE* file, char* acFileBuff, char* acTempText, const char* ObjName, float* one, float* two, float* three)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%s %f %f %f", acTempText, one, two, three);
		if (strcmp(acTempText, ObjName))
		{
			return false;
		}
		return true;
	}
	bool LoadGlobalMapObject(FILE* file, char* acFileBuff, char* acTempText, const char* ObjName, float* one, float* two, float* three, float* four)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%s %f %f %f %f", acTempText, one, two, three, four);
		if (strcmp(acTempText, ObjName))
		{
			return false;
		}
		return true;
	}
	bool LoadGlobalMapObject(FILE* file, char* acFileBuff, char* acTempText, const char* ObjName, float* one)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%s %f", acTempText, one);
		if (strcmp(acTempText, ObjName))
		{
			return false;
		}
		return true;
	}
	
	bool LoadObjectPositoon(FILE* file, char* acFileBuff, char* acTempText, NiPoint3* kPoint, NiQuaternion* quater, float* scale)
	{
		if (!fgets(acFileBuff, 256, file))
		{
			return false;
		}
		sscanf(acFileBuff, "%f %f %f %f %f %f %f %f", &kPoint->x, &kPoint->y, &kPoint->z, &quater->m_fX, &quater->m_fY, &quater->m_fZ, &quater->m_fW, scale);
		return true;
	}
	World kWorld;
	NiColor BackgroundColor;

	NiThreadProcedure* _procedure;
	NiThread* _Thread;
	NiString _FilePath;
	NiString _FileName;
	IniFile _IniFile;
	NiNodePtr terrain;
};


class EditorSceneBackgroundThread : public NiThreadProcedure
{
public:
	EditorSceneBackgroundThread(EditorScene* editorScene)
	{
		_Scene = editorScene;
	}
	unsigned int ThreadProcedure(void* pvArg)
	{
		_Scene->LoadTerrain();
		return 1;
	}

private:
	EditorScene* _Scene;
};

