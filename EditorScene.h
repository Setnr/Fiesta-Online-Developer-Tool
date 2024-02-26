#pragma once
#include "FiestaScene.h"

#include "IngameWorld.h"
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
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		NiVisibleArray m_kVisible2;
		NiCullingProcess m_spCuller2(&m_kVisible);
		NiCamera* m_spCamera = kWorld.GetCamera();
		NiDrawScene(m_spCamera, kWorld.GetSkyNode(), m_spCuller);
		NiDrawScene(m_spCamera, BaseNode, m_spCuller2);
		return;
		renderer->SetCameraData(m_spCamera);
		m_spCuller.Process(m_spCamera, kWorld.GetWorldScene(), m_spCuller.GetVisibleSet());
		NiDrawVisibleArray(m_spCamera, *m_spCuller.GetVisibleSet());
	}
	
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
		sscanf(acFileBuff, "%f %f %f %f %f %f %f %f", &kPoint->x, &kPoint->y, &kPoint->z, &quater->m_fW, &quater->m_fX, &quater->m_fY, &quater->m_fZ, scale);
		return true;
	}
	World kWorld;
	NiColor BackgroundColor;
	NiCameraPtr Camera;
};

