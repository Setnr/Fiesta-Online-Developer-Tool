#include <NiCamera.h>
#include <NiNode.h>
#include <NiZBufferProperty.h>
#include <NiMaterialProperty.h>
class PgWinCamera : public NiCamera
{
public:
	PgWinCamera(NiRenderer* Renderer) : NiCamera()
	{
		m_spScene = NiNew NiNode;
		//m_spScene->AttachChild(this);
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
class PgWinMgr
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

	}

	void ShowWin(NiNode* SceneRootNode) 
	{
		m_spCamera->m_spScene->AttachChild(SceneRootNode);
		SceneRootNode->UpdateProperties();
		SceneRootNode->UpdateEffects();
		m_spCamera->m_spScene->Update(0.0f);
	}

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
	/*CNSTimer_MarkCheck m_kMarkCheck;
	NiPointer<NiNode> m_spFullScreenEffectNode;*/
	NiPointer<PgWinCamera> m_spCamera;
	/*PgTList<NiPointer<PgWin> > m_kWinList;
	PgTList<PgWin*> m_kShowList;
	PgTQueue<PgWinMgr::PgWinMsg> m_kMsgQ;
	NiPointer<PgWin> m_pkModalWin;
	PgTList<PgWin*> m_kStackModalWin;
	PgCursor* m_pkCursor;
	NiPointer<NiInputMouse> m_spMouse;
	NiPointer<NiInputKeyboard> m_spKeyboard;
	bool m_bEnableInput;
	NiTList<PgWin*> m_kPopupWinStack;
	PgWin* m_pkToolTipWin;
	PgWin* m_pkFirstHitWin;
	NiPointer<NiTexturePalette> m_spTexturePalette;
	bool m_bHitWin;
	bool m_bTopWinScroll;*/
};