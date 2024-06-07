#pragma once
#include <NiCamera.h>
#include <NiRect.h>

NiSmartPointer(NiViewPort);
class NiViewPort : public NiRefObject
{
public:
	NiViewPort(NiRect<float> port) 
	{
		_Port = port;
		_Camera = NiNew NiCamera;
		auto m_bCameraOrtho = false;
		NiFrustum SkyFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 6000.0, m_bCameraOrtho);
		NiFrustum WorldFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 6000.0, m_bCameraOrtho);


		float m_fCameraFOV = 50.0f;
		float fTop = m_fCameraFOV / 180.0 * 0.5 * NI_PI;
		float fTopa = tan(fTop);
		float v4 = fTopa;
		float fRight = fTopa;
		float v5 = 1600; /*TODO DYNAMIC*/
		float v6 = 900;
		float fTopb;
		if (900 >= (double)1600)
		{
			fTopb = v4 * (v6 / v5);
			v4 = fTopb;
		}
		else
		{
			fRight = v5 / v6 * v4;
		}
		float fTopc = -fRight;
		float v7 = fTopc;
		WorldFrustum.m_fLeft = fTopc;
		WorldFrustum.m_fRight = fRight;
		WorldFrustum.m_fTop = v4;
		float fTopd = -v4;
		WorldFrustum.m_fBottom = fTopd;

		_Camera->SetViewFrustum(WorldFrustum); //World::SetCameraFOV

		Pitch = 1.57f * 2.0f;
		Yaw = -1.57f;
		Roll = 1.57f;
		NiMatrix3 rotation;
		rotation.FromEulerAnglesXYZ(Roll, Yaw, Pitch);

		_Camera->SetRotate(rotation);
		_Camera->Update(0.0f);
	}
	NiCameraPtr GetCamera() { return _Camera; }
	NiRect<float> GetScreenSpace() { return _Port; }
	void SetScreenSpace(NiRect<float> port) { _Port = port; }

private:
	NiRect<float> _Port;
	NiCameraPtr _Camera;
	float Pitch, Yaw, Roll;
};