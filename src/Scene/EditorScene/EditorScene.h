#pragma once
#include <FiestaScene/FiestaScene.h>
#include <mutex>
#include <IngameWorld/IngameWorld.h>
#include <Scene/ScreenElements/ScreenElement.h>
#include "EditorScene/Modes/EditMode.h"

NiSmartPointer(EditorScene);
class EditorScene : public FiestaScene 
{
	NiDeclareRTTI;
public:
	EditorScene();
	~EditorScene();
	virtual void Draw(NiRenderer* renderer)
	{
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		if (kWorld) 
		{
			renderer->SetBackgroundColor(kWorld->GetBackgroundColor());
			Camera->SetViewFrustum(kWorld->GetSkyFrustum());
			for(auto sky : kWorld->GetSkyNodeList())
			{
				NiDrawScene(Camera, sky, m_spCuller);
			}
			Camera->SetViewFrustum(kWorld->GetWorldFrustum());
		}
		NiDrawScene(Camera, BaseNode, m_spCuller);

	}
	virtual void DrawImGui();
	void LoadNewMap(MapInfo* info);
	void CreateNewMap(MapInfo* info, int MapSize);
	void SetWorld(IngameWorldPtr NewWorld) { NewWorldLoaded = NewWorld; }
	virtual void Update(float fTime);
	
	virtual void Terminate() { _EditMode = NULL; ScreenElements.clear(); if(kWorld) kWorld->ClearStacks(); }
	virtual void CreateMenuBar();
	IngameWorldPtr GetWorld() { return kWorld; }
	virtual void ProcessInput();
	EditModePtr GetCurrentEditMode() { return _EditMode; }
private:
	void UpdateEditMode();
	IngameWorldPtr NewWorldLoaded;
	IngameWorldPtr kWorld;

	EditModePtr _EditMode;

	std::vector<ScreenElementPtr> ScreenElements;
};