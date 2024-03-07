#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
EditorScene::EditorScene(NiString FilePath, NiString FileName) 
{

	_FilePath = FilePath;
	_FileName = FileName;

	_procedure = NiNew EditorSceneBackgroundThread(this);
	_Thread = NiThread::Create(_procedure);
	NIASSERT(_Thread)
	_Thread->SetPriority(NiThread::NORMAL);
	_Thread->Resume();;


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
	kWorld.SetFarFrumstum(Frustum);
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


	_Thread->WaitForCompletion();

	UtilDebugString("Thred Done")
	if(terrain)
	{
		terrain->SetName("terrain");
		kWorld.GetWorldScene()->AttachChild(terrain);
	}
	UtilDebugString("After Terrain")


	BaseNode->AttachChild(kWorld.GetWorldScene());
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera->Update(0.0f);

	UtilDebugString("Finished Loading World")
	

	NiStream kStream;
	kStream.InsertObject(kWorld.GetWorldScene());
	kStream.InsertObject(Camera);
	kStream.Save("./TESTING.nif");

	return;
}

NiNode* EditorScene::LoadTerrain()
{
	terrain = NULL;
	char RowBuffer[256];
	NiString CleanPath = _FilePath.GetSubstring(0, ".");
	NiString IniFile = CleanPath;
	IniFile.Insert(".ini", CleanPath.Length());
	UtilDebugString("IniFile: %s", IniFile)
	UtilDebugString("IniFile: %s", IniFile)
	FILE* Ini = fopen(IniFile, "r");
	if (!Ini)
		return NULL;
	UtilDebugString("IniFile start load")
	_IniFile.Load(Ini);
	_IniFile.Print();
	char HTDFilePath[256];
	_IniFile.HeightFileName = _IniFile.HeightFileName.GetSubstring(1, _IniFile.HeightFileName.Length() - 1);
	UtilDebugString("Reduced Path to %s", _IniFile.HeightFileName)
	sprintf(HTDFilePath, "%s%s", "..\\..\\..\\", _IniFile.HeightFileName);
	FILE* HTD =fopen(HTDFilePath, "rb");
	if(!HTD)
		UtilDebugString("Failed to open %s", HTDFilePath)
	int PointCounter;
	fread(&PointCounter, sizeof(PointCounter), 1, HTD);
	UtilDebugString("HTDSize = %i", PointCounter)
	struct HTDPoint 
	{
		float Height;
	};
	HTDPoint* PointArray = new HTDPoint[PointCounter];
	fread(PointArray, sizeof(HTDPoint) * PointCounter, 1, HTD);
	int Width = _IniFile.HeightMap_width - 1;
	int Height = _IniFile.HeightMap_height - 1;
	int BlockX = Width / _IniFile.QuadsWide;
	int BlockY = Height / _IniFile.QuadsHigh;

	NiSourceTexturePtr texture = NiSourceTexture::Create(NiFixedString(_IniFile.LayerList[0]->BlendFileName));
	
	NiPoint3** BlockArray = new NiPoint3*[BlockX * BlockY];
	for (int _BlockX = 0; _BlockX < BlockX; _BlockX++) 
	{
		for (int _BlockY = 0; _BlockY < BlockY; _BlockY++)
		{
			NiPoint3* BlockPoints = NiNew NiPoint3[(_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh  + 1)];
			BlockArray[_BlockX + _BlockY * BlockX] = BlockPoints;
			for (int w = 0; w < _IniFile.QuadsWide + 1; w++)
			{
				for (int h = 0; h < _IniFile.QuadsHigh + 1; h++)
				{
					/*
					* TODO Fix Points
					*/
					BlockPoints[w + h * _IniFile.QuadsWide] = NiPoint3(_IniFile.OneBlock_width * w, _IniFile.OneBlock_height * h, PointArray[w + _BlockX * _IniFile.QuadsWide + h * _IniFile.QuadsWide * BlockY].Height );
					

				}
			}
		}
	}

	for (int _BlockX = 0; _BlockX < BlockX; _BlockX++)
	{
		for (int _BlockY = 0; _BlockY < BlockY; _BlockY++)
		{
			unsigned short Vertices = (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1);
			NiPoint3* TerrainBlock = BlockArray[_BlockX + _BlockY * BlockX];
			NiPoint3* pkNormal = NiNew NiPoint3[Vertices * 2];
			NiColorA* pkColor = NiNew NiColorA[Vertices * 2];
			NiPoint2* pkTexture = 0;
			unsigned short usNumTextureSets = 1;
			NiGeometryData::DataFlags eNBTMethod = NiGeometryData::DataFlags::NBT_METHOD_NONE;
			unsigned short usTriangles = (_IniFile.QuadsHigh + 1) * (_IniFile.QuadsWide + 1) * 2;
			unsigned short* pusTriList = (unsigned short* )NiNew NiPoint3 [usTriangles];
			memset(pusTriList, 0, usTriangles * 3 * 4);
			int trict = 0;
			int PosCount = 0;
			for (int h = 0; h < _IniFile.QuadsHigh; h++)
			{
				for (int w = 0; w < _IniFile.QuadsWide; w++)
				{
					unsigned short UL = w + (h * (_IniFile.QuadsWide + 1)) ;      // Upper left.
					unsigned short UR = (w + 1) + (h * (_IniFile.QuadsWide + 1));  // Upper rwght.
					unsigned short BL  = UL + (_IniFile.QuadsWide + 1);          // Bottom left.
					unsigned short BR = UR + (_IniFile.QuadsWide + 1);      // Bottom right.

					pusTriList[trict] = UR;
					trict++;
					pusTriList[trict] = BL;
					trict++;
					pusTriList[trict] = UL;
					trict++;

					//UtilDebugString("Top Triangle %i %i %i", pusTriList[trict - 3], pusTriList[trict - 2], pusTriList[trict - 1])

					pusTriList[trict] = BR;
					trict++;
					pusTriList[trict] = UR;
					trict++;
					pusTriList[trict] = BL;
					trict++;

					//UtilDebugString("Bot Triangle %i %i %i", pusTriList[trict - 3], pusTriList[trict - 2], pusTriList[trict - 1])
					
					/*
					*	0	|	1	|	2 
					* 	3	|	4	|	5
					* 	6	|	7	|	8
					* 013
					* 134
					* 124
					* 245
					* 346
					* 467
					* 457
					* 578
					* 
					* 
					* 
					* 	0	|	1	|	2   ... | 64
					* 	65	|	66	|	67  ... | 128
					* 	...	|	...	|	... ... | ...
					* 	4031|	4032|	4033... | 4095
					*/
					pkNormal[PosCount] = World::ms_kUpDir;
					pkColor[PosCount] = NiColorA(0.0f, 1.0f, 0.0f, 1.0f);
					PosCount++;

					pkNormal[PosCount] = World::ms_kUpDir;
					pkColor[PosCount] = NiColorA(0.0f, 1.0f, 0.0f, 1.0f);
					PosCount++;
				}
			}
			NiTriShapeData* data = NiNew NiTriShapeData(Vertices, TerrainBlock, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod, usTriangles, pusTriList);
			NiTriShape* Shape = NiNew NiTriShape(data);
			char TexturePath[256];
			sprintf(TexturePath, "%s%s", "..\\..\\..\\", _IniFile.LayerList[0]->DiffuseFileName);
			UtilDebugString("TexturePath %s", TexturePath)
			NiTexturingPropertyPtr prop = NiNew NiTexturingProperty("E:\\VM\\Gamebryo\\MemoriaV2TestServer\\resmap\\field\\Rou\\grasst1.dds");
			Shape->AttachProperty(prop);
			Shape->Update(0.0);
			if (!terrain)
				terrain = NiNew NiNode;
			terrain->AttachChild(Shape);
		}
	}
}

void IniFile::Load(FILE* Ini) 
{
	char Type[256];
	char DeadChar[256];
	char Info[256];
	NiString Prev = "";
	int ct = 0;
	while (true) 
	{
		if (LoadLine(Ini, Type, DeadChar, Info))
			ct++;
		if (Prev == Type) {
			ct--;
			continue;
		}
		Prev = Type;
		UtilDebugString("Type: %s, DeadChar: %s, Info: %s" , Type,DeadChar,Info)
		if (NiString(Type) == "#PGFILE")
			FileType = Info;
		if (NiString(Type) == "#HeightFileName")
			HeightFileName = Info;
		if (NiString(Type) == "#VerTexColorTexture")
			VertexColorTexture = Info;


		if (NiString(Type) == "#FILE_VER")
			Version = atof(Info);
		if (NiString(Type) == "#OneBlockWidth")
			OneBlock_width = atof(Info);
		if (NiString(Type) == "#OneBlockHeight")
			OneBlock_height = atof(Info);


		if (NiString(Type) == "#HEIGHTMAP_WIDTH")
			HeightMap_width = atoi(Info);
		if (NiString(Type) == "#HEIGHTMAP_HEIGHT")
			HeightMap_height = atoi(Info);
		if (NiString(Type) == "#QuadsWide")
			QuadsWide = atoi(Info);
		if (NiString(Type) == "#QuadsHigh")
			QuadsHigh = atoi(Info);

		if (ct > 9)
			break;
	}

	while (true) 
	{
		if (LoadLine(Ini, Type, DeadChar, Info)) 
		{
			if (NiString(Type) == "#END_FILE")
				break;
			if (NiString(Type) == "#Layer")
				LoadLayer(Ini);
		}
	}
}
bool IniFile::LoadLine(FILE* Ini, char* Type, char* DeadChar, char* Info)
{
	char buffer[1024];
	if (!fgets(buffer, 1024, Ini))
		return false;
	if (!strcmp(buffer, "")) 
	{
		return false;
	}
	sscanf(buffer, "%s %s %s", Type, DeadChar, Info);
	return true;
}
void IniFile::LoadLayer(FILE* Ini) 
{
	char Type[256];
	char DeadChar[256];
	char Info[256];
	int ct = 0;
	Layer* _Layer = new Layer;;
	while (true)
	{
		if (LoadLine(Ini, Type, DeadChar, Info))
			ct++;

		if (NiString(Type) == "#Name")
			_Layer->Name = Info;
		if (NiString(Type) == "#DiffuseFileName")
			_Layer->DiffuseFileName = Info;
		if (NiString(Type) == "#BlendFileName")
			_Layer->BlendFileName = Info;


		if (NiString(Type) == "#StartPos_X")
			_Layer->StartPos_X = atof(Info);
		if (NiString(Type) == "#StartPos_Y")
			_Layer->StartPos_Y = atof(Info);
		if (NiString(Type) == "#Width")
			_Layer->Width = atof(Info);

		if (NiString(Type) == "#Height")
			_Layer->Height = atof(Info);
		if (NiString(Type) == "#UVScaleDiffuse")
			_Layer->UVScaleDiffuse = atof(Info);
		if (NiString(Type) == "#UVScaleBlend")
			_Layer->UVScaleBlend = atof(Info);

		if (ct > 8)
			break;
	}
	LayerList.push_back(_Layer);
}