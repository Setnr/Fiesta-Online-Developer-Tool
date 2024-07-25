#include "EditScene.h"
#include <vector>
#include <future>

#include "FiestaOnlineTool.h"
#include <NiPick.h>
#include <set>

NiRect<float> BotLeft(0.0f, 0.5f, 0.5f, 0.0f);
NiRect<float> TopLeft(0.0f, 0.5f, 1.0f, 0.5f);
NiRect<float> BotRight(0.5f, 1.0f, 0.5f, 0.0f);
NiRect<float> TopRight(0.5f, 1.0f, 1.0f, 0.5f);

void EditScene::LoadMap(MapInfo* Info) 
{
	std::lock_guard<std::mutex> lock(UpDateWorldLock);
	UpDateWorld = NiNew World(Info); 
	if(!BoundingBox)
		PgUtil::LoadNodeNifFile(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\BoundingBox.nif").c_str(), &BoundingBox, NULL);
	if(!HTDOrbNode)
	{
		PgUtil::LoadNodeNifFile(PgUtil::CreateFullFilePathFromApplicationFolder(".\\FiestaOnlineTool\\HTDCircle.nif").c_str(), &HTDOrbNode, NULL);
		NiWireframePropertyPtr ptr = NiNew NiWireframeProperty;
		ptr->SetWireframe(true);
		HTDOrbNode->AttachProperty(ptr);
	}
	if (!_Brush)
		_Brush = NiNew Brush(NiSmartPointerCast(TerrainWorld,UpDateWorld), HTDOrbNode, 1);
}

void EditScene::Draw(NiRenderer* renderer)
{
	if (!kWorld)
		return;
	renderer->SetBackgroundColor(kWorld->GetBackgroundColor());
	kWorld->GetCamera()->SetViewFrustum(kWorld->GetSkyFrustum());

	NiVisibleArray m_kVisible;
	NiCullingProcess m_spCuller(&m_kVisible);
	NiDrawScene(kWorld->GetCamera(), kWorld->GetSkyNode(), m_spCuller);

	kWorld->GetCamera()->SetViewFrustum(kWorld->GetWorldFrustum());
	NiVisibleArray m_kVisible2;
	NiCullingProcess m_spCuller2(&m_kVisible2);
	NiDrawScene(kWorld->GetCamera(), kWorld->GetWorldScene(), m_spCuller);

	if (MiddleMouseLoader.ShowInternalScene())
		MiddleMouseLoader.DrawScene(renderer);

	if (SHMDWindowLoader.ShowInternalScene())
		SHMDWindowLoader.DrawScene(renderer);
}

void EditScene::Update(float fTime) 
{
	if (UpDateWorld)
	{
		std::lock_guard<std::mutex> lock(UpDateWorldLock);
		kWorld = UpDateWorld;
		UpDateWorld = nullptr;
		Camera = kWorld->GetCamera();
		ResetCamera();
		_Brush->UpdateWorld(NiSmartPointerCast(TerrainWorld, kWorld));
	}
	if (kWorld)
	{
		if (kWorld->GetWorldScene())
			kWorld->GetWorldScene()->Update(fTime);
		if(kWorld->GetSkyNode())
		{
			NiPoint3 translate(Camera->GetTranslate());
			kWorld->GetSkyNode()->SetTranslate(translate);
			kWorld->GetSkyNode()->Update(fTime / 10.f);
		}
	}
	if(MiddleMouseLoader.ShowInternalScene())
		MiddleMouseLoader.Update(fTime);

	if (SHMDWindowLoader.ShowInternalScene())
		SHMDWindowLoader.Update(fTime);
}

void EditScene::SelectObject() {
	NiPoint3 kOrigin, kDir;
	long X, Y;
	FiestaOnlineTool::GetMousePosition(X, Y);
	if (Camera->WindowPointToRay(X, Y, kOrigin, kDir))
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

			std::set<NiPickable*> PossibleObjects;

			for (int i = 0; i < results.GetSize(); i++)
			{
				auto record = results.GetAt(i);
				auto obj = record->GetAVObject();
				if (NiIsKindOf(NiPickable, obj))
				{
					PossibleObjects.insert((NiPickable*)obj);
				}
				else
				{
					//We only earch Upwards, 
					//if we search Downwards we can click the terrain and select the first 
					//NiPickable which must not be the actually targeted Node
					//Picking only works when compiled with the define of PICKABLEOBJECTS being true
					while (obj = obj->GetParent())
					{
						if (NiIsKindOf(NiPickable, obj))
						{
							PossibleObjects.insert((NiPickable*)obj);
							break;
						}
					}
				}
			}
			if (auto search = PossibleObjects.find(SelectedObj); search == PossibleObjects.end())
			{
				if (PossibleObjects.size() != 0)
				{
					SelectedObj = *PossibleObjects.begin();
					float angle, x, y, z;
					SelectedObj->GetRotate().ExtractAngleAndAxis(angle, x, y, z);
					SelectedObjAngels = glm::degrees(glm::vec3{ eulerAngles(glm::angleAxis((float)angle, glm::vec3(x, y, z))) });
					UpdateGeneralInfoNode((NiNode*)&*SelectedObj);
				}
			}
		}
	}
}

void EditScene::ResetCamera() 
{
	LookAndMoveAtWorldPoint(kWorld->GetSpawnPoint());
	Camera->Update(0.0f);
}

void EditScene::UpdateGeneralInfoNode(NiNodePtr Node) 
{
	if (GeneralInfoNode && NiIsKindOf(NiPickable, GeneralInfoNode))
	{
		NiPickable* ptr = (NiPickable*) & *GeneralInfoNode;
		ptr->HideBoundingBox(BoundingBox);
	}
	GeneralInfoNode = Node;
	if(Node && NiIsKindOf(NiPickable, Node))
	{
		NiPickable* ptr = (NiPickable*)&*GeneralInfoNode;
		ptr->ShowBoundingBox(BoundingBox);
	}
}
