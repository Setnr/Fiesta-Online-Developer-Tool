#pragma once
#include <NiTriShape.h>
#include "PgUtil.h"
NiSmartPointer(PgWinObj);
NiSmartPointer(PgWinHoverObj);
NiSmartPointer(PgWinButtonObj);
class PgWin;
class PgWinObj : public NiRefObject
{
	NiDeclareRootRTTI(PgWinObj);
public:
	PgWinObj() {  }
	bool AddObj(NiNode* SceneObj, const char* RegularName)
	{
		ParentObj = SceneObj;
		NiAVObject* obj = SceneObj->GetObjectByName(RegularName);
		if (!obj->IsKindOf(&NiTriShape::ms_RTTI))
			return 0;

		_RegularSprite = (NiTriShape*)obj;
		return 1;
	}
	bool AddHiddenSprite(NiNode* SceneObj, const char* RegularName)
	{
		ParentObj = SceneObj;
		NiAVObject* obj = SceneObj->GetObjectByName(RegularName);
		if (!obj->IsKindOf(&NiTriShape::ms_RTTI))
			return 0;
		_RegularSprite = (NiTriShape*)obj;
		_RegularSprite->DetachParent();
		return 1;
	}
	bool HitTest(tagPOINT kPoint)
	{
		int Left, Top;
		unsigned int Width, Height;
		PgUtil::CreateScreenGeometryDataFromTriShapeData(_RegularSprite->GetModelData(), &Left, &Top, &Width, &Height);
		//UtilDebugString("kPoint.x: %i, kPoint.y: %i, Left: %i, Top: %i, Width: %i, Height: %i", kPoint.x, kPoint.y,Left,Top,Width, Height)
		if (kPoint.x >= Left && kPoint.x <= Left + Width && kPoint.y >= Top && kPoint.y <= Top + Height)
		{
			return true;
		}
		if(_RegularSprite->GetParent())
			return false;
		this->HandleLeave();
		return false;
	}
	virtual void HandleHit() {}
	virtual void HandleLeave() {}
	virtual void ResetClick() {  }
	virtual bool Click(PgWin* Window) { return false; }
protected:
	NiNode* ParentObj;
	NiTriShapePtr _RegularSprite;
};

class PgWinHoverObj : public PgWinObj
{
	NiDeclareRootRTTI(PgWinHoverObj);
public:
	PgWinHoverObj(){}
	bool AddHover(NiNode* SceneObj, const char* RegularName, const char* Hovername)
	{
		NiAVObject* obj = SceneObj->GetObjectByName(Hovername);
		if (!obj->IsKindOf(&NiTriShape::ms_RTTI))
			return 0;
		_HoveredSprite = (NiTriShape*)obj;
		_HoveredSprite->DetachParent();
		return this->AddObj(SceneObj, RegularName);
	}

	void HandleHit()
	{
		if (_HoveredSprite->GetParent())
			return;
		ParentObj->AttachChild(_HoveredSprite);
		_RegularSprite->DetachParent();
		
	}
	void HandleLeave() 
	{
		_HoveredSprite->DetachParent();
		ParentObj->AttachChild(_RegularSprite);
	}

protected:
	NiTriShapePtr _HoveredSprite;
};

class PgWinButtonObj : public PgWinHoverObj
{
	NiDeclareRootRTTI(PgWinButtonObj);
public:
	PgWinButtonObj() {}
	bool AddButton(NiNode* SceneObj,unsigned int ClickID , const char* RegularName, const char* Hovername, const char* ClickName)
	{
		NiAVObject* obj = SceneObj->GetObjectByName(ClickName);
		this->ClickID = ClickID;
		if (!obj->IsKindOf(&NiTriShape::ms_RTTI)) 
			return 0;
		_ClickedSprite = (NiTriShape*)obj;
		_ClickedSprite->DetachParent();
		return this->AddHover(SceneObj, RegularName, Hovername);
	}
	bool AddHiddenButton(NiNode* SceneObj, unsigned int ClickID ,const char* RegularName, const char* Hovername, const char* ClickName)
	{
		if (!AddButton(SceneObj, ClickID ,RegularName, Hovername, ClickName))
		{
			UtilDebugString("Failed To Add Hidden Button");
			return 0;
		}
		_RegularSprite->DetachParent();
		return 1;
	}
	bool Click(PgWin* pWin);
	void ResetClick() 
	{
		_ClickedSprite->DetachParent();
		PgWinHoverObj::HandleHit();
	}
protected:
	NiTriShapePtr _ClickedSprite;
	unsigned int ClickID;
};

class PgServerStatusSprite : public PgWinObj 
{
	
};