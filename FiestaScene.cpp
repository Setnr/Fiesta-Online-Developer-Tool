#include "FiestaScene.h"
#include "PgUtil.h"

#include "FiestaOnlineTool.h"
#include <NiViewMath.h>

#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

//const NiRTTI FiestaScene::ms_RTTI("FiestaScene", NULL);
bool FiestaScene::SetupScene(NiNodePtr& m_spScene, NiCameraPtr& m_spCamera)
{
	m_spScene = BaseNode;
    NIASSERT(m_spScene != NULL);
    if (!PgUtil::CatchCamera(m_spScene, &m_spCamera))
    {
        NiMessageBox::DisplayMessage("Failed to Catch Camera", "Error");
        return 0;
    }
    return 1;
}

void FiestaScene::UpdateCamera(float fTime)
{
	
	if (FiestaOnlineTool::GetRotateCamera())
	{
		int iX, iY, iZ;
		if (FiestaOnlineTool::GetPositionDelta(iX, iY, iZ))
		{

			unsigned int uiAppHeight = NiApplication::ms_pkApplication->GetAppWindow()->GetHeight();
			unsigned int uiAppWidth = NiApplication::ms_pkApplication->GetAppWindow()->GetWidth();
			if (uiAppHeight > 0 && uiAppWidth > 0)
			{
				float fPitchDelta = NI_PI * 0.375f * (float)(iY) / (float)uiAppHeight;
				float fHeadingDelta = NI_PI * 0.375f * (float)(iX) / (float)uiAppWidth;
				Pitch += fPitchDelta;
				Yaw -= fHeadingDelta;
				NiMatrix3 rotation;
				rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);
				Camera->SetRotate(rotation);
			}
		}
	}
	bool W_Key = false;
	bool S_Key = false;
	bool A_Key = false;
	bool D_Key = false;
	if (FiestaOnlineTool::IsMoveKeyPressed(W_Key, S_Key, A_Key,D_Key)) 
	{
		NiPoint3 CameraPosition = Camera->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);

		NiPoint3 WorldDirect = Camera->GetWorldDirection() * 32;
		NiPoint3 RightDirect = Camera->GetWorldRightVector() * 32;
		
		if(W_Key)
			MoveDirect += WorldDirect;
		if (S_Key)
			MoveDirect -= WorldDirect;
		if (D_Key)
			MoveDirect += RightDirect;
		if (A_Key)
			MoveDirect -=  RightDirect;
		Camera->SetTranslate(CameraPosition + MoveDirect);
	}
	Camera->Update(0.0f);
}

void FiestaScene::StartImGuiFrame()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void FiestaScene::EndImGuiFrame()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void FiestaScene::DrawImGui() 
{
	auto io = ImGui::GetIO();
	FPS[values_offset] = io.Framerate;
	values_offset = (values_offset + 1) % IM_ARRAYSIZE(FPS);
	float average = 0.0f;
	for (int n = 0; n < IM_ARRAYSIZE(FPS); n++)
		average += FPS[n];
	average /= (float)IM_ARRAYSIZE(FPS);
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 150, 0));
	ImGui::SetNextWindowSize(ImVec2(150,50));
	ImGui::Begin("General", NULL, flags);
	ImGui::Text("    %f FPS", io.Framerate);
	ImGui::Text("AVG %f FPS", average);
	ImGui::End();
}
