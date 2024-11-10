#pragma once
#include <NiRTTI.h>
#include "WorldChanges.h"
#include <Data/IngameWorld/IngameWorld.h>
NiSmartPointer(ChangeNiColor);
class ChangeNiColor : public WorldChange
{
public:
	NiDeclareRTTI;
	ChangeNiColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor)
	{
		world = World;
		FromColor = FromFogColor;
		ToColor = ToFogColor;
	}
	
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(ChangeNiColor, NewObject))
		{
			ChangeNiColorPtr _NewObject = NiSmartPointerCast(ChangeNiColor, NewObject);
			ToColor = _NewObject->GetToColor();
		}
	}
	
	NiColor GetToColor() { return ToColor; }
protected:
	IngameWorldPtr world;
	NiColor FromColor;
	NiColor ToColor;
};
NiSmartPointer(FloatChange);
class FloatChange : public WorldChange
{
public:
	NiDeclareRTTI;
	FloatChange(IngameWorldPtr World, float FromDepth, float ToDepth)
	{
		world = World;
		_FromFloat = FromDepth;
		_ToFloat = ToDepth;
	}
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(FloatChange, NewObject))
		{
			FloatChangePtr _NewObject = NiSmartPointerCast(FloatChange, NewObject);
			_ToFloat = _NewObject->GetFloat();
		}
	}
	float GetFloat() { return _ToFloat; }
protected:
	IngameWorldPtr world;
	float _FromFloat;
	float _ToFloat;
};
NiSmartPointer(NodeChange);
class NodeChange : public WorldChange
{
public:
	NiDeclareRTTI;
	NodeChange(IngameWorldPtr World, NiNodePtr From, NiNodePtr To)
	{
		world = World;
		_From = From;
		_To = To;
	}
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(NodeChange, NewObject))
		{
			NodeChangePtr _NewObject = NiSmartPointerCast(NodeChange, NewObject);
			_From = _NewObject->GetNode();
		}
	}
	NiNodePtr GetNode() { return _From; }
protected:
	IngameWorldPtr world;
	NiNodePtr _From;
	NiNodePtr _To;
};


NiSmartPointer(UpdateNodePos);
class UpdateNodePos : public WorldChange
{
public:
	NiDeclareRTTI;
	UpdateNodePos(IngameWorldPtr World, std::vector<NiPickablePtr> NodeList, NiPoint3 PosChange)
	{
		world = World;
		_NodeList = NodeList;
		_PosChange = PosChange;
	}
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodePos, NewObject))
		{
			UpdateNodePosPtr _NewObject = NiSmartPointerCast(UpdateNodePos, NewObject);
			_PosChange += _NewObject->GetChange();
		}
	}
	std::vector<NiPickablePtr> GetList() { return _NodeList; }
	NiPoint3 GetChange() { return _PosChange; }
	virtual void Undo()
	{
		world->UpdatePos(_NodeList, -1.f * _PosChange, false);
	};
	virtual void Redo()
	{
		world->UpdatePos(_NodeList, _PosChange, false);
	};
	bool ExtraCheck(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodePos, NewObject))
		{
			UpdateNodePosPtr _NewObject = NiSmartPointerCast(UpdateNodePos, NewObject);
			if (_NewObject->GetList() != _NodeList)
				return true;
			else
				return false;
		}
		return true;
	}
protected:
	IngameWorldPtr world;
	std::vector<NiPickablePtr> _NodeList;
	NiPoint3 _PosChange;
};

NiSmartPointer(UpdateNodeRotation);
class UpdateNodeRotation : public WorldChange
{
public:
	NiDeclareRTTI;
	UpdateNodeRotation(IngameWorldPtr World, std::vector<NiPickablePtr> NodeList, glm::vec3 RotChange)
	{
		world = World;
		_NodeList = NodeList;
		_RotChange = RotChange;
	}
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodeRotation, NewObject))
		{
			UpdateNodeRotationPtr _NewObject = NiSmartPointerCast(UpdateNodeRotation, NewObject);
			_RotChange += _NewObject->GetChange();
		}
	}
	std::vector<NiPickablePtr> GetList() { return _NodeList; }
	glm::vec3 GetChange() { return _RotChange; }
	bool ExtraCheck(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodeRotation, NewObject))
		{
			UpdateNodeRotationPtr _NewObject = NiSmartPointerCast(UpdateNodeRotation, NewObject);
			if (_NewObject->GetList() != _NodeList)
				return true;
			else
				return false;
		}
		return true;
	}
	virtual void Undo()
	{
		world->UpdateRotation(_NodeList, -1.f * _RotChange, false);
	};
	virtual void Redo()
	{
		world->UpdateRotation(_NodeList, _RotChange, false);
	};
protected:
	IngameWorldPtr world;
	std::vector<NiPickablePtr> _NodeList;
	glm::vec3 _RotChange;
};

NiSmartPointer(UpdateNodeScale);
class UpdateNodeScale : public WorldChange
{
public:
	NiDeclareRTTI;
	UpdateNodeScale(IngameWorldPtr World, std::vector<NiPickablePtr> NodeList, float Scale)
	{
		world = World;
		_NodeList = NodeList;
		_Scale = Scale;
	}
	void Update(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodeScale, NewObject))
		{
			UpdateNodeScalePtr _NewObject = NiSmartPointerCast(UpdateNodeScale, NewObject);
			_Scale += _NewObject->GetChange();
		}
	}
	std::vector<NiPickablePtr> GetList() { return _NodeList; }
	float GetChange() { return _Scale; }
	virtual void Undo()
	{
		world->UpdateScale(_NodeList, -1.f * _Scale, false);
	};
	virtual void Redo()
	{
		world->UpdateScale(_NodeList, _Scale, false);
	};
	bool ExtraCheck(WorldChangePtr NewObject)
	{
		if (NiIsKindOf(UpdateNodeScale, NewObject))
		{
			UpdateNodeScalePtr _NewObject = NiSmartPointerCast(UpdateNodeScale, NewObject);
			if (_NewObject->GetList() != _NodeList)
				return true;
			else
				return false;
		}
		return true;
	}
protected:
	IngameWorldPtr world;
	std::vector<NiPickablePtr> _NodeList;
	float _Scale;
};


NiSmartPointer(FogChangeColor);
class FogChangeColor : public ChangeNiColor
{
public:
	NiDeclareRTTI;
	FogChangeColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor) : ChangeNiColor(World, FromFogColor, ToFogColor)
	{
	}
	virtual void Undo()
	{
		world->SetFogColor(FromColor, false);
	};
	virtual void Redo()
	{
		world->SetFogColor(ToColor, false);
	};
};
NiSmartPointer(GlobalLightNodeChange);
class GlobalLightNodeChange : public NodeChange
{
public:
	NiDeclareRTTI;
	GlobalLightNodeChange(IngameWorldPtr World, NiNodePtr From, NiNodePtr To) : NodeChange(World, From, To)
	{
	}
	virtual void Undo()
	{
		world->SetGlobalLightNode(_From, false);
	};

	virtual void Redo()
	{
		world->SetGlobalLightNode(_To, false);
	};
};
NiSmartPointer(FogChangeDepth);
class FogChangeDepth : public FloatChange
{
public:
	NiDeclareRTTI;
	FogChangeDepth(IngameWorldPtr World, float FromDepth,float ToDepth) : FloatChange(World,FromDepth,ToDepth)
	{
	}
	virtual void Undo()
	{
		world->SetFogDepth(_FromFloat,false);
	};
	
	virtual void Redo()
	{
		world->SetFogDepth(_ToFloat,false);
	};
};
NiSmartPointer(ChangeFarFrustum);
class ChangeFarFrustum : public FloatChange
{
public:
	NiDeclareRTTI;
	ChangeFarFrustum(IngameWorldPtr World, float FromDepth, float ToDepth) : FloatChange(World, FromDepth, ToDepth)
	{
	}
	virtual void Undo()
	{
		world->SetFarFrustum(_FromFloat, false);
	};

	virtual void Redo()
	{
		world->SetFarFrustum(_ToFloat, false);
	};
};
NiSmartPointer(ChangeDirectionalAmbientLightColor);
class ChangeDirectionalAmbientLightColor : public ChangeNiColor
{
public:
	NiDeclareRTTI;
	ChangeDirectionalAmbientLightColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor) : ChangeNiColor(World,FromFogColor,ToFogColor)
	{
	}
	virtual void Undo()
	{
		world->SetDirectionalLightAmbientColor(FromColor, false);
	};

	virtual void Redo()
	{
		world->SetDirectionalLightAmbientColor(ToColor, false);
	};
};
NiSmartPointer(ChangeDirectionalDiffuseLightColor);
class ChangeDirectionalDiffuseLightColor : public ChangeNiColor
{
public:
	NiDeclareRTTI;
	ChangeDirectionalDiffuseLightColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor) : ChangeNiColor(World, FromFogColor, ToFogColor)
	{
	}
	virtual void Undo()
	{
		world->SetDirectionalLightDiffuseColor(FromColor, false);
	};

	virtual void Redo()
	{
		world->SetDirectionalLightDiffuseColor(ToColor, false);
	};
};
NiSmartPointer(ChangeBackgroundColor);
class ChangeBackgroundColor : public ChangeNiColor
{
public:
	NiDeclareRTTI;
	ChangeBackgroundColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor) : ChangeNiColor(World, FromFogColor, ToFogColor)
	{
	}
	virtual void Undo()
	{
		world->SetBackgroundColor(FromColor, false);
	};

	virtual void Redo()
	{
		world->SetBackgroundColor(ToColor, false);
	};
};
NiSmartPointer(ChangeGlobalLightColor);
class ChangeGlobalLightColor : public ChangeNiColor
{
public:
	NiDeclareRTTI;
	ChangeGlobalLightColor(IngameWorldPtr World, NiColor FromFogColor, NiColor ToFogColor) : ChangeNiColor(World, FromFogColor, ToFogColor)
	{
	}
	virtual void Undo()
	{
		world->SetGlobalLight(FromColor, false);
	};

	virtual void Redo()
	{
		world->SetGlobalLight(ToColor, false);
	};
};


NiSmartPointer(AttachingNode);
class AttachingNode : public WorldChange
{
public:
	NiDeclareRTTI;
	AttachingNode(IngameWorldPtr World, NiNodePtr Node, void(IngameWorld::*ReDo)(NiNodePtr,bool), void(IngameWorld::*UnDo)(NiNodePtr, bool))
	{
		world = World;
		_Node = Node;
		_UnDo = UnDo;
		_ReDo = ReDo;
	}

	virtual void Undo()
	{
		(world->*_UnDo)(_Node, false);
	};
	virtual void Redo()
	{
		(world->*_ReDo)(_Node, false);
	};
	bool ExtraCheck(WorldChangePtr NewObject) { return true; }
protected:
	IngameWorldPtr world;
	NiNodePtr _Node;
	void(IngameWorld::* _UnDo)(NiNodePtr, bool);
	void(IngameWorld::* _ReDo)(NiNodePtr, bool);
};

NiSmartPointer(AttachingNodeList);
class AttachingNodeList : public WorldChange
{
public:
	NiDeclareRTTI;
	AttachingNodeList(IngameWorldPtr World, std::vector<NiPickablePtr> Node, void(IngameWorld::* ReDo)(std::vector<NiPickablePtr> objs, bool), void(IngameWorld::* UnDo)(std::vector<NiPickablePtr> objs, bool))
	{
		world = World;
		_Node = Node;
		_UnDo = UnDo;
		_ReDo = ReDo;
	}

	virtual void Undo()
	{
		(world->*_UnDo)(_Node, false);
	};
	virtual void Redo()
	{
		(world->*_ReDo)(_Node, false);
	};
	bool ExtraCheck(WorldChangePtr NewObject) { return true; }
protected:
	IngameWorldPtr world;
	std::vector<NiPickablePtr> _Node;
	void(IngameWorld::* _UnDo)(std::vector<NiPickablePtr> , bool);
	void(IngameWorld::* _ReDo)(std::vector<NiPickablePtr> , bool);
};