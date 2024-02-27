#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
EditorScene::EditorScene(NiString FilePath, NiString FileName) 
{
	if (!kWorld.InitScene())
		return;
	if (!kWorld.InitCamera())
		return;
	if (!kWorld.InitSkyCtrl())
		return;
	if (!kWorld.InitLightFog())
		return;
	if (!kWorld.InitShadow())
		return;

#define EditorSceneError(Msg) {NiMessageBox::DisplayMessage(Msg, "Error"); return;}
	FILE* file = fopen(FilePath, "r");
	if (!file)
	{
		EditorSceneError("Failed to open SHMDFile")
	}
	char acFileBuff[256];
	char acFileName[256];
	char acTempText[256];
	if (!fgets(acFileBuff, 256, file))
	{
		EditorSceneError("Failed to Read SHMDFile")
	}
	sscanf(acFileBuff, "%s", acTempText);
	if (strcmp(acTempText, "shmd0_5"))
	{
		EditorSceneError("Wrong Version of SHMDFile")
	}
	int Counter;
	if (!GetObjectCountLine(file, acFileBuff, acTempText, "Sky", &Counter))
	{
		EditorSceneError("Failed Loading Sky Count")
	}
	if (Counter)
	{
		if(LoadObjectNifFile(file, acFileBuff, acTempText, acFileName))
		{
			char buf[1024];
			GetCurrentDirectoryA(sizeof(buf), buf);
			NiNodePtr sky;
			PgUtil::LoadNodeNifFile(acFileName, &sky, NULL);
			kWorld.AttachSky(sky);
		}
		else 
		{
			EditorSceneError("Failed Loading Sky Path")
		}
	}
	if (!GetObjectCountLine(file, acFileBuff, acTempText, "Water", &Counter))
	{
		EditorSceneError("Failed Loading Water Count")
	}
	if (Counter) 
	{
		for (int i = 0; i < Counter; i++) 
		{
			if (LoadObjectNifFile(file, acFileBuff, acTempText, acFileName))
			{
				NiNodePtr water;
				PgUtil::LoadNodeNifFile(acFileName, &water, NULL);
				kWorld.AttachWater(water);
			}
			else
			{
				EditorSceneError("Failed Loading Water Path")
			}
		}
	}
	if (!GetObjectCountLine(file, acFileBuff, acTempText, "GroundObject", &Counter))
	{
		EditorSceneError("Failed Loading GroundObject Count")
	}
	if (Counter)
	{
		for (int i = 0; i < Counter; i++)
		{
			if (LoadObjectNifFile(file, acFileBuff, acTempText, acFileName))
			{
				NiNodePtr groundobject;
				PgUtil::LoadNodeNifFile(acFileName, &groundobject, NULL);
				kWorld.AttachGroundObj(groundobject);
			}
			else
			{
				EditorSceneError("Failed Loading GroundObject Path")
			}
		}
	}
	float depth;
	float red;
	float green;
	float blue;
	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "GlobalLight", &red, &green, &blue)) 
	{
		EditorSceneError("Failed Loading GlobalLight")
	}
	//NiAmbientLightPtr MapLight = NiNew NiAmbientLight;
	//MapLight->SetDiffuseColor(NiColor(red, green, blue));

	kWorld.SetAmbientLightAmbientColor(NiColor(red, green, blue));
	//kWorld.ClearGlobalLightMap();
	//kWorld.AddGlobalLight(MapLight);
	// 
	// 
	//World::LoadLightInfoNode(v2, v20);
	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "Fog", &depth, &red, &green, &blue)) 
	{
		EditorSceneError("Failed Loading Fog")
	}

	kWorld.SetFogColor(NiColor(red,green,blue));
	kWorld.SetFogDepth(depth);

	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "BackGroundColor", &red, &green, &blue))
	{
		EditorSceneError("Failed Loading BackGroundColor")
	}
	BackgroundColor.r = red;
	BackgroundColor.g = green;
	BackgroundColor.b = blue;

	float Frustum;
	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "Frustum", &Frustum))
	{
		EditorSceneError("Failed Loading Frustum")
	}
	kWorld.SetFarFrumstum(Frustum); // MachineOpt::SetViewTerrainLength
	while (true) 
	{
		if (!fgets(acFileBuff, 256, file))
		{
			EditorSceneError("Failed Loading ObjectDataStream")
		}
		sscanf(acFileBuff, "%s", acTempText);
		if (!strcmp(acTempText, "DataObjectLoadingEnd"))
		{
			UtilDebugString("Found End of Data with DataObjectLoadingEnd")
			break;
		}
		sscanf(acFileBuff, "%s %d", acTempText, &Counter);
		sprintf(acFileName, "%s%s", "..\\..\\..\\", acTempText);
		NiNodePtr obj;
		PgUtil::LoadNodeNifFile(acFileName, &obj, NULL);
		if(!obj)
			UtilDebugString("Failed to Load Map Data Object %s ", acFileName)
		NiPoint3 point;
		NiQuaternion quater;
		float Scale;
		for (int i = 0; i < Counter; i++) 
		{
			if (!LoadObjectPositoon(file, acFileBuff, acTempText, &point, &quater, &Scale)) 
			{
				EditorSceneError("Failed Loading Object Positon")
			}

			NiNode* AddingObj = NiNew NiNode;
			AddingObj->AttachChild(obj);
			AddingObj->SetTranslate(point);
			AddingObj->SetRotate(quater);
			AddingObj->SetScale(Scale);
			kWorld.AttachGroundCollidee(AddingObj);
			
		}
	}
	
	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "DirectionLightAmbient", &red, &green, &blue))
	{
		EditorSceneError("Failed Loading DirectionLightAmbient")
	}
	kWorld.SetMapDirectionalLightAmbientColor(NiColor(red,green,blue));

	if (!LoadGlobalMapObject(file, acFileBuff, acTempText, "DirectionLightDiffuse", &red, &green, &blue))
	{
		EditorSceneError("Failed Loading DirectionLightDiffuse")
	}
	kWorld.SetMapDirectionalLightDiffuseColor(NiColor(red, green, blue));
	
	Camera = kWorld.GetCamera();
	Camera->SetTranslate(NiPoint3(5576, 5768, 2500));
	Pitch = 1.57f * 2.0f;
	Yaw = -1.57f;
	Roll = 1.57f;
	NiMatrix3 rotation;
	rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

	Camera->SetRotate(rotation);
	//Camera->LookAtWorldPoint(NiPoint3(5576.985840, 5768.680176, 743.123779), World::ms_kUpDir);

	BaseNode->AttachChild(kWorld.GetWorldScene());
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera->Update(0.0f);

	UtilDebugString("Finished Loading World")
	

	NiStream kStream;
	kStream.InsertObject(kWorld.GetSkyNode());
	kStream.InsertObject(Camera);
	kStream.Save("./TESTING.nif");

	return;
}