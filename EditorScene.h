#pragma once
#include "FiestaScene.h"
#include "IngameWorld.h"

#include <NiThread.h>
struct HTDPoint
{
	float Height;
};
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
	
	void FixSupTexturing(NiNode* obj);

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

