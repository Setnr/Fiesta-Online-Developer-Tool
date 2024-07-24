#pragma once
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "../IniFile.h"
#include "../IngameWorld.h"
NiSmartPointer(Brush);
class Brush : public NiRefObject 
{
public:
	Brush(NiNodePtr HTDOrbNode,int BrushSize) : Node(HTDOrbNode), BrushSize(BrushSize) {}
	virtual BrushPtr Draw() {}
	
	virtual void Update(TerrainWorldPtr kWorld){}
	virtual void DrawInternal() {}
	virtual const char* GetName() { return "-"; }
	virtual void Show(bool Show){}
	virtual void Init() {}
	void SetIntersect(NiPoint3 Intersect) { _Intersect = Intersect; }
	virtual void MouseClick() { InitMouse = true; }
	void MouseRelease() { InitMouse = false; }
	NiPoint3 GetIntersect() { return _Intersect; }
protected:
	NiNodePtr Node;
	int BrushSize;
	bool InitMouse = false;
	NiPoint3 _Intersect;
};

class HTDBrush : public Brush
{
public:
	HTDBrush(NiNodePtr HTDOrbNode, int BrushSize) : Brush(HTDOrbNode, BrushSize){}
	virtual BrushPtr Draw();
};

class ColorBrush : public Brush
{
public:
	ColorBrush(NiNodePtr HTDOrbNode, int BrushSize) : Brush(HTDOrbNode, BrushSize) {}
	virtual BrushPtr Draw();
};