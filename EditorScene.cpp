#include "EditorScene.h"
#include <NiMain.h>
#include "PgUtil.h"
#include <NiApplication.h>
#include <NiD3DShaderFactory.h>
#include <iostream>
#include <fstream>
EditorScene::EditorScene(NiString FilePath, NiString FileName) 
{

	_FilePath = FilePath;
	_FileName = FileName;

	_procedure = NiNew EditorSceneBackgroundThread(this);
	_Thread = NiThread::Create(_procedure);
	NIASSERT(_Thread)
	_Thread->SetPriority(NiThread::NORMAL);
	_Thread->Resume();


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
	std::ifstream SHMD;
	SHMD.open(FilePath, std::ios::in);

	if (!SHMD.is_open()) 
	{
		EditorSceneError("Failed to open SHMDFile");
	}

	if (!CheckSHMDVersion(SHMD))
		EditorSceneError("SHMD has wrong FileFormat");

	if (!LoadSky(SHMD))
		EditorSceneError("SHMD - Failed to Load Sky");
	if (!LoadWater(SHMD))
		EditorSceneError("SHMD - Failed to Load Water");
	if (!LoadGroundObject(SHMD))
		EditorSceneError("SHMD - Failed to Load GroundObject");
	if(!LoadGlobalLight(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalLight");
	if (!LoadFog(SHMD))
		EditorSceneError("SHMD - Failed to Load Fog");
	if (!LoadBackGroundColor(SHMD))
		EditorSceneError("SHMD - Failed to Load BackGroundColor");
	if (!LoadFrustum(SHMD))
		EditorSceneError("SHMD - Failed to Load Frustum");
	if(!LoadGlobalObjects(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightAmbient(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightDiffuse(SHMD))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");

	Camera = kWorld.GetCamera();
	Camera->SetTranslate(NiPoint3(5576, 5768, 2500));
	Pitch = 1.57f * 2.0f;
	Yaw = -1.57f;
	Roll = 1.57f;
	NiMatrix3 rotation;
	rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

	Camera->SetRotate(rotation);

	_Thread->WaitForCompletion();

	if(TerrainParent)
	{
		terrain->SetName("terrain");
		kWorld.AttachGroundTerrain(TerrainParent);
	}

	//NiNodePtr NiN;
	//NiString CleanPath = _FilePath.GetSubstring(0, ".");
	//NiString IniFile = CleanPath;
	//IniFile.Insert(".sdt", CleanPath.Length());
	//PgUtil::LoadTerrainNif(IniFile,&NiN, 0);
	//FixSupTexturing(NiN);
	//kWorld.AttachGroundTerrain(NiN);

	BaseNode->AttachChild(kWorld.GetWorldScene());
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);

	Camera->Update(0.0f);


	NiStream kStream2;
	kStream2.InsertObject(kWorld.GetTerrainScene());
	kStream2.Save("./TESTING123.nif");	

	//NiStream kStream;
	//kStream.InsertObject(NiN);
	//kStream.Save("./STDNIF.nif");
	return;
}

NiNode* EditorScene::LoadTerrain()
{
	terrain = NULL;
	TerrainParent = NULL;
	char RowBuffer[513];
	NiString CleanPath = _FilePath.GetSubstring(0, ".");
	NiString IniFile = CleanPath;
	IniFile.Insert(".ini", CleanPath.Length());
	FILE* Ini = fopen(IniFile, "r");
	if (!Ini)
		return NULL;
	_IniFile.Load(Ini);
	_IniFile.Print();

	_IniFile.HeightFileName = _IniFile.HeightFileName.substr(1, _IniFile.HeightFileName.length() - 2);
	std::string HTDFilePath = PgUtil::CreateFullFilePathFromBaseFolder(_IniFile.HeightFileName);
	FILE* HTD =fopen(HTDFilePath.c_str(), "rb");
	if (!HTD)
		return NULL;
	int PointCounter;
	fread(&PointCounter, sizeof(PointCounter), 1, HTD);

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
				NiMessageBox::DisplayMessage("Load Terrain BlockPoint Array MissMatch", "Error");
			}
		}
	}

	for (int _BlockX = 0; _BlockX < BlockX; _BlockX++)
	{
		for (int _BlockY = 0; _BlockY < BlockY; _BlockY++)
		{
			for (int _Layer = 0; _Layer < _IniFile.LayerList.size(); _Layer++)
			{
				IniFile::Layer::RGBColor SplitColor(239);

				IniFile::Layer* CurrentLayer = _IniFile.LayerList.at(_Layer);
				NiPixelData* LayerData = CurrentLayer->pixldata;
				if (LayerData->GetPixelFormat() != NiPixelFormat::RGBA32)
				{
					NiMessageBox::DisplayMessage("NiPixelData From Layer Has Wrong NiPixelFormat", "Error");
				}

				IniFile::Layer::RGBAColor* ColorArray = (IniFile::Layer::RGBAColor*)LayerData->GetPixels();
				if (LayerData->GetSizeInBytes() < (_IniFile.HeightMap_height - 1) * (_IniFile.HeightMap_width - 1) * sizeof(IniFile::Layer::RGBAColor))
				{
					NiMessageBox::DisplayMessage("NiPixelData From Layer Has Wrong Size", "Error");
				}
				int PixelCounter = 0;
				for (int x = 0; x < _IniFile.QuadsWide; x++)
				{
					for (int y = 0; y < _IniFile.QuadsHigh; y++)
					{
						int PreBlockXPart = _BlockX * _IniFile.QuadsWide;
						int CurrentBlockXPart = x;
						int XPart = PreBlockXPart + CurrentBlockXPart;

						int PreBlockYPart = _BlockY * _IniFile.QuadsHigh * LayerData->GetWidth();
						int CurrentBlockYPart = LayerData->GetWidth() * y;
						int YPart = PreBlockYPart + CurrentBlockYPart;

						int PointOffset = XPart + YPart;
						if (ColorArray[PointOffset] > SplitColor)
						{
							PixelCounter++;
						}
					}
				}

				if (PixelCounter < 1)
					continue;



				unsigned short Vertices = (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1);
				unsigned short usTriangles = PixelCounter * 2;

				NiPoint3* TerrainBlock = BlockArray[_BlockX + _BlockY * BlockX];

				NiPoint3* pkNormal = NiNew NiPoint3[Vertices];
				NiColorA* pkColor = NiNew NiColorA[Vertices];

				NiPoint2* pkTexture = NiNew NiPoint2[Vertices * 2];//  NiNew NiPoint2[2 * (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1)];
				unsigned int TextureCounter = 0;
				for (int h = 0; h < _IniFile.QuadsHigh + 1; h++)
				{
					for (int w = 0; w < _IniFile.QuadsWide + 1; w++)
					{
						pkTexture[TextureCounter] = NiPoint2(w * (16.0f/ 64.0f) + _BlockX * 16.0f, h * 0.25f + _BlockY * (16.0f / 64.0f)); //16.0f
						pkTexture[Vertices + TextureCounter] = NiPoint2(w *( 0.25f / 64.0f) + _BlockX * 0.25f, h * (0.25f / 64.0f) + _BlockY * 0.25f);
						TextureCounter++;
					}
				}
				//memset(pkTexture, 0xFF, 2 * (_IniFile.QuadsWide + 1) * (_IniFile.QuadsHigh + 1) * sizeof(NiPoint2));
				unsigned short usNumTextureSets = 2;

				NiGeometryData::DataFlags eNBTMethod = NiGeometryData::DataFlags::NBT_METHOD_NONE;

				unsigned short* pusTriList = (unsigned short*)NiNew NiPoint3[usTriangles];
				memset(pusTriList, 0, usTriangles * 3 * 4);

				int trict = 0;
				for (int i = 0; i < Vertices; i++)
				{
					pkNormal[i] = World::ms_kUpDir;
					pkColor[i] = NiColorA(1.00f, 1.00f, 1.00f, 1.0f);
				}
				for (int w = 0; w < _IniFile.QuadsWide; w++)
				{
					for (int h = 0; h < _IniFile.QuadsHigh; h++)
					{
						int PreBlockXPart = _BlockX * _IniFile.QuadsWide;
						int CurrentBlockXPart = w;
						int XPart = PreBlockXPart + CurrentBlockXPart;

						int PreBlockYPart = _BlockY * _IniFile.QuadsHigh * LayerData->GetWidth();
						int CurrentBlockYPart = LayerData->GetWidth() * h;
						int YPart = PreBlockYPart + CurrentBlockYPart;

						int PointOffset = XPart + YPart;

						if (ColorArray[PointOffset] > SplitColor)
						{

							unsigned short BottomLeft = w + (h * (_IniFile.QuadsWide + 1));      // Upper left.
							unsigned short BottomRight = (w + 1) + (h * (_IniFile.QuadsWide + 1));  // Upper rwght.
							unsigned short TopLeft = BottomLeft + (_IniFile.QuadsWide + 1);          // Bottom left.
							unsigned short TopRight = BottomRight + (_IniFile.QuadsWide + 1);      // Bottom right.


							pusTriList[trict] = BottomLeft;
							trict++;

							pusTriList[trict] = BottomRight;
							trict++;
							pusTriList[trict] = TopLeft;
							trict++;

							pusTriList[trict] = TopLeft;
							trict++;
							pusTriList[trict] = BottomRight;
							trict++;
							pusTriList[trict] = TopRight;
							trict++;

						}
					}
				}
				NiSourceTexturePtr BaseTexture = CurrentLayer->BaseTexture;
				NiSourceTexturePtr BlendTexture = CurrentLayer->BlendTexture;

				NiTriShapeData* data = NiNew NiTriShapeData(Vertices, TerrainBlock, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod, usTriangles, pusTriList);
				NiTriShape* Shape = NiNew NiTriShape(data);
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
				NiTexturingProperty::Map* NormalMap = NiNew NiTexturingProperty::Map(NULL,0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP_MIPNEAREST,0);
				
				NormalMap->SetFilterMode(NiTexturingProperty::FILTER_BILERP_MIPNEAREST);
				NormalMap->SetClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
				NormalMap->SetTexture(NULL);

				pkTP->SetBaseClampMode(NiTexturingProperty::WRAP_S_WRAP_T);

				pkTP->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
				pkTP->SetBaseMap(NormalMap);
				Shape->AttachProperty(pkTP);
				Shape->AttachProperty(alphaprop);

				Shape->CalculateNormals();
				
				Shape->ApplyAndSetActiveMaterial("PgTerrain");

				Shape->Update(0.0);
				Shape->UpdateEffects();
				Shape->UpdateProperties();
				Shape->Update(0.0);
				if (!terrain)
				{
					terrain = NiNew NiNode;
					TerrainParent = NiNew NiSortAdjustNode;
					NiAlphaProperty* alphapro = NiNew NiAlphaProperty();
					TerrainParent->AttachChild(terrain);
					TerrainParent->AttachProperty(alphapro);
				}
				terrain->AttachChild(Shape);
			}
		}
	}
	return NULL;
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
				NiTriShape* Shape = (NiTriShape*)AVObj;

				NiTexturingProperty* prop = (NiTexturingProperty*)Shape->GetProperty(NiProperty::TEXTURING);
				Shape->DetachAllProperties();

				NiTexture* BaseTexture = prop->GetShaderMap(0)->GetTexture();
				NiTexture* BlendTexture = prop->GetShaderMap(1)->GetTexture();

				std::map<NiTexture*, NiTexturingProperty::ShaderMap*>::iterator BaseTextureIterator = BaseTextureSafeMap.find(BaseTexture);
				std::map<NiTexture*, NiTexturingProperty::ShaderMap*>::iterator BlendTextureIterator = BlendTextureSafeMap.find(BlendTexture);

				NiTexturingProperty::ShaderMap* BaseTextureMap;
				NiTexturingProperty::ShaderMap* BlendTextureMap;

				if (BaseTextureIterator != BaseTextureSafeMap.end())
				{
					BaseTextureMap = BaseTextureIterator->second;
				}
				else 
				{
					BaseTextureMap = NiNew NiTexturingProperty::ShaderMap(BaseTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
					BaseTextureMap->SetID(0);
					BaseTextureMap->SetTexture(BaseTexture);
					BaseTextureSafeMap.insert(std::pair<NiTexture*, NiTexturingProperty::ShaderMap*>(BaseTexture, BaseTextureMap));
					
				}

				if (BlendTextureIterator != BlendTextureSafeMap.end())
				{
					BlendTextureMap = BaseTextureIterator->second;
				}
				else
				{
					if(BlendTextureSafeMap.size() != 7)
					{
						BlendTextureMap = NiNew NiTexturingProperty::ShaderMap(BlendTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T, NiTexturingProperty::FILTER_BILERP, 0);
						BlendTextureMap->SetID(1);
						BlendTextureMap->SetTexture(BlendTexture);
						BlendTextureSafeMap.insert(std::pair<NiTexture*, NiTexturingProperty::ShaderMap*>(BlendTexture, BlendTextureMap));
					}
					else
						BlendTextureMap = NULL;

				}


				NiTexturingProperty* pkTP = NiNew NiTexturingProperty();

				pkTP->SetShaderMap(0, BaseTextureMap);
				pkTP->SetShaderMap(1, BlendTextureMap);

				NiTexturingProperty::Map* NormalMap = NiNew NiTexturingProperty::Map();
				NormalMap->SetFilterMode(NiTexturingProperty::FILTER_BILERP_MIPNEAREST);
				NormalMap->SetClampMode(NiTexturingProperty::WRAP_S_WRAP_T);
				NormalMap->SetTexture(NULL);
				NormalMap->SetTextureTransform(NULL);
				pkTP->SetBaseMap(NormalMap);
				pkTP->SetBaseClampMode(NiTexturingProperty::WRAP_S_WRAP_T);

				pkTP->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);

				Shape->AttachProperty(pkTP);

				NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
				alphaprop->SetDestBlendMode(NiAlphaProperty::ALPHA_ZERO);
				Shape->AttachProperty(alphaprop);

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
		memset(Type, 0, sizeof(Type));
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
	char InfoFixer[256];
	char FileNamePathFixer[256];
	Layer* _Layer = new Layer;;
	while (true)
	{
		LoadLine(Ini, Type, InfoFixer, DeadChar);

		if (NiString(Type) == "{")
			continue;
		if (NiString(Type) == "}")
			break;
		strcpy(Info, &InfoFixer[1]);
		if (NiString(Type) == "#Name")
		{
			_Layer->Name = Info;
		}
		if (NiString(Type) == "#DiffuseFileName")
		{
			strcpy(FileNamePathFixer, &Info[1]);
			FileNamePathFixer[strlen(FileNamePathFixer) - 1] = 0x0;
			_Layer->DiffuseFileName = FileNamePathFixer;
		}
		if (NiString(Type) == "#BlendFileName")
		{
			strcpy(FileNamePathFixer, &Info[1]);
			FileNamePathFixer[strlen(FileNamePathFixer) - 1] = 0x0;
			_Layer->BlendFileName = FileNamePathFixer;
		}


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

	}
	_Layer->CreateTexture();
	LayerList.push_back(_Layer);


}