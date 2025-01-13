#pragma once
#include "TerrainMode.h"
#include "Brush/Brush.h"
#include <Data/HeightTerrainData/HeightTerrainData.h>

NiSmartPointer(TerrainBrushMode);
class TerrainBrushMode : public TerrainMode 
{
	NiDeclareRTTI;
	TerrainBrushMode(IngameWorldPtr World, FiestaScenePtr Scene) : TerrainMode(World, Scene) 
	{
	}
	~TerrainBrushMode() 
	{
		_Brushes.clear();
		_CurrentBrush = NULL;
	}

	virtual void Update(float fTime);
	virtual void ProcessInput();
	void DrawBrushes();
protected:
	void LoadBrushes(std::string BrushFolder);

	BrushPtr GetCurrentBrush() { return _CurrentBrush; }
	std::vector<BrushPtr> _Brushes;
	BrushPtr _CurrentBrush;
};