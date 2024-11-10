#include "EditMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include "ImGui/ImGuizmo.h"

NiSmartPointer(SHBDMode);
class SHBDMode : public EditMode
{
	NiDeclareRTTI;
	SHBDMode(IngameWorldPtr World, EditorScenePtr Scene) : EditMode(World, (FiestaScenePtr)&* Scene)
	{
		Camera = World->GetCamera();
		MouseOrb = PgUtil::LoadNifFile(PgUtil::PathFromApplicationFolder(".\\FiestaOnlineTool\\HTDCircle.nif").c_str());
		NiWireframePropertyPtr ptr = NiNew NiWireframeProperty;
		ptr->SetWireframe(true);
		MouseOrb->AttachProperty(ptr);
		MouseOrb->SetScale((50.f / 160.f) * _BrushSize);
		_BaseNode = NiNew NiNode;

		CreateSHBDNode();

		_BaseNode->AttachChild(MouseOrb);
		_BaseNode->AttachChild(_SHBDNode);

		Light = NiNew NiAmbientLight();
		Light->SetAmbientColor(NiColor(0.792157f, 0.792157f, 0.792157f));
		Light->SetDiffuseColor(NiColor::WHITE);
		Light->SetSpecularColor(NiColor::BLACK);
		Light->AttachAffectedNode(_BaseNode);
		_BaseNode->AttachProperty(NiNew NiVertexColorProperty);
		_BaseNode->AttachChild(Light);

		_BaseNode->UpdateEffects();
		_BaseNode->UpdateProperties();
		_BaseNode->Update(0.f);

		ScreenElements.push_back(NiNew LuaElement(Scene, "EditorELements/SHBD.lua"));
		SetBrushSize(_BrushSize);
		_FarFrustum = kWorld->GetFarFrustum();
		_FogDepth = kWorld->GetFogDepth();
		kWorld->SetFarFrustum(50000.f, false);
		kWorld->SetFogDepth(0.0f, false);
	}
	~SHBDMode()
	{
		kWorld->SetFarFrustum(_FarFrustum, false);
		kWorld->SetFogDepth(_FogDepth, false);
		TextureConnector.clear();
		_BaseNode = NULL;
		MouseOrb = NULL;
		Light = NULL;
		_SHBDNode = NULL;
		Camera = NULL;
		SetShowElements(true);
	}
	virtual void Draw();
	virtual void Update(float fTime);
	virtual void ProcessInput();
	virtual std::string GetEditModeName() { return "SHBD"; }
	
	int GetBrushSize() { return _BrushSize; }
	void SetBrushSize(int Size) { _BrushSize = Size; MouseOrb->SetScale((6.25f / 160.f) * _BrushSize);	}
	void SetShowElements(bool Show);
	bool GetShowElements() { return _ShowElements; }
	void SetWalkable(bool Walkable) { _Walkable = Walkable; }
	bool GetWalkable() { return _Walkable; }
private:

	int _BrushSize = 1;
	bool _ShowElements = true;
	bool _Walkable = true;
	float _FarFrustum,_FogDepth;
	

	void UpdateMouseIntersect();
	void CreateSHBDNode();
	NiNodePtr _BaseNode;
	NiNodePtr _SHBDNode;
	NiAmbientLightPtr Light;
	NiNodePtr MouseOrb;
	NiCameraPtr Camera;
	NiPoint3 MouseIntersect;
	std::vector<std::vector<NiPixelDataPtr>> TextureConnector;
	int TextureSize = 128;

	unsigned int Walkable, Blocked;
};