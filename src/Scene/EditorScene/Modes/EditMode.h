#pragma once
#include <NiMainLibType.h>
#include <NiMain.h>
#include <NiRTTI.h>

#include <IngameWorld/IngameWorld.h>
#include <FiestaScene/FiestaScene.h>
#include <Scene/ScreenElements/ScreenElement.h>
NiSmartPointer(EditMode);
class EditMode : public NiRefObject
{
	NiDeclareRootRTTI(EditMode);
	EditMode(IngameWorldPtr World,FiestaScenePtr Scene) : kWorld(World),_Scene(Scene){}
	~EditMode() 
	{
		_Scene = NULL; 
		kWorld = nullptr; 
		ScreenElements.clear(); 
	}
	virtual void Update(float fTime);
	virtual void Draw();
	virtual void ProcessInput(){}
	virtual std::string GetEditModeName() { return ""; }
protected:
	IngameWorldPtr kWorld;
	FiestaScenePtr _Scene;
	std::vector<ScreenElementPtr> ScreenElements;
};