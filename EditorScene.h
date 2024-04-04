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
		struct RGBColor {
		public:
			RGBColor(unsigned char _r, unsigned char _g, unsigned char _b) {
				r = _r;
				g = _g;
				b = _b;
			}
			RGBColor(unsigned char c) {
				r = c;
				g = c;
				b = c;
			}
			bool operator==(const RGBColor& c)
			{
				return c.b == b && c.g == g && c.r == r;
			}
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};
		struct RGBAColor {
		public:
			RGBAColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
				r = _r;
				g = _g;
				b = _b;
				a = _a;
			}
			RGBAColor(RGBColor c) {
				r = c.r;
				g = c.g;
				b = c.b;
				a = 0xFF;
			}
			RGBAColor(unsigned char c) {
				r = c;
				g = c;
				b = c;
				a = c;
			}
			bool operator==(const RGBAColor& c)
			{
				return c.a == a && c.b == b && c.g == g && c.r == r;
			}
			bool operator!=(const RGBAColor& c)
			{
				return c.a != a || c.b != b || c.g != g || c.r != r;
			}
			bool operator>(const RGBColor& c)
			{
				return r > c.r && g > c.g && b > c.b;
			}
			bool operator==(const RGBColor& c)
			{
				return c.b == b && c.g == g && c.r == r;
			}
			bool operator!=(const RGBColor& c)
			{
				return c.b != b || c.g != g || c.r != r;
			}
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};

		Layer() {}
		void CreateTexture() 
		{
			NiImageConverter* conv = NiImageConverter::GetImageConverter();

			char acFileName[513];
			PgUtil::CreateFullFilePathFromBaseFolder(acFileName, BlendFileName);
			UtilDebugString("BlendFileName for Texture %s", acFileName);

			NiPixelData* ReadImage = conv->ReadImageFile(acFileName, 0);
			if (ReadImage->GetPixelFormat() != NiPixelFormat::RGB24)
			{
				UtilDebugString("BlendFileName for Texture %s", acFileName)
				UtilDebugString("NiPixelFormat %i", ReadImage->GetPixelFormat())
				NiMessageBox::DisplayMessage("NiPixelData From ReadImage in Layer::CreateTexture Has Wrong NiPixelFormat", "Error");
			}
			
			pixldata = NiNew NiPixelData(ReadImage->GetWidth(), ReadImage->GetHeight(), NiPixelFormat::RGBA32);
			RGBAColor* PixelColorA = (RGBAColor*)pixldata->GetPixels();
			RGBColor* PixelColor = (RGBColor*)ReadImage->GetPixels();
			if (ReadImage->GetSizeInBytes() / sizeof(RGBColor) != pixldata->GetSizeInBytes() / sizeof(RGBAColor))
			{
				UtilDebugString("ReadImage Pixels %i | WriteImage Pixels %i", ReadImage->GetSizeInBytes() / sizeof(RGBColor), pixldata->GetSizeInBytes() / sizeof(RGBAColor))
				NiMessageBox::DisplayMessage("Size MissMatch in Recreating NiPixelData for TerrainTexture", "Error");
			}

			for (int w = 0; w < ReadImage->GetWidth(); w++)
			{
				for (int h = 0; h < ReadImage->GetHeight(); h++)
				{ 
					int XPart = w;

					int PreFullLines = ReadImage->GetWidth() * h;
					int YPartNormal = PreFullLines;
					int PointOffsetNormal = XPart + YPartNormal;

					int YPartFlipped = ReadImage->GetWidth() * (ReadImage->GetHeight() - h - 1);
					int PointOffsetFlipped = XPart + YPartFlipped;

					PixelColorA[PointOffsetFlipped] = RGBAColor(PixelColor[PointOffsetNormal]);
				}
			}

			UtilDebugString("NiPixelFormat %i", pixldata->GetPixelFormat());
			NiTexture::FormatPrefs BasePref;
			BasePref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::PIX_DEFAULT;
			BasePref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::MIP_DEFAULT;
			BasePref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

			NiTexture::FormatPrefs BlendPref;
			BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
			BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
			BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

			BlendTexture = NiSourceTexture::Create(pixldata, BlendPref);

			
			PgUtil::CreateFullFilePathFromBaseFolder(acFileName, DiffuseFileName);
			UtilDebugString("DiffuseFileName for Texture %s", acFileName);
			BaseTexture = NiSourceTexture::Create(acFileName,BasePref);
			if (BaseTexture == NULL)
				NiMessageBox::DisplayMessage("BaseTexture is Nullptr", "");
		}
		NiString Name;
		NiString DiffuseFileName;
		NiString BlendFileName;
		float StartPos_X;
		float StartPos_Y;
		float Width;
		float Height;
		float UVScaleDiffuse;
		float UVScaleBlend;

		NiPixelData* pixldata;
		NiSourceTexture* BlendTexture;
		NiSourceTexture* BaseTexture;
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
		{
			UtilDebugString("Name: %s", LayerList.at(i)->Name)
			UtilDebugString("DiffuseFileName: %s", LayerList.at(i)->DiffuseFileName)
			UtilDebugString("BlendFileName: %s", LayerList.at(i)->BlendFileName)
		}
				
	}
};

class EditorScene : public FiestaScene
{
public:
	EditorScene(NiString FilePath, NiString FileName);
	~EditorScene() {
		if (_Thread)
			NiDelete _Thread;
		if (_procedure)
			NiDelete _procedure;
	}
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
		PgUtil::CreateFullFilePathFromBaseFolder(acFileName, acTempText);
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
	
	bool LoadObjectPosition(FILE* file, char* acFileBuff, char* acTempText, NiPoint3* kPoint, NiQuaternion* quater, float* scale)
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

	void LoadBeforeObjects(FILE* file);
	void LoadObjects(FILE* file);
	void LoadAfterObjects(FILE* file);


	World kWorld;
	NiColor BackgroundColor;

	
	NiThreadProcedure* _procedure;
	NiThread* _Thread;
	NiString _FilePath;
	NiString _FileName;
	IniFile _IniFile;
	NiNodePtr terrain;

	std::map<NiTexture*, NiTexturingProperty::ShaderMap*> BaseTextureSafeMap;
	std::map<NiTexture*, NiTexturingProperty::ShaderMap*> BlendTextureSafeMap;
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

