#pragma once
#include <NiNode.h>
#include "PgWinObj.h"
#include "PgUtil.h"
NiSmartPointer(PgWin);
class PgWin : public NiRefObject
{
	NiDeclareRootRTTI(PgWin);
public:
	NiNode* GetSceneRoot() { return _SceneRoot; }
	void UpdateWin() 
	{
		if(ResetClick.GetSize() > 0)
		{
			for (int i = 0; i < ResetClick.GetSize(); i++)
			{
				ResetClick.GetAt(i)->ResetClick();
			}
			ResetClick.RemoveAll();
		}
	}
	bool HitTest(tagPOINT kPoint)
	{
		if (_Panel->HitTest(kPoint))
		{
			for (int i = 0; i < _VisibleObjects.GetSize(); i++)
			{ 
				PgWinObjPtr obj = _VisibleObjects.GetAt(i);
				if (obj->HitTest(kPoint))
					obj->HandleHit();
			}
			return true;
		}
		return false;
	}
	bool ClickTest(tagPOINT kPoint)
	{
		if (_Panel->HitTest(kPoint))
		{
			for (int i = 0; i < _VisibleObjects.GetSize(); i++)
			{
				PgWinObjPtr obj = _VisibleObjects.GetAt(i);
				if (obj->HitTest(kPoint))
				{
					if(obj->Click(this))
						ResetClick.Add(obj);
				}
			}
			return true;
		}
		return false;
	}
	void OnCommand(unsigned int ClickID) 
	{
		UtilDebugString("Clicked Button with Enum Index %i", ClickID);
	}
protected:
	PgWin(){}
	void CreateButton(NiNode* SceneObj,unsigned int ClickID, const char* RegularName, const char* Hovername, const char* ClickName) 
	{
		PgWinButtonObjPtr SelectButton = NiNew PgWinButtonObj;
		SelectButton->AddButton(_SceneRoot, ClickID,RegularName, Hovername, ClickName);
		_SubObjects.Add((PgWinObjPtr)SelectButton);
		_VisibleObjects.Add((PgWinObjPtr)SelectButton);
	}
	void CreateHiddenButton(NiNode* SceneObj, const char* RegularName, const char* Hovername, const char* ClickName)
	{
		PgWinButtonObjPtr SelectButton = NiNew PgWinButtonObj;
		SelectButton->AddHiddenButton(_SceneRoot, 0xFFFFFFFF,RegularName, Hovername, ClickName);
		_SubObjects.Add((PgWinObjPtr)SelectButton);
	}
	void CreatePanel(NiNode* SceneObj, const char* RegularName) 
	{
		PgWinObjPtr Panel = NiNew PgWinObj;
		Panel->AddObj(_SceneRoot, RegularName);
		_Panel = Panel;
	}
	void CreateFrame(NiNode* SceneObj, const char* RegularName)
	{
		PgWinObjPtr Frame = NiNew PgWinObj;
		Frame->AddObj(_SceneRoot, RegularName);
		_Frame = Frame;
	}
	
	NiNodePtr _SceneRoot;
	PgWinObjPtr _Panel;
	PgWinObjPtr _Frame;
	NiTObjectArray<PgWinObjPtr> _VisibleObjects;
	NiTObjectArray<PgWinObjPtr> _SubObjects;
	NiTObjectArray<PgWinObjPtr> ResetClick;
};

class LoginServerList : public PgWin 
{
	enum Button : unsigned int 
	{
		SelectWorld,
		SelectServer1
	};
public:
	LoginServerList() 
	{
		_SceneRoot = PgUtil::LoadNifFile("E:\\Coding\\Core\\Core Fiesta\\resmenu\\account\\LoginServerList.nif", NULL);
		CreatePanel(_SceneRoot, "Panel01");
		CreateFrame(_SceneRoot, "Frame");// Das geht nicht
		CreateButton(_SceneRoot, Button::SelectWorld, "SelectNor", "SelectOver", "SelectClick");
		CreateButton(_SceneRoot, Button::SelectServer1, "ServerNameNor_01", "ServerNameOver_01", "ServerNameClick_01");
		CreateServerStatusSprite(_SceneRoot, "ServerConGood_01", "ServerConNormal_01", "ServerConBusy_01", "ServerConCheck_01", "ServerConClosed_01");
				
		for (int i = 2; i < 9; i++) 
		{
			char NameNor[512];
			NiSprintf(NameNor, sizeof(NameNor), "ServerNameNor_0%i", i);
			char NameOver[512];
			NiSprintf(NameOver, sizeof(NameOver), "ServerNameOver_0%i", i);
			char NameClick[512];
			NiSprintf(NameClick, sizeof(NameClick), "ServerNameClick_0%i", i);

			CreateHiddenButton(_SceneRoot, NameNor, NameOver, NameClick);

			char Good[512];
			NiSprintf(Good, sizeof(Good), "ServerConGood_0%i", i);
			char Normal[512];
			NiSprintf(Normal, sizeof(Normal), "ServerConNormal_0%i", i);
			char Busy[512];
			NiSprintf(Busy, sizeof(Busy), "ServerConBusy_0%i", i);
			char Check[512];
			NiSprintf(Check, sizeof(Check), "ServerConCheck_0%i", i);
			char Close[512];
			NiSprintf(Close, sizeof(Close), "ServerConClosed_0%i", i);
			CreateServerStatusSprite(_SceneRoot, Good, Normal, Busy, Check, Close);

		}
		_SceneRoot->UpdateProperties();
		_SceneRoot->UpdateEffects();
		_SceneRoot->Update(0.0f);
	}

	void CreateServerStatusSprite(NiNode* SceneObj, const char* Good, const char* Normal, const char* Busy, const char* Check, const char* Closed)
	{
		PgWinButtonObjPtr GoodSprite = NiNew PgWinButtonObj;
		PgWinButtonObjPtr NorSprite = NiNew PgWinButtonObj;
		PgWinButtonObjPtr BusySprite = NiNew PgWinButtonObj;
		PgWinButtonObjPtr CheckSprite = NiNew PgWinButtonObj;
		PgWinButtonObjPtr ClosedSprite = NiNew PgWinButtonObj;
		GoodSprite->AddHiddenSprite(_SceneRoot, Good);
		NorSprite->AddHiddenSprite(_SceneRoot, Normal);
		BusySprite->AddHiddenSprite(_SceneRoot, Busy);
		CheckSprite->AddHiddenSprite(_SceneRoot, Check);
		ClosedSprite->AddHiddenSprite(_SceneRoot, Closed);
		_SubObjects.Add((PgWinObjPtr)GoodSprite);
		_SubObjects.Add((PgWinObjPtr)NorSprite);
		_SubObjects.Add((PgWinObjPtr)BusySprite);
		_SubObjects.Add((PgWinObjPtr)CheckSprite);
		_SubObjects.Add((PgWinObjPtr)ClosedSprite);
	}
	NiTObjectArray<PgWinObjPtr> _ServerStatus;
};