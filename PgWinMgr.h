#include <NiCamera.h>
#include <NiNode.h>
#include <NiZBufferProperty.h>
#include <NiMaterialProperty.h>
#include "PgWin.h"
class PgWinCamera : public NiCamera
{
public:
	PgWinCamera(NiRenderer* Renderer) : NiCamera()
	{
		m_spScene = NiNew NiNode;
		NiZBufferProperty* bufferProperty = NiNew NiZBufferProperty;
		bufferProperty->SetZBufferTest(true);
		bufferProperty->SetZBufferWrite(true);
		m_spScene->AttachProperty(bufferProperty);
		m_spScene->SetTranslate(NiPoint3(0.0, 0.0, 2.0));
		this->SetTranslate(NiPoint3(0.0, 0.0, 0.0));
		this->Update(0.0f);
		this->LookAtWorldPoint(NiPoint3(0.0, 0.0, 100.0), NiPoint3(0.0, -1.0, 0.0));
		this->Update(0.0f);
		m_spScene->UpdateProperties();
		m_spScene->UpdateEffects();
		NiMaterialProperty* materialpropert = NiNew NiMaterialProperty;
		materialpropert->SetEmittance(NiColor(1.0f));
		m_spScene->AttachProperty(materialpropert);
		m_spScene->UpdateProperties();
		this->UpdateWorldData();
		this->m_spRenderer = Renderer;
		this->UpdateRenderTargetBounds();
		this->Update(0.0f);
		this->UpdateWorldData();
		
	}

	void UpdateWorldData() 
	{
		NiAVObject::UpdateWorldData();
		this->WorldToCameraMatrix();
	}
	void UpdateRenderTargetBounds() 
	{
		NiRenderTargetGroup* pkRenderTargetGroup;
		float fWidth; 
		float fHeight; 
		pkRenderTargetGroup = this->m_spRenderer->GetDefaultRenderTargetGroup();
		fWidth = (float)pkRenderTargetGroup->GetWidth(0);
		fHeight = (float)pkRenderTargetGroup->GetHeight(0);
		this->m_kViewFrustum.m_fLeft = 0.5;
		this->m_kViewFrustum.m_fRight = fWidth + 0.5;
		this->m_kViewFrustum.m_fTop = fHeight - 0.5;
		this->m_kViewFrustum.m_fBottom = -0.5;
		this->m_kViewFrustum.m_fNear = 1.0;
		this->m_kViewFrustum.m_fFar = 10000.0;
		this->m_kViewFrustum.m_bOrtho = 1;
		m_spScene->SetTranslate(NiPoint3(0.0,-fHeight, this->m_kViewFrustum.m_fNear));
		
	}
	void CompactChildArray(unsigned int uiSize) 
	{
		if (uiSize < m_spScene->GetArrayCount() - m_spScene->GetChildCount()) 
		{
			m_spScene->CompactChildArray();
		}
	}

	NiPointer<NiNode> m_spScene;
	NiPointer<NiRenderer> m_spRenderer;
};
class PgWinCullingProcess : public NiCullingProcess 
{
public:
	PgWinCullingProcess(NiVisibleArray& pkVisibleSet) : NiCullingProcess(&pkVisibleSet)
	{

	}
	void Process(NiAVObject* pkObject) 
	{
		pkObject->OnVisible(*this);
	}
	void Process(const NiCamera* pkCamera, NiAVObject* pkScene, NiVisibleArray* pkVisibleSet) 
	{
		NiVisibleArray* pkSaveVisibleSet; 

		if (pkCamera && pkScene)
		{
			this->m_pkCamera = pkCamera;
			if (pkVisibleSet)
			{
				pkSaveVisibleSet = this->m_pkVisibleSet;
				this->m_pkVisibleSet = pkVisibleSet;
			}
			if (this->m_pkVisibleSet)
				pkScene->Cull(*this);
			if (pkVisibleSet)
				this->m_pkVisibleSet = pkSaveVisibleSet;
			this->m_pkCamera = 0;
		}
	}
};
NiSmartPointer(PgWinMgr);
class PgWinMgr : public NiRefObject
{
public:
	PgWinMgr(){}
	char PgInit(NiRenderer* renderer) 
	{
		m_spCamera = NiNew PgWinCamera(renderer);
		NiNode* pkNode = m_spCamera->m_spScene;
		NiPoint3 OldTranslate = pkNode->GetTranslate();
		OldTranslate.z += 10.0f;
		pkNode->SetTranslate(OldTranslate);
		this->SetScreenInfo(renderer);
		return 1;
	}
	void SetScreenInfo(NiRenderer* pkRenderer) 
	{
		if (!pkRenderer)
			return;
		NiRenderTargetGroup* pkTargetGroup = pkRenderer->GetDefaultRenderTargetGroup();
		iScreenLeftPos = 0;
		iScreenTopPos = 0;
		iScreenRightPos = pkTargetGroup->GetWidth(0) - 1;
		iScreenBottomPos = pkTargetGroup->GetHeight(0) - 1;
		iScreenWidth = pkTargetGroup->GetWidth(0);
		iScreenHeight = pkTargetGroup->GetHeight(0);
	}
	void Update() 
	{
		for (int i = 0; i < _VisibleWindows.GetSize(); i++)
			_VisibleWindows.GetAt(i)->UpdateWin();
		m_spCamera->m_spScene->Update(0.0f);
	}
	void HideWin(PgWin* pWin) 
	{
		/*Remove Window from Visible Windows and Detach Window from Scene*/
		for (int i = 0; i < _VisibleWindows.GetSize(); i++)
		{
			if (_VisibleWindows.GetAt(i) == pWin) 
			{
				pWin->GetSceneRoot()->DetachParent();
				_VisibleWindows.RemoveAt(i);
				_VisibleWindows.Compact();
				m_spCamera->m_spScene->Update(0.0f);
				return;
			}
		}
	}
	void ShowWin(PgWin* pWin) 
	{
		/*Just to catch that we dont show Windows double*/
		for (int i = 0; i < _VisibleWindows.GetSize(); i++)
			if (_VisibleWindows.GetAt(i) == pWin)
			{
				return;
			}
		/*Add Window to Array and add it to the Scene*/
		_VisibleWindows.Add(pWin);
		m_spCamera->m_spScene->AttachChild(pWin->GetSceneRoot());
		pWin->UpdateWin();
		m_spCamera->m_spScene->Update(0.0f);
	}
	void AddWindow(PgWin* pWin)
	{
		for (int i = 0; i < _Windows.GetSize(); i++)
			if (_Windows.GetAt(i) == pWin)
			{
				NiMessageBox::DisplayMessage("Tryed To Add Allready Known Window","Error");
				return;
			}

		_Windows.Add(pWin);
	}
	void CheckForHit(tagPOINT kPoint)
	{
		for (int i = 0; i < _VisibleWindows.GetSize(); i++)
		{
			_VisibleWindows.GetAt(i)->HitTest(kPoint);
		}
	}
	void CheckForClick(tagPOINT kPoint)
	{
		for (int i = 0; i < _VisibleWindows.GetSize(); i++)
		{
			_VisibleWindows.GetAt(i)->ClickTest(kPoint);
		}
	}
	NiTObjectArray<PgWinPtr> _VisibleWindows;
	NiTObjectArray<PgWinPtr> _Windows;

	void Draw(NiRenderer* renderer) 
	{
		if (this->m_spCamera) 
		{
			m_spCamera->CompactChildArray(0xA);
			renderer->ClearBuffer(0, 4);
			NiVisibleArray kWinVisible;
			kWinVisible.SetAllocatedSize(0x400u);
			PgWinCullingProcess kWinCuller(kWinVisible);
			NiDrawScene(this->m_spCamera, this->m_spCamera->m_spScene, kWinCuller, 0);
		}
	}
	NiPointer<PgWinCamera> m_spCamera;

	static int iScreenLeftPos;
	static int iScreenRightPos;

	static int iScreenWidth;
	static int iScreenHeight;

	static int iScreenTopPos;
	static int iScreenBottomPos;

};