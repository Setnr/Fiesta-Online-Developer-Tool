#include "FiestaScene.h"
#include "PgUtil.h"

#include "FiestaOnlineTool.h"
#include <NiViewMath.h>

#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Logger.h"

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

	auto& io = ImGui::GetIO();
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		Camera->Update(0.0f);
		return;
	}
	static POINT CursorPos;
	static bool UpdateMouse = false;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) 
	{
		GetCursorPos(&CursorPos);
		UpdateMouse = true;
		FiestaOnlineTool::DisableCursor();
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		UpdateMouse = false;
		FiestaOnlineTool::EnableCursor();
	}
	if(ImGui::IsMouseDown(ImGuiMouseButton_Right) && UpdateMouse)
	{
		POINT CurrentCursorPos;
		GetCursorPos(&CurrentCursorPos);
		CurrentCursorPos.x = CurrentCursorPos.x - CursorPos.x;
		CurrentCursorPos.y = CurrentCursorPos.y - CursorPos.y;
		SetCursorPos(CursorPos.x, CursorPos.y);

		if (CurrentCursorPos.x || CurrentCursorPos.y)
		{
			unsigned int uiAppHeight = NiApplication::ms_pkApplication->GetAppWindow()->GetHeight();
			unsigned int uiAppWidth = NiApplication::ms_pkApplication->GetAppWindow()->GetWidth();
			if (uiAppHeight > 0 && uiAppWidth > 0)
			{
				float fPitchDelta = NI_PI * 0.375f * ((float)CurrentCursorPos.y) / (float)uiAppHeight;
				float fHeadingDelta = NI_PI * 0.375f * ((float)CurrentCursorPos.x) / (float)uiAppWidth;
				
				Pitch += fPitchDelta;
				Yaw -= fHeadingDelta;
				NiMatrix3 rotation;
				rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);
				Camera->SetRotate(rotation);
			}
		}
	}
	float DeltaTime = fTime - FiestaOnlineTool::GetLastUpdateTime();
	if (io.MouseWheel != 0.0f) 
	{
		NiPoint3 CameraPosition = Camera->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);

		float SpeedUp = io.MouseWheel;
		if (io.KeyShift)
			SpeedUp *= 5.0f;
		NiPoint3 WorldDirect = Camera->GetWorldDirection() * 115.f * DeltaTime * SpeedUp;

		MoveDirect += WorldDirect;
		Camera->SetTranslate(CameraPosition + MoveDirect);
	}
	bool W_Key = ImGui::IsKeyDown((ImGuiKey)0x57);
	bool S_Key = ImGui::IsKeyDown((ImGuiKey)0x53);
	bool A_Key = ImGui::IsKeyDown((ImGuiKey)0x41);
	bool D_Key = ImGui::IsKeyDown((ImGuiKey)0x44);
	bool E_Key = ImGui::IsKeyDown((ImGuiKey)0x45);
	bool Q_Key = ImGui::IsKeyDown((ImGuiKey)0x51);
	if (W_Key || S_Key || A_Key || D_Key || E_Key || Q_Key)
	{
		
		NiPoint3 CameraPosition = Camera->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);
		float SpeedUp = 3.0f;
		if (io.KeyShift)
			SpeedUp = 10.0f;
		NiPoint3 WorldDirect = Camera->GetWorldDirection() * 115.f * DeltaTime * SpeedUp;
		NiPoint3 RightDirect = Camera->GetWorldRightVector() * 115.f * DeltaTime * SpeedUp;
		
		if(W_Key)
			MoveDirect += WorldDirect;
		if (S_Key)
			MoveDirect -= WorldDirect;
		if (D_Key)
			MoveDirect += RightDirect;
		if (A_Key)
			MoveDirect -=  RightDirect;
		if (Q_Key)
			CameraPosition.z += 115.f * DeltaTime * SpeedUp;
		if (E_Key)
			CameraPosition.z -= 115.f * DeltaTime * SpeedUp;
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

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 150, 50));
	ImGui::SetNextWindowSize(ImVec2(150,100));
	ImGui::Begin("General", NULL, flags);
	ImGui::Text("    %f FPS", io.Framerate);
	ImGui::Text("AVG %f FPS", average);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 150));
	ImGui::SetNextWindowSize(ImVec2(500, 150));
	ImGui::Begin("Logger", NULL, flags);
	Logger::Draw();
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);
	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		this->CreateMenuBar();
		ImGui::EndMainMenuBar();
	}
}