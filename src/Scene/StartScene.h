#pragma once
#include "FiestaScene.h"
#include <NiThread.h>
#include "PgUtil.h"

NiSmartPointer(StartScene);
class StartScene : public FiestaScene
{
public:
	StartScene();
	~StartScene() 
	{
	}

	void UpdateCamera(float fTime) {}
	void Draw(NiRenderer* renderer)
	{
		if (!Camera)
			PgUtil::CatchCamera(BaseNode, &Camera);
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		
		NiDrawScene(Camera, BaseNode, m_spCuller);
	}
	virtual void DrawImGui();
	virtual void CreateMenuBar();
protected:
	bool ShowLoadMenu;
};



