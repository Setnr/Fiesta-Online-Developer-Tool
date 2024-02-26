#pragma once
#include "FiestaScene.h"
#include <NiThread.h>
#include "PgUtil.h"

NiSmartPointer(StartScene);
class StartScene : public FiestaScene
{
public:
	StartScene();
	~StartScene() 
	{
		if(_procedure)
			NiDelete _procedure;
		if (_Thread->GetStatus() != NiThread::COMPLETE) 
		{
			NiDelete _Thread;
		}
	}
	void RunThread();
	bool IsLoaded() 
	{
		bool ret = false;
		LoadedLocK.Lock();
		ret = Loaded;
		LoadedLocK.Unlock();
		return ret;
	}
	bool UpdateScene() 
	{
		return IsLoaded();
	}
	FiestaScene* GetNewScene()
	{
		return LoadedScene;
	}
	void Draw(NiRenderer* renderer)
	{
		if (!m_spCamera)
			PgUtil::CatchCamera(BaseNode, &m_spCamera);
		NiVisibleArray m_kVisible;
		NiCullingProcess m_spCuller(&m_kVisible);
		NiDrawScene(m_spCamera, BaseNode, m_spCuller);
	}
private:
	NiThreadProcedure* _procedure;
	NiThread* _Thread;
	void LoadingFinished() 
	{
		LoadedLocK.Lock();
		Loaded = true;
		LoadedLocK.Unlock();
	}
	NiCriticalSection LoadedLocK;
	bool Loaded;

	FiestaScene* LoadedScene;
	NiCameraPtr m_spCamera;
};


class StartSceneBackgroundThread : public NiThreadProcedure
{
public:
	StartSceneBackgroundThread(StartScene* startScene) 
	{
		_Scene = startScene;
	}
	unsigned int ThreadProcedure(void* pvArg) 
	{
		_Scene->RunThread();
		return 1;
	}


private:
	StartScene* _Scene;
};
