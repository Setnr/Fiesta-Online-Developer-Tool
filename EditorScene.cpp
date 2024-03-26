#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
#include <NiApplication.h>
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

	kWorld.SetAmbientLightAmbientColor(NiColor(red, green, blue));

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

	_Thread->WaitForCompletion();

	UtilDebugString("Thred Done")
	if(terrain)
	{
		terrain->SetName("terrain");
		kWorld.AttachGroundCollidee(terrain);
	}
	UtilDebugString("After Terrain")


	//NiNodePtr NiN = PgUtil::LoadNifFile("E:\\VM\\Gamebryo\\MemoriaV2TestServer\\Testing2.nif", 0);
	//FixSupTexturing(NiN);
	//kWorld.AttachGroundCollidee(NiN);

	BaseNode->AttachChild(kWorld.GetWorldScene());
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera->Update(0.0f);

	UtilDebugString("Finished Loading World")
	
	NiStream kStream;
	kStream.InsertObject(kWorld.GetWorldScene());
	//kStream.InsertObject(terrain);
	//kStream.InsertObject(Camera);
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
	FILE* Ini = fopen(IniFile, "r");
	if (!Ini)
		return NULL;
	_IniFile.Load(Ini);
	_IniFile.Print();
	char HTDFilePath[256];
	_IniFile.HeightFileName = _IniFile.HeightFileName.GetSubstring(1, _IniFile.HeightFileName.Length() - 1);
	sprintf(HTDFilePath, "%s%s", "..\\..\\..\\", _IniFile.HeightFileName);
	FILE* HTD =fopen(HTDFilePath, "rb");
	if(!HTD)
		UtilDebugString("Failed to open %s", HTDFilePath)
	int PointCounter;
	fread(&PointCounter, sizeof(PointCounter), 1, HTD);
	UtilDebugString("HTDSize = %i", PointCounter)

	HTDPoint* PointArray = new HTDPoint[PointCounter];
	fread(PointArray, sizeof(HTDPoint) * PointCounter, 1, HTD);

	int Width = _IniFile.HeightMap_width - 1;
	int Height = _IniFile.HeightMap_height - 1;
	int BlockX = Width / _IniFile.QuadsWide;
	int BlockY = Height / _IniFile.QuadsHigh;

	NiPoint3** BlockArray = new NiPoint3*[BlockX * BlockY];

	for (int _BlockX = 0; _BlockX < BlockX; _BlockX++) 
	{
		for (int _BlockY = 0; _BlockY < BlockY; _BlockY++)
		{
			int BlockPointsSize = (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1);
			NiPoint3* BlockPoints = NiNew NiPoint3[BlockPointsSize];

			BlockArray[_BlockX + _BlockY * BlockX] = BlockPoints;
			int MaxPointEntry = 0;
			for (int w = 0; w < _IniFile.QuadsWide + 1; w++)
			{
				for (int h = 0; h < _IniFile.QuadsHigh + 1; h++)
				{
					float x = _IniFile.OneBlock_width * w + _IniFile.OneBlock_width * _IniFile.QuadsWide * _BlockX;
					float y = _IniFile.OneBlock_height * h + _IniFile.OneBlock_height * _IniFile.QuadsHigh * _BlockY;

					int XPart = w + _IniFile.QuadsWide * _BlockX;

					int PreBlockLines = (_IniFile.QuadsWide * BlockX + 1) * _IniFile.QuadsHigh * _BlockY;
					int PreFullLines = (_IniFile.QuadsWide * BlockX + 1) * h;
					int YPart = PreBlockLines + PreFullLines;
					int PointOffset = XPart + YPart;

					//UtilDebugString("ID %i mit XPart %i und YPart %i", PointOffset, XPart, YPart)

					float z = PointArray[PointOffset].Height;

					int BlockPointsOffset = w + h * (_IniFile.QuadsWide + 1);
					if (BlockPointsOffset > MaxPointEntry)
						MaxPointEntry = BlockPointsOffset;

					//UtilDebugString("Array Struct %i w: %i h: %i, Wide: %i", BlockPointsOffset, w, h, _IniFile.QuadsWide)

					BlockPoints[BlockPointsOffset] = NiPoint3(x,y,z);
				}
			}
			if(BlockPointsSize - 1 != MaxPointEntry)
			{
				UtilDebugString("BlockPoint Array MissMatch")
				NiMessageBox::DisplayMessage("Load Terrain BlockPoint Array MissMatch", "Error");
			}
		}
	}

	for (int _BlockX = 0; _BlockX < BlockX; _BlockX++)
	{
		for (int _BlockY = 0; _BlockY < BlockY; _BlockY++)
		{
			unsigned short Vertices = (_IniFile.QuadsWide + 1 ) * (_IniFile.QuadsHigh + 1 );
			unsigned short usTriangles = _IniFile.QuadsWide * _IniFile.QuadsHigh * 2;

			NiPoint3* TerrainBlock = BlockArray[_BlockX + _BlockY * BlockX];

			NiPoint3* pkNormal = NiNew NiPoint3[usTriangles];
			NiColorA* pkColor = NiNew NiColorA[usTriangles];

			NiPoint2* pkTexture = NULL;//  NiNew NiPoint2[2 * (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1)];
			//memset(pkTexture, 0xFF, 2 * (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1) * sizeof(NiPoint2));
			unsigned short usNumTextureSets = 0;

			NiGeometryData::DataFlags eNBTMethod = NiGeometryData::DataFlags::NBT_METHOD_NONE;

			unsigned short* pusTriList = (unsigned short* )NiNew NiPoint3 [usTriangles];
			memset(pusTriList, 0, usTriangles * 3 * 4);

			int trict = 0;
			for (int i = 0; i < usTriangles; i++)
			{
				pkNormal[i] = World::ms_kUpDir;
				pkColor[i] = NiColorA(0.50f, 0.50f, 0.50f, 1.0f);
			}
			int j = 0;
			for (int w = 0; w < _IniFile.QuadsWide; w++)
			{
				int i = 0;
				if (j % 2)
				{
					i = 1;
				}
				j++;
				for (int h = 0; h < _IniFile.QuadsHigh; h++)
				{
					unsigned short UL = w + (h * (_IniFile.QuadsWide + 1)) ;      // Upper left.
					unsigned short UR = (w + 1) + (h * (_IniFile.QuadsWide + 1));  // Upper rwght.
					unsigned short BL  = UL + (_IniFile.QuadsWide + 1);          // Bottom left.
					unsigned short BR = UR + (_IniFile.QuadsWide + 1);      // Bottom right.

					if (i % 2) 
					{
						pusTriList[trict] = UL;
						trict++;
						pusTriList[trict] = UR;
						trict++;
						pusTriList[trict] = BR;
						trict++;

						pusTriList[trict] = UL;
						trict++;
						pusTriList[trict] = BL;
						trict++;
						pusTriList[trict] = BR;
						trict++;
					}
					else 
					{
						pusTriList[trict] = BL;
						trict++;
						pusTriList[trict] = UR;
						trict++;
						pusTriList[trict] = UL;
						trict++;

						pusTriList[trict] = UR;
						trict++;
						pusTriList[trict] = BR;
						trict++;
						pusTriList[trict] = BL;
						trict++;
					}
					i++;
				}
			}
			NiTriShapeData* data = NiNew NiTriShapeData(Vertices, TerrainBlock, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod, usTriangles, pusTriList);
			NiTriShape* Shape = NiNew NiTriShape(data);

			NiPixelDataPtr pixldata;
			NiImageConverter* conv = NiImageConverter::GetImageConverter();
			NiPixelData* ReadImage = conv->ReadImageFile("E:\\VM\\Gamebryo\\\MemoriaV2TestServer\\resmap\\fieldtexture\\L1_A.BMP", 0);
			pixldata = NiNew NiPixelData(ReadImage->GetWidth(), ReadImage->GetHeight(), NiPixelFormat::RGBA32,1,1);
			NiTexture::FormatPrefs kPrefs;
			kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::AlphaFormat::ALPHA_DEFAULT;
			kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
			kPrefs.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;

			NiSourceTexturePtr BlendTexture = NiSourceTexture::Create(pixldata, kPrefs);
			NiSourceTexturePtr BaseTexture = NiSourceTexture::Create("E:\\VM\\Gamebryo\\\MemoriaV2TestServer\\resmap\\field\\Rou\\grasst1.dds",kPrefs,true,true);
			
			NiTexturingProperty::ShaderMap* BaseTextureMap = NiNew NiTexturingProperty::ShaderMap(BaseTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
			BaseTextureMap->SetID(0);
			NiTexturingProperty::ShaderMap* BlendTextureMap = NiNew NiTexturingProperty::ShaderMap(BlendTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
			BlendTextureMap->SetID(1);
			NiTexturingProperty* pkTP = NiNew NiTexturingProperty();
			NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();

			alphaprop->SetDestBlendMode(NiAlphaProperty::ALPHA_ZERO);
			
			BaseTextureMap->SetTexture(BaseTexture);
			BlendTextureMap->SetTexture(BlendTexture);

			pkTP->SetShaderMap(0, BaseTextureMap);
			pkTP->SetShaderMap(1, BlendTextureMap);
			NiTexturingProperty::Map* NormalMap = NiNew NiTexturingProperty::Map();
			NormalMap->SetFilterMode(NiTexturingProperty::FILTER_BILERP_MIPNEAREST);
			pkTP->SetBaseMap(NormalMap);
			pkTP->SetApplyMode(NiTexturingProperty::APPLY_DECAL); //APPLY_MODULATE
			pkTP->SetBaseClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
			pkTP->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
			//pkTP->SetBaseTexture(BaseTexture);

			Shape->AttachProperty(pkTP);
			Shape->AttachProperty(alphaprop);
			
			
			NiShaderFactory* ptr = NiShaderFactory::GetInstance();

			if (!ptr)
				MessageBoxA(0, "Shader NiShaderFactory* ptr Error", "Error", 0);
			NiShader* shader = ptr->RetrieveShader("PgTerrain",0);
			if(!shader)
				MessageBoxA(0, "Shader PgTerrain Search Error", "Error", 0);
			Shape->SetShader(shader);
			Shape->StoreNormalBinormalTangent(NiGeometryData::DataFlags::NBT_METHOD_ATI);
			Shape->CreateNormals(true);
			Shape->CalculateNormals();
			
			UtilDebugString("Done Texture");

			Shape->Update(0.0);
			Shape->UpdateEffects();
			Shape->UpdateProperties();
			Shape->Update(0.0);
			if (!terrain)
				terrain = NiNew NiNode;
			terrain->AttachChild(Shape);
		}
	}
}

void EditorScene::FixSupTexturing(NiNode* obj) 
{
	if (obj->GetChildCount() >= 0) 
	{
		for (int i = 0; i < obj->GetChildCount(); i++)
		{
			NiAVObject* AVObj = obj->GetAt(i);
			if (AVObj->IsKindOf(&NiNode::ms_RTTI))
				FixSupTexturing((NiNode*)AVObj);
			else if (AVObj->IsKindOf(&NiTriShape::ms_RTTI)) 
			{
				NiTriShape* shape = (NiTriShape*)AVObj;
				NiTexturingProperty* prop = (NiTexturingProperty*)shape->GetProperty(NiProperty::TEXTURING);
				NiTexturingProperty::ShaderMap* map = prop->GetShaderMap(0);
				NiTexture* text = map->GetTexture();
				if(!text->IsKindOf(&NiSourceTexture::ms_RTTI))
					MessageBoxA(0, "No SourceTexture", "Error", 0);
				else
				{
					NiSourceTexture* stext = (NiSourceTexture*)text;
					MessageBoxA(0, stext->GetFilename(), "Info", 0);
					NiTexture::FormatPrefs kPrefs;
					kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::AlphaFormat::ALPHA_DEFAULT;
					kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
					kPrefs.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
					NiSourceTexturePtr BaseTexture = NiSourceTexture::Create("E:\\VM\\Gamebryo\\\MemoriaV2TestServer\\resmap\\field\\Rou\\grasst1.dds", kPrefs, true, true);
					NiTexturingProperty* NewProp = NiNew NiTexturingProperty();
					NewProp->SetBaseTexture(BaseTexture);
					shape->DetachProperty(prop);
					shape->AttachProperty(NewProp);
				}
				//NiTexturingProperty* NewProp = NiNew NiTexturingProperty;
				//NewProp->SetBaseTexture(text);
				//NiDelete prop;
			}
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