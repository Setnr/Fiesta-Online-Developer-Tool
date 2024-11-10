#include "SHMDMode.h"
#include <Scene/ScreenElements/LuaElement/LuaElement.h>
#include <Scene/ScreenElements/LoadObject/AddSingleObject.h>
#include <Scene/ScreenElements/LoadObject/AddMultipleObject.h>
#include <Scene/ScreenElements/LoadObject/ReplaceObjects.h>

NiImplementRTTI(SHMDMode, EditMode);

void SHMDMode::Draw()
{
	EditMode::Draw();
	DrawGizmo(); 
}

void SHMDMode::Update(float fTime)
{
}

void SHMDMode::UpdateScale(float Scale) 
{
	kWorld->UpdateScale(GetSelectedNodes(), Scale);
}  

void SHMDMode::ProcessInput() 
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) 
	{
		SelectObject(kWorld->PickObject(), ImGui::IsKeyDown((ImGuiKey)VK_CONTROL));
	}
	if (ImGui::IsKeyDown((ImGuiKey)VK_CONTROL)) 
	{
		if (ImGui::IsKeyPressed((ImGuiKey)0x45)) //e
			OperationMode = ImGuizmo::OPERATION::ROTATE;
		if (ImGui::IsKeyPressed((ImGuiKey)0x57)) //w
			OperationMode = ImGuizmo::OPERATION::TRANSLATE;
		if (ImGui::IsKeyPressed((ImGuiKey)0x44)) //d
			SnapMovement = !SnapMovement;
		if (HasSelectedObject() && ImGui::IsKeyPressed((ImGuiKey)0x43))
			CopyObject = SelectedObjects.back();
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
		kWorld->RemoveObject(SelectedObjects);
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
				if (ptr->GetSHMDPath() == obj->GetSHMDPath())
					SelectObject(ptr, true);
			}
		}
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
	{
		if(NiIsKindOf(EditorScene,_Scene))
		{
			bool HasElement = false;
			for (auto element : ScreenElements)
			{
				if (NiIsKindOf(LuaElement, element)) {
					LuaElementPtr ptr = NiSmartPointerCast(LuaElement, element);
					if (ptr->GetFileName() == "EditorELements/SHMDMiddleMouse.lua")
						HasElement = true;
				}
			}
			if(!HasElement)
				ScreenElements.push_back(NiNew LuaElement(NiSmartPointerCast(EditorScene, _Scene), "EditorELements/SHMDMiddleMouse.lua", ImGui::GetIO().MousePos));
		}
	}
}

void SHMDMode::DrawGizmo() 
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
		kWorld->UpdatePos(SelectedObjects, ChangeingPosition - PreviousPos);
	if (tmpRotation != glm::vec3{ 0,0,0 })
		kWorld->UpdateRotation(SelectedObjects, tmpRotation);

}

void SHMDMode::CreateAddElement(std::string name) 
{
	if (name == "Moveable Object") 
	{
		auto AddFunc = &IngameWorld::AddObject;
		for (auto element : ScreenElements) 
		{
			if (NiIsKindOf(AddMultipleObject, element))
				return;
		}
		ScreenElements.push_back(NiNew AddMultipleObject(kWorld, AddFunc, kWorld->GetWorldPoint()));
	}
	else if ("Replace Object") 
	{
		auto nodes = GetSelectedNodes();
		for (auto element : ScreenElements)
		{
			if (NiIsKindOf(ReplaceObjects, element))
				return;
		}
		ScreenElements.push_back(NiNew ReplaceObjects(kWorld, nodes));
	}
	else
	{
		void (IngameWorld:: * AddFunc)(NiNodePtr, bool);
		if (name == "Sky") 
		{
			AddFunc = &IngameWorld::AddSky;
		}
		else if (name == "Water")
		{
			AddFunc = &IngameWorld::AddWater;
		}
		else if (name == "Ground") 
		{
			AddFunc = &IngameWorld::AddGroundObject;
		}else
		{
			return;
		}
		for (auto element : ScreenElements)
		{
			if (NiIsKindOf(AddSingleObject, element))
				return;
		}
		ScreenElements.push_back(NiNew AddSingleObject(kWorld, AddFunc,NiPoint3::ZERO));
	}
}