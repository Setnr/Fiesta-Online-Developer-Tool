#pragma once
#include "EditScene.h"
#include "FiestaOnlineTool.h"
#include "ImGui/ImGuizmo.h"
void EditScene::UpdateCamera(float fTime)
{
	if (Camera == NULL)
		return;
	FiestaScene::UpdateCamera(fTime);

	if (ImGui::IsAnyItemHovered())
		return;

	if (ImGui::IsKeyPressed((ImGuiKey)0x52)) //R
	{
		ResetCamera();
	}
	switch (CurrentEditMode) 
	{
	case SHMD:
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
		{
			SelectObject();
		}
		SHMDCopyPaste();
		SHMDDelete();
		break;
	case SHBD:
		UpdateSHBDTexture();
		MoveSHBDTexture(fTime);
		MoveViaMiddleMouse();
		break;
	case HTDG:
		NiPoint3 kOrigin, kDir;
		long X, Y;
		FiestaOnlineTool::GetMousePosition(X, Y);
		if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
		{
			NiPick _Pick;
			_Pick.SetPickType(NiPick::FIND_FIRST);
			_Pick.SetSortType(NiPick::SORT);
			_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
			_Pick.SetFrontOnly(true);
			_Pick.SetReturnNormal(true);
			_Pick.SetObserveAppCullFlag(true);
			_Pick.SetTarget(kWorld->GetTerrainScene());
			if (_Pick.PickObjects(kOrigin, kDir, true))
			{
				NiPick::Results& results = _Pick.GetResults();
				NiPoint3 Intersect = results.GetAt(0)->GetIntersection();
				
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					_HTDBrush->UpdateHTD(kWorld->GetIni(), kWorld->GetHTD(), Intersect);
				}
				HTDOrbNode->SetTranslate(Intersect);
			}
		}
		break;
	}
}

void EditScene::SHMDCopyPaste() 
{
	if (SelectedObj && (
		ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x43)
		|| ImGui::IsKeyPressed((ImGuiKey)VK_CONTROL) && ImGui::IsKeyDown((ImGuiKey)0x43)
		)
		)  // ctrl c
	{
		CopyObj = SelectedObj;
	}
	float CurTime = NiGetCurrentTimeInSec();
	if (LastPasteTime + Settings::PasteDelay() < CurTime && CopyObj && (
		ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x56)
		|| ImGui::IsKeyPressed((ImGuiKey)VK_CONTROL) && ImGui::IsKeyDown((ImGuiKey)0x56)
		)
		)   // ctrl v
	{
		LastPasteTime = CurTime;
			
		NiPoint3 kOrigin, kDir;
		long X, Y;
		FiestaOnlineTool::GetMousePosition(X, Y);
		if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
		{
			NiPick _Pick;
			_Pick.SetPickType(NiPick::FIND_FIRST);
			_Pick.SetSortType(NiPick::SORT);
			_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
			_Pick.SetFrontOnly(true);
			_Pick.SetReturnNormal(true);
			_Pick.SetObserveAppCullFlag(true);
			_Pick.SetTarget(kWorld->GetWorldScene());
			if (_Pick.PickObjects(kOrigin, kDir, true))
			{
				NiPick::Results& results = _Pick.GetResults();
				NiPickablePtr Obj = (NiPickable*)CopyObj->Clone();
				NiNodePtr ptr = (NiNode*)&*Obj;
				kWorld->AttachGroundObj(ptr);

				Obj->SetName(CopyObj->GetName());
				Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);

				Obj->SetSelectiveUpdateRigid(true);
				auto node = kWorld->GetGroundObjNode();
				node->UpdateEffects();
				node->UpdateProperties();
				node->Update(0.0);
				SelectedObj = Obj;
				
				SelectedObj->SetTranslate(results.GetAt(0)->GetIntersection());
			}
		}
	}
}

void EditScene::SHMDDelete() 
{
	if (ImGui::IsKeyPressed((ImGuiKey)VK_DELETE))
	{
		if (!SelectedObj)
		{
			SelectObject();
		}
		if (SelectedObj)
		{
			SelectedObj->GetParent()->DetachChild(SelectedObj);
			SelectedObj = NULL;
		}
	}
}

void EditScene::UpdateSHBDTexture() 
{
	NiPoint3 kOrigin, kDir;
	long X, Y;
	FiestaOnlineTool::GetMousePosition(X, Y);
	NiPoint3 Intersect(0.f, 0.f, 0.f);
	if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetTarget(kWorld->GetSHBDNode());
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(false);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);

		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();
			for (int i = 0; i < results.GetSize(); i++)
			{
				auto result = results.GetAt(i);
				Intersect = result->GetIntersection();
				kWorld->CreateBrushTexture(Intersect, BrushSize, MoveStatus);
			}
		}
	}
}

void EditScene::MoveViaMiddleMouse() 
{
	ImGuiIO& io = ImGui::GetIO();
	static POINT CursorPos;
	POINT CurrentCursorPos;
	GetCursorPos(&CurrentCursorPos);
	auto xoffset = CurrentCursorPos.x - CursorPos.x;
	auto yoffset = CurrentCursorPos.y - CursorPos.y;
	CursorPos = CurrentCursorPos;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		NiPoint3 CameraPosition = Camera->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);
		float SpeedUp = 3.0f;
		if (io.KeyShift)
			SpeedUp = 10.0f;
		NiPoint3 UpVector = Camera->GetWorldUpVector();
		UpVector.z = 0.f;

		NiPoint3 RightDirect = Camera->GetWorldRightVector();
		RightDirect.z = 0.f;

		if (yoffset)
			MoveDirect += UpVector * yoffset;
		if (xoffset)
			MoveDirect -= RightDirect * xoffset;

		Camera->SetTranslate(CameraPosition + MoveDirect);
	}
}

void EditScene::MoveSHBDTexture(float fTime)
{
	ImGuiIO& io = ImGui::GetIO();
	float DeltaTime = fTime - FiestaOnlineTool::GetLastUpdateTime();
	if (io.MouseWheel != 0.0f)
	{
		NiNodePtr SHBDNode = kWorld->GetSHBDNode();
		NiPoint3 NodePositon = SHBDNode->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);

		float SpeedUp = io.MouseWheel;
		if (io.KeyShift)
			SpeedUp *= 5.0f;
		NodePositon.z = NodePositon.z + 115.f * DeltaTime * SpeedUp;

		SHBDNode->SetTranslate(NodePositon);
	}

}