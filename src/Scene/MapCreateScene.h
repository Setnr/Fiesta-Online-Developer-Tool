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
		kWorld->ClearTerrainScene();
		_Fractal->CreateTerrain(kWorld, MapSize, ShadowMap);
		kWorld->GetTerrainScene()->UpdateEffects();
		kWorld->GetTerrainScene()->UpdateProperties();
		kWorld->GetTerrainScene()->Update(0.0f);
	}
	int Octave = 10;
	bool AlgoPreview = false;
	FractalPtr _Fractal;
	TerrainWorldPtr kWorld;
	bool ShowHTDSave = false;
	bool ShadowMap = false;
	NiPoint3 SunVector;
};