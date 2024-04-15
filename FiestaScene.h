#pragma once
#include <NiMainLibType.h>
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
	virtual void Update(float fTime)
	{
		this->UpdateCamera(fTime);
		BaseNode->Update(fTime);
	}
	virtual void UpdateCamera(float fTime);
	
protected:
	NiSortAdjustNodePtr BaseNode;
	NiCameraPtr Camera;

	float Yaw;
	float Pitch;
	float Roll;
	
};

