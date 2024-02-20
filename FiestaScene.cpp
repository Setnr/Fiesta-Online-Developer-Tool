#include "FiestaScene.h"
#include "PgUtil.h"

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