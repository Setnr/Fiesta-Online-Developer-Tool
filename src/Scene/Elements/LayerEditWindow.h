#include "../../Data/TerrainWorld.h"


class LayerEditWindow 
{
public:
	LayerEditWindow(TerrainWorldPtr World);

	std::shared_ptr<TerrainLayer>  NewLayer(std::string BledFileName, float Width, float Height);
	void ChangeLayer(std::shared_ptr<TerrainLayer> Layer);

	bool Show();
	void UpdateLayer(std::vector<std::vector<TerrainWorld::HTDHelper>>& _HTD);
private:
	TerrainWorldPtr kWorld;
	bool _Show = false;
	std::shared_ptr<TerrainLayer> Layer;

	float MinHeight;
	float MaxHeight;
	NiScreenElementsPtr pkScreenTexture;
};