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
	bool SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamerea);
	virtual void UpdateScene(){}
protected:
	NiSortAdjustNodePtr BaseNode;
};

