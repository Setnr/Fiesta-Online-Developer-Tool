#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <ImGui/imgui.h>
#include <ImGui/imfilebrowser.h>
#include <Data/IngameWorld/IngameWorld.h>
#include <NiDX9Renderer.h>
class AddSingleObject : public ScreenElement
{
public:
	AddSingleObject(IngameWorldPtr world, void (IngameWorld::* UnDoFunc)(NiNodePtr, bool), void (IngameWorld::* ReDoFunc)(NiNodePtr, bool), NiPoint3 pos) : _FileBrowser(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_NoModal)
	{
		_World = world;
		_UnDoFunc = UnDoFunc;
		_ReDoFunc = ReDoFunc;
		_Pos = pos;
	}
	virtual bool Draw();
private:
	IngameWorldPtr _World;
	void (IngameWorld::* _UnDoFunc)(NiNodePtr, bool);
	void (IngameWorld::* _ReDoFunc)(NiNodePtr, bool);
	NiPoint3 _Pos;
	ImGui::FileBrowser _FileBrowser;
};

