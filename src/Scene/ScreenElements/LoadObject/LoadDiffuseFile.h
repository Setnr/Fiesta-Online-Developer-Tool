#pragma once
#include <Scene/ScreenElements/ScreenElement.h>
#include <ImGui/imgui.h>
#include <ImGui/imfilebrowser.h>
#include <Data/IngameWorld/IngameWorld.h>
#include <NiDX9Renderer.h>
#include "AddSingleObject.h"
#include "FiestaOnlineTool.h"
#include <Data/IngameWorld/WorldChanges/FogChange.h>
#include <Scene/EditorScene/Modes/TextureMode.h>
class LoadDiffuseFile : public ScreenElement
{
	NiDeclareRTTI;
public:
	LoadDiffuseFile(std::shared_ptr<TerrainLayerData> Layer, IngameWorldPtr World, TextureModePtr Mode) : _FileBrowser(ImGuiFileBrowserFlags_NoTitleBar | ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_NoModal)
	{
		_Layer = Layer;
		_World = World;
		_ScreenElement = NULL;
		_FileBrowser.Open();
		_FileBrowser.SetPwd(AddSingleObject::GetInitPath());
		_FileBrowser.SetTypeFilters({ ".bmp", ".dds"});
		_Mode = Mode;
		
	}
	virtual bool Draw()
	{
		_FileBrowser.Display();

		if (!_FileBrowser.IsOpened())
			return false;

		if (CurPath != _FileBrowser.GetSelected())
		{
			CurPath = _FileBrowser.GetSelected().string();
			if(CurPath.ends_with("bmp") || CurPath.ends_with("dds"))
			{
				_Texture = NiSourceTexture::Create(CurPath.c_str());
				if (_Texture)
				{
					_ScreenElement = PgUtil::CreateScreenElement(_Texture->GetWidth(), _Texture->GetHeight(), _Texture);
				}
			}
		}
		
		if (_FileBrowser.HasSelected())
		{
			auto ClientPath = PgUtil::PathFromClientFolder("");
			std::string OldFile = _Layer->DiffuseFileName;
			_Layer->DiffuseFileName = "." + CurPath.substr(ClientPath.length());
			_Layer->LoadDiffuseFile();
			AddSingleObject::SetInitPath(_FileBrowser.GetPwd().string());

			_World->AttachStack(NiNew LayerDiffuseChange(_World, _Layer, OldFile,_Layer->DiffuseFileName, _Mode));
			_World->ShowTerrain(_World->GetShowTerrain());
			_Mode->UpdateCurrentLayer(_Layer);
			return false;
		}
		if(_ScreenElement)
		{
			auto WindowPos = ImGui::GetWindowPos();
			auto Size = ImGui::GetWindowSize();

			auto& io = ImGui::GetIO();

			unsigned int Width = _Texture->GetWidth();
			unsigned int Height = _Texture->GetHeight();

			float fLeft = (io.DisplaySize.x - Width) / io.DisplaySize.x;
			float fTop = 0.f;//(Height) / io.DisplaySize.y;

			float fBottom = fTop + (Height / io.DisplaySize.y);
			float fRight = fLeft + (Width / io.DisplaySize.x);

			_ScreenElement->SetVertex(0, 0, NiPoint2(fLeft, fBottom));
			_ScreenElement->SetVertex(0, 1, NiPoint2(fRight, fBottom));
			_ScreenElement->SetVertex(0, 2, NiPoint2(fRight, fTop));
			_ScreenElement->SetVertex(0, 3, NiPoint2(fLeft, fTop));
			_ScreenElement->UpdateBound();

			auto renderer = FiestaOnlineTool::GetRenderer();
			if (renderer)
				_ScreenElement->Draw(renderer);
		}
		return true;
	}

private:
	IngameWorldPtr _World;
	ImGui::FileBrowser _FileBrowser;
	std::string CurPath = "";
	std::shared_ptr<TerrainLayerData> _Layer;
	NiScreenElementsPtr _ScreenElement;
	NiSourceTexturePtr _Texture;
	TextureModePtr _Mode;
};

