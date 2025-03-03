#include "NPCEditMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include <NPCData/NPCData.h>
#include <Data/NiCustom/ShineNPCNode.h>

NiImplementRTTI(NPCEditMode, EditMode);

void NPCEditMode::Draw()
{
	EditMode::Draw();
	DrawGizmo();
}
void NPCEditMode::Update(float fTime)
{
}

void NPCEditMode::DrawGizmo()
{
	if (SelectedObjects.empty())
		return;

	/*
	Huge Credits to Maki for helping me with this Code
	by giving me his :)
	*/

	float_t matrix[16]{};
	float matrixScale[3] = { 1.f, 1.f, 1.f };
	glm::vec3 tmpRotation{};
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::BeginFrame();
	NiPickablePtr SelectedNode = SelectedObjects.back();

	NiCameraPtr Camera = kWorld->GetCamera();

	NiPoint3 target = Camera->GetTranslate() + Camera->GetWorldDirection() * 10.f;
	const auto eye = Camera->GetTranslate();
	const auto up = glm::vec3{ 0, 0, 1 };

	const auto view = glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(target.x, target.y, target.z), up);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(kWorld->GetFOV()), (((float_t)io.DisplaySize.x / (float_t)io.DisplaySize.y)), Camera->GetViewFrustum().m_fNear, Camera->GetViewFrustum().m_fFar);

	NiPoint3 PreviousPos = SelectedNode->GetTranslate();
	NiPoint3 ChangeingPosition = SelectedNode->GetTranslate();

	ImGuizmo::RecomposeMatrixFromComponents(&ChangeingPosition.x, &tmpRotation[0], matrixScale, (float*)matrix);

	ImGuizmo::SetRect((float_t)0, (float_t)0, (float_t)io.DisplaySize.x, (float_t)io.DisplaySize.y);
	if (SnapMovement)
		ImGuizmo::Manipulate(&view[0][0], &projectionMatrix[0][0], OperationMode, ImGuizmo::MODE::WORLD, (float*)matrix, nullptr, &SnapSize.x);
	else
		ImGuizmo::Manipulate(&view[0][0], &projectionMatrix[0][0], OperationMode, ImGuizmo::MODE::WORLD, (float*)matrix, nullptr, nullptr);

	ImGuizmo::DecomposeMatrixToComponents((float*)matrix, (float*)&ChangeingPosition.x, &tmpRotation[0], matrixScale);

	NiPoint3 Move = ChangeingPosition - PreviousPos;
	if (Move != NiPoint3::ZERO)
	{
		kWorld->UpdatePos(SelectedObjects, ChangeingPosition - PreviousPos);
		
	}
	if (tmpRotation != glm::vec3{ 0,0,0 })
	{
		kWorld->UpdateRotation(SelectedObjects, tmpRotation);
	}

}

void NPCEditMode::ProcessInput()
{
	if (ImGui::IsAnyItemActive())
		return;
	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x53))
	{
		NPCData::SaveNPCs();
	}
	if (ImGuizmo::IsOver())
		return;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		SelectObject(kWorld->PickObject(), ImGui::IsKeyDown((ImGuiKey)VK_CONTROL));
	}
	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL))
	{
		if (ImGui::IsKeyPressed((ImGuiKey)0x45)) //e
			OperationMode = ImGuizmo::OPERATION::ROTATE_Z;
		if (ImGui::IsKeyPressed((ImGuiKey)0x57)) //w
			OperationMode = ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
		//if (ImGui::IsKeyPressed((ImGuiKey)0x44)) //d
		//	SnapMovement = !SnapMovement;
		//if (HasSelectedObject() && ImGui::IsKeyPressed((ImGuiKey)0x43))
		//	CopyObject = SelectedObjects.back();
		if (CopyObject && ImGui::IsKeyPressed((ImGuiKey)0x56))
		{
			auto pos = kWorld->GetWorldPoint();
			CopyObject->HideBoundingBox();
			NiPickablePtr NewObject = (NiPickable*)CopyObject->Clone();
			NewObject->SetTranslate(pos);
			kWorld->AddObject({ NewObject });
			SelectObject(NewObject);
		}
	}
	if (ImGui::IsKeyPressed((ImGuiKey)VK_ESCAPE))
		ClearSelectedObjects();
	if (HasSelectedObject() && ImGui::IsKeyPressed((ImGuiKey)VK_DELETE))
	{
		for (auto pickable : SelectedObjects) 
		{
			if (NiIsKindOf(ShineNPCNode, pickable)) 
			{
				ShineNPCNodePtr ptr = NiSmartPointerCast(ShineNPCNode, pickable);
				kWorld->RemoveShineObject(NiSmartPointerCast(ShineObjectNode,ptr));
				auto obj = ptr->GetShineObject();
				if (NiIsKindOf(ShineNPC, obj)) 
				{
					ShineNPCPtr npc = NiSmartPointerCast(ShineNPC, obj);
					NPCData::RemoveNPC(npc);
				}
			}
		}
		ClearSelectedObjects();
	}
	if (HasSelectedObject() && ImGui::IsKeyDown((ImGuiKey)VK_CONTROL) && ImGui::IsKeyPressed((ImGuiKey)0x41))
	{

		auto obj = SelectedObjects.back();
		auto list = kWorld->GetGroundObjects();
		for (auto listentry : list)
		{
			if (NiIsKindOf(NiPickable, listentry))
			{
				NiPickablePtr ptr = NiSmartPointerCast(NiPickable, listentry);
				if (ptr->GetSHMDPath() == obj->GetSHMDPath() || ImGui::IsKeyDown((ImGuiKey)VK_MENU)) //Select ALL Objects if alt is pressed
					SelectObject(ptr, true);
			}
		}
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) && !HasSelectedObject())
	{
		auto node = kWorld->CreateNewNPC();
		SelectObject(node, false);
	}
}
