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
	
	virtual void UpdateHTD(IniFile& _InitFile, std::vector<std::vector<World::HTDHelper>>& HTD, NiPoint3 InterSect){}
	virtual void DrawInternal() {}
	virtual const char* GetName() { return "-"; }
protected:
	NiNodePtr Node;
	int BrushSize;

};