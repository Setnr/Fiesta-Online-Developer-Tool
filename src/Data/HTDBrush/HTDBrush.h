#pragma once
#include <NiNode.h>
#include "ImGui/imgui.h"
#include "../IniFile.h"
#include "../IngameWorld.h"
NiSmartPointer(HTDBrush);
class HTDBrush : public NiRefObject 
{
public:
	HTDBrush(NiNodePtr HTDOrbNode,int BrushSize) : Node(HTDOrbNode), BrushSize(BrushSize) {}
	virtual HTDBrushPtr Draw();
	
	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<TerrainWorld::HTDHelper>>& HTD, NiPoint3 InterSect){}
	virtual void DrawInternal() {}
	virtual const char* GetName() { return "-"; }
	virtual void Show(bool Show){}
	virtual void Init() {}
protected:
	NiNodePtr Node;
	int BrushSize;
};