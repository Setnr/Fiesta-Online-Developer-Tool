#pragma once
#include <NiMain.h>
NiSmartPointer(FiestaScene);
class FiestaScene : public NiRefObject
{
	NiDeclareRootRTTI(FiestaScene);
public:
	FiestaScene() 
	{
		BaseNode = NiNew NiSortAdjustNode;
	};
	~FiestaScene() 
	{
	}
	virtual bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea);
	virtual bool UpdateScene()
	{
		return 0;
	}
	virtual FiestaScene* GetNewScene() 
	{
		return NULL;
	}
	virtual void Draw(NiRenderer* renderer) {}
protected:
	NiSortAdjustNodePtr BaseNode;
	
};

