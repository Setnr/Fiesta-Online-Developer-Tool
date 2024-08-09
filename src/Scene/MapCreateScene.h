#pragma once
#include <NiMain.h>
#include "FiestaScene.h"
#include "../Data/TerrainWorld.h"
#include "../Data/Fractal.h"
#include "../Data/DiamondSquareFractal.h"
#include "../Data/PerlinFractal.h"
#include "../Data/WorleyFractal.h"
#include "Elements/MapMenu.h"
NiSmartPointer(MapCreateScene);
class MapCreateScene : public FiestaScene  
{
public:
	MapCreateScene() {
		UpdateAlgorithm(_Algo);
		LookAndMoveAtWorldPoint(kWorld->GetSpawnPoint());
		CanSwitch = true;
	}
	~MapCreateScene() {
		_Fractal->Show(false);
	}
	virtual void Draw(NiRenderer* renderer) 
	{
		if (!kWorld || !kWorld->GetTerrainScene())
			return;
		NiVisibleArray m_kVisible2;
		NiCullingProcess m_spCuller2(&m_kVisible2);
		NiDrawScene(kWorld->GetCamera(), kWorld->GetTerrainScene(), m_spCuller2);
	}
	virtual void DrawImGui() ;
	virtual void CreateMenuBar();
	//virtual void Update(float fTime)
	virtual void UpdateCamera(float fTime) { if(Camera)FiestaScene::UpdateCamera(fTime); }

private:
	MapMenu _MapMenu;
	int MapSize = 129;
	int Seed = 12345;
	float noise = 0.5f; 
	float MinValue = 0.f;
	float MaxValue = 40.f;
	int NumPoints = 10;
	int PointLoc = 0;
	float Persistance = 0.98f;
	enum CreateAlgorithm {
		Perlin,
		Worley,
		DiamondSquare
	} _Algo = DiamondSquare;
	void UpdateAlgorithm(CreateAlgorithm NewAlgorithm)
	{
		_Algo = NewAlgorithm;
		if(_Fractal)
			_Fractal->Show(false);
		switch (_Algo) {
		case Perlin:
			_Fractal = NiNew PerlinFractal(Octave, MapSize,Persistance);
			break;
		case Worley:
			_Fractal = NiNew WorleyFractal(NumPoints,PointLoc,Seed,MapSize);
			break;
		case DiamondSquare:
		default:
			_Fractal = NiNew DiamondSquareFractal(MapSize);
				
		}
		_Fractal->generateGrid(MapSize, Seed, noise,MinValue,MaxValue);
		CreateTerrain();
		_Fractal->Show(AlgoPreview);
	}
	void CreateTerrain() 
	{
		if (!kWorld)
		{
			kWorld = NiNew TerrainWorld("");
			Camera = kWorld->GetCamera();
			LookAndMoveAtWorldPoint(NiPoint3(0.0f, 0.0f, 0.0f));
			kWorld->SetAmbientLightAmbientColor(NiColor::WHITE);
		}
		_Fractal->CreateTerrain(kWorld, MapSize, ShadowMap);
	}        
	enum MapType {
		Field,
		IDField,
		KDField
	};
	MapType SaveType = Field;
	char MapName[128] = "SetTool";
	void Save() 
	{
		std::string SubPath = ".\\resmap\\";
		switch (SaveType)
		{
		case Field: SubPath += "field";
			break;
		case IDField: SubPath += "IDField";
			break;
		case KDField: SubPath += "KDField";
			break;
		}
		std::string _MapName = std::string(MapName);
		SubPath += "\\" + _MapName;
		std::string Direcotry = PgUtil::CreateFullFilePathFromBaseFolder(SubPath);
		if (!std::filesystem::exists(Direcotry))
		{
			std::filesystem::create_directories(Direcotry);
		}

		_Fractal->SaveHTD(Direcotry + "\\" + _MapName + ".HTD");
		IniFile file(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\BaseIni.ini"));
		if (!file.Load())
		{
			NiMessageBox::DisplayMessage("Cant Find BaseIni.ini", "ERROR");
			return;
		}
		file.HeightMap_height = MapSize;
		file.HeightMap_width = MapSize;
		file.HeightFileName = SubPath + "\\" + _MapName + ".HTD";
		file.VertexColorTexture = SubPath + "\\Vertex" + _MapName + ".bmp";
		file.LayerList[0]->Height = MapSize - 1;
		file.LayerList[0]->Width = MapSize - 1;
		file.LayerList[0]->Name = "0 BaseTexture";
		file.LayerList[0]->BlendFileName = SubPath + "\\0 BaseTexture.bmp";

		file.LayerList[0]->BlendTexture = _Fractal->GetSourceTexture();
		PgUtil::SaveTexture(Direcotry + "\\Vertex" + _MapName + ".bmp", _Fractal->GetSourceTexture());
		file.UpdateFilePath(Direcotry + "\\" + _MapName + ".ini");

		RGBApixel* PixelData = (RGBApixel*)_Fractal->GetSourceTexture()->GetSourcePixelData()->GetPixels();
		for (int w = 0; w < _Fractal->GetSourceTexture()->GetWidth(); w++)
		{
			for (int h = _Fractal->GetSourceTexture()->GetHeight() - 1; h >= 0; h--)
			{
				int XPart = w;

				int PreFullLines = _Fractal->GetSourceTexture()->GetWidth() * h;
				int YPartNormal = PreFullLines;
				int PointOffsetNormal = XPart + YPartNormal;
				PixelData[PointOffsetNormal] = RGBApixel(0xFF, 0xFF, 0xFF, 0xFF);
			}
		}

		file.Save();
		std::string FullSubPath = PgUtil::CreateFullFilePathFromBaseFolder(SubPath);

		if (std::filesystem::exists(FullSubPath + "\\" + _MapName + ".shmd.bak"))
			std::filesystem::remove(FullSubPath + "\\" + _MapName + ".shmd.bak");
		if (std::filesystem::exists(FullSubPath + "\\" + _MapName + ".shmd"))
		{
			std::filesystem::copy(FullSubPath + "\\" + _MapName + ".shmd", FullSubPath + "\\" + _MapName + ".shmd.bak");
			std::filesystem::remove(FullSubPath + "\\" + MapName + ".shmd");
		}
		std::filesystem::copy(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\Base.shmd"), FullSubPath + "\\" + _MapName + ".shmd");
		ShineBlockData::Save(FullSubPath, _MapName, file.HeightMap_width - 1);

		_Fractal->SaveShadows(PgUtil::CreateFullFilePathFromBaseFolder(file.VertexColorTexture));

		LogInfo("Saved Everything Successfully, you can now add the Map to your MapInfo with Spawn 0 / 0\nPlease reload the MapInfo afterwards!");


	}
	int Octave = 10;
	bool AlgoPreview = false;
	FractalPtr _Fractal;
	TerrainWorldPtr kWorld;
	bool ShowHTDSave = false;
	bool ShadowMap = false;
	bool RenderShadowsLive = false;
};