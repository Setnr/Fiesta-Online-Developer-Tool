#include "../../Data/TerrainWorld.h"
#include "../../NiApplication/FiestaOnlineTool.h"

class LayerEditWindow 
{
public:
	LayerEditWindow(TerrainWorldPtr& World);
	~LayerEditWindow();
	std::shared_ptr<TerrainLayer>  NewLayer(std::string BledFileName, float Width, float Height);
	void ChangeLayer(std::shared_ptr<TerrainLayer> Layer);

	bool Show();
	void UpdateLayer(std::vector<std::vector<TerrainWorld::HTDHelper>>& _HTD);
	NiScreenElementsPtr GetScreenTexture() { return pkScreenTexture; }
	void UpdateTexturePreview() 
	{
		NiSourceTexturePtr ptr = NiSourceTexture::Create(PgUtil::CreateFullFilePathFromBaseFolder(Layer->DiffuseFileName).c_str());
		if (pkScreenElement)
			FiestaOnlineTool::RemoveScreenElemets(pkScreenElement);
		pkScreenElement = PgUtil::CreateScreenElement(256, 256, ptr);
		auto& io = ImGui::GetIO();
		float fLeft = 1.f - 256 / io.DisplaySize.x;
		float fTop = 1.f - 256 / io.DisplaySize.y;
		float fBottom = 1.f;
		float fRight = 1.f;
		pkScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
		pkScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
		pkScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
		pkScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
		pkScreenElement->UpdateBound();
		FiestaOnlineTool::AddScreenElemets(pkScreenElement);
	}
private:
	void UpdateTexturePos();

	TerrainWorldPtr& kWorld;
	bool _Show = false;
	std::shared_ptr<TerrainLayer> Layer;

	float MinHeight;
	float MaxHeight;
	NiScreenElementsPtr pkScreenElement;
	NiScreenElementsPtr pkScreenTexture;

	int BrushColor = 0x0;
	int BrushSize = 2;
	int LayerCt = 0;
};