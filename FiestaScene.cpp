#include "FiestaScene.h"
#include "PgUtil.h"

#include "FiestaOnlineTool.h"
const NiRTTI FiestaScene::ms_RTTI("FiestaScene", NULL);
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
	if (!FiestaOnlineTool::GetRotateCamera())
		return;
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
			Yaw += fHeadingDelta;
			UtilDebugString("fPitchDelta %f", fPitchDelta)

			NiMatrix3 rotation;
			rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

			UtilDebugString("Roll: %f, Pitch: %f, Yaw: %f", Roll, Pitch, Yaw);
			Camera->SetRotate(rotation);

			Camera->Update(0.0f);
		}
		return;
		NiPoint3 translate = Camera->GetTranslate();
		UtilDebugString("translate.x %f ,translate.y %f ,translate.z %f ", translate.x, translate.y, translate.z)
	}
}

