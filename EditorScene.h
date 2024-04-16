#pragma once
#include "FiestaScene.h"
#include "IngameWorld.h"

#include <iostream>
#include <string>
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

			std::string acFileName =  PgUtil::CreateFullFilePathFromBaseFolder(BlendFileName);
			std::cout <<"BlendFileName for Texture " << acFileName << std::endl;

			NiPixelData* ReadImage = conv->ReadImageFile(acFileName.c_str(), 0);
			if (ReadImage->GetPixelFormat() != NiPixelFormat::RGB24)
			{
				NiMessageBox::DisplayMessage("NiPixelData From ReadImage in Layer::CreateTexture Has Wrong NiPixelFormat", "Error");
			}
			
			pixldata = NiNew NiPixelData(ReadImage->GetWidth(), ReadImage->GetHeight(), NiPixelFormat::RGBA32);
			RGBAColor* PixelColorA = (RGBAColor*)pixldata->GetPixels();
			RGBColor* PixelColor = (RGBColor*)ReadImage->GetPixels();
			if (ReadImage->GetSizeInBytes() / sizeof(RGBColor) != pixldata->GetSizeInBytes() / sizeof(RGBAColor))
			{
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

					PixelColorA[PointOffsetNormal] = RGBAColor(PixelColor[PointOffsetNormal].g);
				}
			}

			NiTexture::FormatPrefs BasePref;
			BasePref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::PIX_DEFAULT;
			BasePref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::MIP_DEFAULT;
			BasePref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

			NiTexture::FormatPrefs BlendPref;
			BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
			BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
			BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

			BlendTexture = NiSourceTexture::Create(pixldata, BlendPref);

			acFileName = PgUtil::CreateFullFilePathFromBaseFolder(DiffuseFileName);
			BaseTexture = NiSourceTexture::Create(acFileName.c_str(), BasePref);
			if (BaseTexture == NULL)
				NiMessageBox::DisplayMessage("BaseTexture is Nullptr", "");


		}
		std::string Name;
		std::string DiffuseFileName;
		std::string BlendFileName;
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
	std::string FileType;
	float Version;
	std::string HeightFileName;
	std::string VertexColorTexture;
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
		std::cout << "HeightFileName: " << HeightFileName << " LayerCount: " << std::to_string(LayerList.size()) << std::endl;
		for (int i = 0; i < LayerList.size(); i++)
		{
			std::cout << "Name: "<<LayerList.at(i)->Name << std::endl;
			std::cout << "DiffuseFileName: " << LayerList.at(i)->DiffuseFileName << std::endl;
			std::cout << "BlendFileName: " << LayerList.at(i)->BlendFileName << std::endl;
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

	bool CheckSHMDVersion(std::ifstream& SHMD);
	bool LoadSky(std::ifstream& SHMD);
	bool LoadWater(std::ifstream& SHMD);
	bool LoadGroundObject(std::ifstream& SHMD);
	bool LoadGlobalLight(std::ifstream& SHMD);
	bool LoadFog(std::ifstream& SHMD);
	bool LoadBackGroundColor(std::ifstream& SHMD);
	bool LoadFrustum(std::ifstream& SHMD);
	bool LoadGlobalObjects(std::ifstream& SHMD);
	bool LoadOneObject(std::ifstream& SHMD, std::string& path);
	bool LoadDirectionLightAmbient(std::ifstream& SHMD);
	bool LoadDirectionLightDiffuse(std::ifstream& SHMD);

	World kWorld;
	NiColor BackgroundColor;

	
	NiThreadProcedure* _procedure;
	NiThread* _Thread;
	NiString _FilePath;
	NiString _FileName;
	IniFile _IniFile;
	NiNodePtr terrain;
	NiNodePtr TerrainParent;
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

