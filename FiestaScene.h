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
protected:
	NiSortAdjustNodePtr BaseNode;
};

