#include "lua.hpp"
#include "Logger.h"
#include <Scene/EditorScene/Modes/SHMDMode.h>
#include <Scene/EditorScene/Modes/SHBDMode.h>

int LogFromLua(lua_State* Script)
{
	lua_getglobal(Script, "WindowName");
	if (!lua_isstring(Script, 2))
	{
		LogError("WindowName-Variable missing");
		return 0;
	}
	if (lua_isstring(Script, 1))
	{
		LogLua(lua_tostring(Script, 2), lua_tostring(Script, 1));
	}
	return 0;
}
int ImGuiText(lua_State* Script)
{
	if (lua_isstring(Script, 1))
		ImGui::Text(lua_tostring(Script, 1));
	return 0;
}
int ImGuiSameLine(lua_State* Script)
{
	ImGui::SameLine();
	return 0;
}
int ImGuiButton(lua_State* Script)
{
	if (lua_isstring(Script, 1))
		if (ImGui::Button(lua_tostring(Script, 1)))
			lua_pushboolean(Script, true);
		else
			lua_pushboolean(Script, false);
	else
		lua_pushboolean(Script, false);
	return 1;
}
int ImGuiStartTabWindow(lua_State* Script)
{
	if (lua_isstring(Script, 1))
		if (ImGui::BeginTabItem(lua_tostring(Script, 1)))
			lua_pushboolean(Script, true);
		else
			lua_pushboolean(Script, false);
	else
		lua_pushboolean(Script, false);
	return 1;
}
int ImGuiEndTabWindow(lua_State* Script)
{
	ImGui::EndTabItem();
	return 0;
}
int ImGuiBeginTabBar(lua_State* Script)
{
	if (lua_isstring(Script, 1))
		if (ImGui::BeginTabBar(lua_tostring(Script, 1)))
			lua_pushboolean(Script, true);
		else
			lua_pushboolean(Script, false);
	else
		lua_pushboolean(Script, false);
	return 1;
}
int ImGuiEndTabBar(lua_State* Script)
{
	ImGui::EndTabBar();
	return 0;
}
int ImGuiNewLine(lua_State* Script)
{
	ImGui::NewLine();
	return 0;
}
int ImGuiPopTree(lua_State* Script) 
{
	ImGui::TreePop();
	return 0;
}
int ImGuiTreeView(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		auto ptr = lua_tointeger(Script, 1);
		NiNodePtr Node = (NiNode*)ptr;
		std::string Name = "##" + std::to_string((int)&*Node);
		bool ExpandNode = ImGui::TreeNode(Name.c_str());
		lua_pushboolean(Script, ExpandNode);
		return 1;
	}
	return 0;
}
int ImGuiSelectable(lua_State* Script) 
{
	if (lua_isstring(Script, 1)) 
	{
		std::string Name = lua_tostring(Script, 1);
		bool selected = ImGui::Selectable(Name.c_str()); 
		lua_pushboolean(Script, selected);
		return 1;
	}
	return 0;
}
int GetChild(lua_State* Script) 
{
	if(lua_isinteger(Script,1)&& lua_isinteger(Script,2))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		int idx = lua_tointeger(Script, 2);
		NiAVObjectPtr child = Node->GetAt(idx);
		if (child && NiIsKindOf(NiNode, child))
		{
			lua_pushinteger(Script,(long long)&*child);
			return 1;
		}
	}
	return 0;
}
int GetChildCount(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		int count = Node->GetChildCount();
		lua_pushinteger(Script, count);
		return 1;
	}
	return 0;
}
int HasChilds(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		int count = Node->GetChildCount();
		if(count)
			lua_pushboolean(Script, true);
		else
			lua_pushboolean(Script, false);
		return 1;
	}
	return 0;
}
int GetNodeName(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		std::string Name = "-";
		if(Node->GetName())
			Name = Node->GetName();
		lua_pushstring(Script, Name.c_str());
		return 1;
	}
	return 0;
}
int GetWorldNode(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		LuaElementPtr element = (LuaElement*)lua_tointeger(Script, 1);
		auto scene = element->GetScene();
		if(scene)
		{
			auto world = scene->GetWorld();
			if(world)
			{
				auto Node = world->GetWorldNode();
				lua_pushinteger(Script, (long long)&*Node);
				return 1;
			}
		}
	}
	return 0;
}
int GetIngameWorld(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		LuaElementPtr element = (LuaElement*)lua_tointeger(Script, 1);
		auto scene = element->GetScene();
		if (scene)
		{
			auto world = scene->GetWorld();
			if (world)
			{
				lua_pushinteger(Script, (long long)&*world);
				return 1;
			}
		}
	}
	return 0;
}
int MakeNoMove(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		LuaElement* element = (LuaElement*)lua_tointeger(Script, 1);
		if (element)
		{
			element->AddFlag(ImGuiWindowFlags_NoMove);
		}
	}
	return 0;
}
int MakeNoCollapse(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		LuaElement* element = (LuaElement*)lua_tointeger(Script, 1);
		if (element)
		{
			element->AddFlag(ImGuiWindowFlags_NoCollapse);
		}
	}
	return 0;
}
int DragFloat(lua_State* Script) 
{
	if (lua_isnumber(Script, 1)&& lua_isstring(Script,2)
		&& lua_isnumber(Script, 3) && lua_isnumber(Script, 4) && lua_isnumber(Script, 5))	
	{
		float number = lua_tonumber(Script, 1);
		auto name = lua_tostring(Script, 2);
		float Speed = lua_tonumber(Script, 3);
		float Min = lua_tonumber(Script, 4);
		float Max = lua_tonumber(Script, 5);

		bool changed = ImGui::DragFloat(name, &number,Speed,Min,Max);
		lua_pushboolean(Script, changed);
		lua_pushnumber(Script, number);
		return 2;
	}
	return 0;
}
int ColorEdit3(lua_State* Script) 
{
	if (lua_isstring(Script, 1) && lua_isnumber(Script,2) && lua_isnumber(Script, 3) && lua_isnumber(Script, 4))
	{
		auto name = lua_tostring(Script, 1);
		float Color[3];
		Color[0] = lua_tonumber(Script, 2);
		Color[1] = lua_tonumber(Script, 3);
		Color[2] = lua_tonumber(Script, 4);
		bool changed = ImGui::ColorEdit3(name, Color);
		
		lua_pushboolean(Script, changed);
		lua_pushnumber(Script, Color[0]);
		lua_pushnumber(Script, Color[1]);
		lua_pushnumber(Script, Color[2]);
		return 4;
	}
	return 0;
}  
int CompactChildArray(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		Node->CompactChildArray();
		return 0;
	}
	return 0;
}
int RemoveChild(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2))
	{
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 1);
		NiNodePtr Child = (NiNode*)lua_tointeger(Script, 2);
		Node->DetachChild(Child);
		return 0;
	}
	return 0;
}
int CollapsingHeader(lua_State* Script)
{
	if (lua_isstring(Script, 1)) 
	{
		auto Name = lua_tostring(Script, 1);
		bool state = ImGui::CollapsingHeader(Name);
		lua_pushboolean(Script, state);
		return 1;
	}
	return 0;
}
int GetSkyNode(lua_State* Script) 
{
	if (lua_isinteger(Script, 1)) 
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world) 
		{
			auto vector = world->GetSkyNodeList();
			lua_newtable(Script);
			for (int i = 0; i < vector.size(); i++) 
			{
				lua_pushinteger(Script, i);
				lua_pushinteger(Script, (long long) &*vector[i]);
				lua_settable(Script, -3);
			}
			return 1;
		}
	}
	return 0;
}
int RemoveSky(lua_State* Script) 
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2)) 
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 2);
		if (world && Node) 
		{
			world->RemoveSky(Node);
		}
	}
	return 0;
}
int GetWaterNode(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto vector = world->GetWaterNodeList();
			lua_newtable(Script);
			for (int i = 0; i < vector.size(); i++)
			{
				lua_pushinteger(Script, i);
				lua_pushinteger(Script, (long long)&*vector[i]);
				lua_settable(Script, -3);
			}
			return 1;
		}
	}
	return 0;
}
int RemoveWater(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 2);
		if (world && Node)
		{
			world->RemoveWater(Node);
		}
	}
	return 0;
}
int GetGroundObjectNode(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto vector = world->GetGlobalGroundObjectList();
			lua_newtable(Script);
			for (int i = 0; i < vector.size(); i++)
			{
				lua_pushinteger(Script, i);
				lua_pushinteger(Script, (long long)&*vector[i]);
				lua_settable(Script, -3);
			}
			return 1;
		}
	}
	return 0;
}
int RemoveGroundObject(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiNodePtr Node = (NiNode*)lua_tointeger(Script, 2);
		if (world && Node)
		{
			world->RemoveGroundObject(Node);
		}
	}
	return 0;
}
int GetGlobalLight(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetGlobalLight();

			lua_pushnumber(Script, col.r);
			lua_pushnumber(Script, col.g);
			lua_pushnumber(Script, col.b);
			return 3;
		}
	}
	return 0;
}
int SetGlobalLight(lua_State* Script)
{
	if (lua_isinteger(Script, 1)
		&& lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3)
		&& lua_isnumber(Script, 4))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiColor col(lua_tonumber(Script, 2), lua_tonumber(Script, 3), lua_tonumber(Script, 4));
		world->SetGlobalLight(col);
		return 0;
	}
	return 0;
}
int GetFogColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetFogColor();

			lua_pushnumber(Script, col.r);
			lua_pushnumber(Script, col.g);
			lua_pushnumber(Script, col.b);
			return 3;
		}
	}
	return 0;
}
int SetFogColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1)
		&& lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3)
		&& lua_isnumber(Script, 4))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiColor col(lua_tonumber(Script, 2), lua_tonumber(Script, 3), lua_tonumber(Script, 4));
		world->SetFogColor(col);
		return 0;
	}
	return 0;
}
int GetFogDepth(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetFogDepth();
			lua_pushnumber(Script, col);
			return 1;
		}
	}
	return 0;
}
int SetFogDepth(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isnumber(Script,2))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = (float)lua_tonumber(Script, 2);
			world->SetFogDepth(col);
			return 0;
		}
	}
	return 0;
}
int GetBackgroundColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetBackgroundColor();

			lua_pushnumber(Script, col.r);
			lua_pushnumber(Script, col.g);
			lua_pushnumber(Script, col.b);
			return 3;
		}
	}
	return 0;
}
int SetBackgroundColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1)
		&& lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3)
		&& lua_isnumber(Script, 4))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiColor col(lua_tonumber(Script, 2), lua_tonumber(Script, 3), lua_tonumber(Script, 4));
		world->SetBackgroundColor(col);
		return 0;
	}
	return 0;
}
int GetFrustum(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetFarFrustum();
			lua_pushnumber(Script, col);
			return 1;
		}
	}
	return 0;
}
int SetFrustum(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isnumber(Script, 2))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = (float)lua_tonumber(Script, 2);
			world->SetFarFrustum(col);
			return 0;
		}
	}
	return 0;
}
int GetAmbientLightColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetDirectionalLightAmbientColor();

			lua_pushnumber(Script, col.r);
			lua_pushnumber(Script, col.g);
			lua_pushnumber(Script, col.b);
			return 3;
		}
	}
	return 0; 
}
int SetAmbientLightColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1)
		&& lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3)
		&& lua_isnumber(Script, 4))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiColor col(lua_tonumber(Script, 2), lua_tonumber(Script, 3), lua_tonumber(Script, 4));
		world->SetDirectionalLightAmbientColor(col);
		return 0;
	}
	return 0;
}
int GetDiffuseLightColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		if (world)
		{
			auto col = world->GetDirectionalLightDiffuseColor();

			lua_pushnumber(Script, col.r);
			lua_pushnumber(Script, col.g);
			lua_pushnumber(Script, col.b);
			return 3;
		}
	}
	return 0;
}
int SetDiffuseLightColor(lua_State* Script)
{
	if (lua_isinteger(Script, 1)
		&& lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3)
		&& lua_isnumber(Script, 4))
	{
		IngameWorldPtr world = (IngameWorld*)lua_tointeger(Script, 1);
		NiColor col(lua_tonumber(Script, 2), lua_tonumber(Script, 3), lua_tonumber(Script, 4));
		world->SetDirectionalLightDiffuseColor(col);
		return 0;
	}
	return 0;
}
int GetCurrentScenePtr(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		LuaElementPtr element = (LuaElement*)lua_tointeger(Script, 1);
		auto scene = element->GetScene();
		if (scene) 
		{
			lua_pushinteger(Script, (long long)&*scene);
			return 1;
		}
	}
	return 0;
}
int GetCurrentEditMode(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		EditorScenePtr scene = (EditorScene*)lua_tointeger(Script, 1);
		auto EditMode = scene->GetCurrentEditMode();
		if (EditMode)
		{
			std::string EditModeName = EditMode->GetEditModeName();
			lua_pushinteger(Script, (long long)&*EditMode);
			lua_pushstring(Script, EditModeName.c_str());
			return 2;
		}
	}
	return 0;
}
int HasSelectedObject(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		EditModePtr _EditMode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHMDMode, _EditMode))
		{
			SHMDModePtr mode = NiSmartPointerCast(SHMDMode, _EditMode);
			bool _HasSelected = mode->HasSelectedObject();
			lua_pushboolean(Script, _HasSelected);
			return 1;
		}
	}
	return 0;
}
int GetCurrentObjectMode(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		ImGuizmo::OPERATION OperationMode = _EditMode->GetOperationMode();
		lua_pushinteger(Script, OperationMode);
		return 1;
	}
	return 0;
}
int RadioButton(lua_State* Script) 
{
	if (lua_isinteger(Script, 1) && lua_isstring(Script,2) && lua_isinteger(Script,3))
	{
		ImGuizmo::OPERATION CurOperationMode = static_cast<ImGuizmo::OPERATION>(lua_tointeger(Script, 1));
		auto Name = lua_tostring(Script, 2);
		ImGuizmo::OPERATION CheckOperationMode = static_cast<ImGuizmo::OPERATION>(lua_tointeger(Script, 3));
		lua_pushboolean(Script, ImGui::RadioButton(Name, (int*)&CurOperationMode, CheckOperationMode));
		return 1;
	}
	return 0;
}
int SetOperationMode(lua_State* Script) 
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		ImGuizmo::OPERATION OperationMode = static_cast<ImGuizmo::OPERATION>(lua_tointeger(Script, 2));
		_EditMode->SetOperationMode(OperationMode);
	}
	return 0;
}
int GetSelectedNode(lua_State* Script) 
{
	if (lua_isinteger(Script, 1))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		auto objs = _EditMode->GetSelectedNodes();
		if (objs.size() == 0)
			return 0;
		NiPickablePtr lastobj = objs.at(objs.size() - 1);
		lua_pushinteger(Script, (long long)&*lastobj);
		return 1;
	}
	return 0;
}
int GetTranslate(lua_State* Script) {
	if (lua_isinteger(Script, 1)) 
	{
		NiNodePtr obj = (NiNode*)lua_tointeger(Script, 1);
		auto pos = obj->GetTranslate();
		lua_pushinteger(Script, pos.x);
		lua_pushinteger(Script, pos.y);
		lua_pushinteger(Script, pos.z);
		return 3;
	}
	return 0;
}
int GetRotate(lua_State* Script) {
	if (lua_isinteger(Script, 1))
	{
		NiNodePtr obj = (NiNode*)lua_tointeger(Script, 1);
		auto matrix = obj->GetRotate();
		NiPoint3 data;
		matrix.ToEulerAnglesXYZ(data.x, data.y, data.z);
		data = data * 100.f;
		lua_pushinteger(Script, data.x);
		lua_pushinteger(Script, data.y);
		lua_pushinteger(Script, data.z);
		return 3;
	}
	return 0;
}
int DragFloat3(lua_State* Script) 
{
	if (lua_isstring(Script, 1) && lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3) && lua_isnumber(Script, 4)
		&& lua_isnumber(Script, 5) && lua_isnumber(Script, 6)
		&& lua_isnumber(Script, 7)) 
	{
		auto Name = lua_tostring(Script, 1);
		NiPoint3 Data;
		Data.x = lua_tonumber(Script, 2);
		Data.y = lua_tonumber(Script, 3);
		Data.z = lua_tonumber(Script, 4);
		float Speed = lua_tonumber(Script, 5);
		float Min = lua_tonumber(Script, 6);
		float Max = lua_tonumber(Script, 7);
		bool changed = ImGui::DragFloat3(Name, (float*)&Data, Speed, Min, Max);
		lua_pushboolean(Script, changed);
		lua_pushnumber(Script, Data.x);
		lua_pushnumber(Script, Data.y);
		lua_pushnumber(Script, Data.z);
		return 4;
	}
	return 0;
}
int SetTranslate(lua_State* Script) {
	if (lua_isinteger(Script, 1) && lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3) && lua_isnumber(Script, 4))
	{
		NiNodePtr obj = (NiNode*)lua_tointeger(Script, 1);
		NiPoint3 Data;
		Data.x = lua_tonumber(Script, 2);
		Data.y = lua_tonumber(Script, 3);
		Data.z = lua_tonumber(Script, 4);
		obj->SetTranslate(Data);
	}
	return 0;
}
int SetRotate(lua_State* Script) {
	if (lua_isinteger(Script, 1) && lua_isnumber(Script, 2)
		&& lua_isnumber(Script, 3) && lua_isnumber(Script, 4))
	{
		NiNodePtr obj = (NiNode*)lua_tointeger(Script, 1);
		NiPoint3 Data;
		Data.x = lua_tonumber(Script, 2);
		Data.y = lua_tonumber(Script, 3);
		Data.z = lua_tonumber(Script, 4);
		Data = Data / 100.f;
		NiMatrix3 mat; 
		mat.FromEulerAnglesXYZ(Data.x, Data.y, Data.z);
		obj->SetRotate(mat);
	}
	return 0;
}
int GetScale(lua_State* Script) 
{
	if (lua_isinteger(Script, 1)) 
	{
		NiNodePtr obj = (NiNode*)lua_tointeger(Script, 1);
		float Scale = obj->GetScale();
		lua_pushnumber(Script, Scale);
		return 1;
	}
	return 0;
}
int SetScale(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isnumber(Script,2))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		
		float Scale = lua_tonumber(Script, 2);

		_EditMode->UpdateScale(Scale);
	}
	return 0;
}
int CheckBox(lua_State* Script) 
{
	if (lua_isstring(Script, 1) && lua_isboolean(Script, 2)) 
	{
		auto name = lua_tostring(Script, 1);
		bool active = lua_toboolean(Script, 2);
		auto changed = ImGui::Checkbox(name, &active);
		lua_pushboolean(Script, changed);
		return 1;
	}
	return 0;
}
int GetSnapMoveStep(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		auto snap = _EditMode->GetSnapSize();
		lua_pushnumber(Script, snap.x);
		return 1;
	}
	return 0;
}
int SetSnapMoveStep(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isnumber(Script,2))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		float snap = lua_tonumber(Script, 2);
		_EditMode->SetSnapSize(snap);
	}
	return 0;
}
int SetSnapMove(lua_State* Script) 
{
	if (lua_isinteger(Script, 1) && lua_isboolean(Script, 2))
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		bool snap = lua_toboolean(Script, 2);
		_EditMode->SetSnapeMovement(snap);
	}
	return 0;
}
int GetSnapMove(lua_State* Script)
{
	if (lua_isinteger(Script, 1) )
	{
		SHMDModePtr _EditMode = (SHMDMode*)lua_tointeger(Script, 1);
		lua_pushboolean(Script, _EditMode->GetSnapMovement());
		return 1;
	} 
	return 0;
}
int CreateAddElement(lua_State* Script) 
{
	if (lua_isinteger(Script, 1) && lua_isstring(Script, 2))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHMDMode, mode)) 
		{
			SHMDModePtr ptr = NiSmartPointerCast(SHMDMode, mode);
			auto name = lua_tostring(Script, 2);
			ptr->CreateAddElement(name);
			
		}
	}
	return 0;
}
int GetBrushSize(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			lua_pushinteger(Script,ptr->GetBrushSize());
			return 1;
		}
	}
	return 0;
}
int DragInt(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isstring(Script, 2)
		&& lua_isnumber(Script, 3) && lua_isinteger(Script, 4) && lua_isinteger(Script, 5))
	{
		int number = lua_tointeger(Script, 1);
		auto name = lua_tostring(Script, 2);
		float Speed = lua_tonumber(Script, 3);
		int Min = lua_tointeger(Script, 4);
		int Max = lua_tointeger(Script, 5);

		bool changed = ImGui::DragInt(name, &number, Speed, Min, Max);
		lua_pushboolean(Script, changed);
		lua_pushinteger(Script, number);
		return 2;
	}
	return 0;
}
int SetBrushSize(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isinteger(Script, 2))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			int Size =lua_tointeger(Script, 2);
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			ptr->SetBrushSize(Size);
		}
	}
	return 0;
}
int ShowSHMDElements(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			lua_pushboolean(Script, ptr->GetShowElements());
			return 1;
		}
	}
	return 0;
}
int SetShowSHMDElements(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isboolean(Script, 2))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			bool Show = lua_toboolean(Script, 2);
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			ptr->SetShowElements(Show);
		}
	}
	return 0;
}
int GetWalkable(lua_State* Script)
{
	if (lua_isinteger(Script, 1))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			lua_pushboolean(Script, ptr->GetWalkable());
			return 1;
		}
	}
	return 0;
}
int SetWalkable(lua_State* Script)
{
	if (lua_isinteger(Script, 1) && lua_isboolean(Script, 2))
	{
		EditModePtr mode = (EditMode*)lua_tointeger(Script, 1);
		if (NiIsKindOf(SHBDMode, mode))
		{
			bool Show = lua_toboolean(Script, 2);
			SHBDModePtr ptr = NiSmartPointerCast(SHBDMode, mode);
			ptr->SetWalkable(Show);
		}
	}
	return 0;
}

void SetFunctions(lua_State* Script) 
{
	lua_pushcclosure(Script, LogFromLua, 0);
	lua_setglobal(Script, "Log");

	lua_pushcclosure(Script, ImGuiText, 0);
	lua_setglobal(Script, "Text");

	lua_pushcclosure(Script, ImGuiButton, 0);
	lua_setglobal(Script, "Button");

	lua_pushcclosure(Script, ImGuiSameLine, 0);
	lua_setglobal(Script, "SameLine");

	lua_pushcclosure(Script, ImGuiStartTabWindow, 0);
	lua_setglobal(Script, "CreateTabWindow");

	lua_pushcclosure(Script, ImGuiEndTabWindow, 0);
	lua_setglobal(Script, "EndTabWindow");

	lua_pushcclosure(Script, ImGuiBeginTabBar, 0);
	lua_setglobal(Script, "BeginTabBar");

	lua_pushcclosure(Script, ImGuiEndTabBar, 0);
	lua_setglobal(Script, "EndTabBar");

	lua_pushcclosure(Script, ImGuiNewLine, 0);
	lua_setglobal(Script, "NewLine");

	lua_pushcclosure(Script, ImGuiPopTree, 0);
	lua_setglobal(Script, "PopTree");

	lua_pushcclosure(Script, ImGuiTreeView, 0);
	lua_setglobal(Script, "TreeView");

	lua_pushcclosure(Script, ImGuiSelectable, 0);
	lua_setglobal(Script, "Selectable");

	lua_pushcclosure(Script, GetChild, 0);
	lua_setglobal(Script, "GetChild");

	lua_pushcclosure(Script, GetChildCount, 0);
	lua_setglobal(Script, "GetChildCount");

	lua_pushcclosure(Script, HasChilds, 0);
	lua_setglobal(Script, "HasChilds");

	lua_pushcclosure(Script, GetNodeName, 0);
	lua_setglobal(Script, "GetNodeName");

	lua_pushcclosure(Script, GetWorldNode, 0);
	lua_setglobal(Script, "GetWorldNode");

	lua_pushcclosure(Script, MakeNoCollapse, 0);
	lua_setglobal(Script, "MakeNoCollapse");

	lua_pushcclosure(Script, MakeNoMove, 0);
	lua_setglobal(Script, "MakeNoMove");

	lua_pushcclosure(Script, DragFloat, 0);
	lua_setglobal(Script, "DragFloat");

	lua_pushcclosure(Script, ColorEdit3, 0);
	lua_setglobal(Script, "ColorEdit3");

	lua_pushcclosure(Script, CompactChildArray, 0);
	lua_setglobal(Script, "CompactChildArray");

	lua_pushcclosure(Script, RemoveChild, 0);
	lua_setglobal(Script, "RemoveChild");

	lua_pushcclosure(Script, CollapsingHeader, 0);
	lua_setglobal(Script, "CollapsingHeader");

	lua_register(Script, "GetSkyNode", GetSkyNode);
	lua_register(Script, "RemoveSky", RemoveSky);
	lua_register(Script, "GetWaterNode", GetWaterNode);
	lua_register(Script, "RemoveWater", RemoveWater);
	lua_register(Script, "GetGroundObjectNode", GetGroundObjectNode);
	lua_register(Script, "RemoveGroundObject", RemoveGroundObject);

	lua_register(Script, "GetGlobalLight", GetGlobalLight);
	lua_register(Script, "SetGlobalLight", SetGlobalLight);
	lua_register(Script, "GetFogColor", GetFogColor);
	lua_register(Script, "GetFogDepth", GetFogDepth);
	lua_register(Script, "SetFogColor", SetFogColor);
	lua_register(Script, "SetFogDepth", SetFogDepth);
	lua_register(Script, "GetBackgroundColor", GetBackgroundColor);
	lua_register(Script, "SetBackgroundColor", SetBackgroundColor);
	lua_register(Script, "GetFrustum", GetFrustum);
	lua_register(Script, "SetFrustum", SetFrustum);
	lua_register(Script, "GetAmbientLightColor", GetAmbientLightColor);
	lua_register(Script, "SetAmbientLightColor", SetAmbientLightColor);
	lua_register(Script, "GetDiffuseLightColor", GetDiffuseLightColor);
	lua_register(Script, "SetDiffuseLightColor", SetDiffuseLightColor);
	lua_register(Script, "GetIngameWorld", GetIngameWorld);

	lua_register(Script, "GetCurrentScenePtr", GetCurrentScenePtr);
	lua_register(Script, "GetCurrentEditMode", GetCurrentEditMode);
	lua_register(Script, "HasSelectedObject", HasSelectedObject);
	lua_register(Script, "GetCurrentObjectMode", GetCurrentObjectMode);
	lua_register(Script, "RadioButton", RadioButton);
	lua_register(Script, "SetOperationMode", SetOperationMode);
	lua_register(Script, "GetSelectedNode", GetSelectedNode);
	lua_register(Script, "GetTranslate", GetTranslate);
	lua_register(Script, "GetRotate", GetRotate);
	lua_register(Script, "DragFloat3", DragFloat3);
	lua_register(Script, "SetTranslate", SetTranslate);
	lua_register(Script, "SetRotate", SetRotate);
	lua_register(Script, "GetScale", GetScale);
	lua_register(Script, "SetScale", SetScale);
	lua_register(Script, "CheckBox", CheckBox);
	lua_register(Script, "GetSnapMoveStep", GetSnapMoveStep);
	lua_register(Script, "SetSnapMoveStep", SetSnapMoveStep);
	lua_register(Script, "SetSnapMove", SetSnapMove);
	lua_register(Script, "GetSnapMove", GetSnapMove);
	lua_register(Script, "CreateAddElement", CreateAddElement);

	lua_register(Script, "GetBrushSize", GetBrushSize);
	lua_register(Script, "DragInt", DragInt);
	lua_register(Script, "SetBrushSize", SetBrushSize);
	lua_register(Script, "ShowSHMDElements", ShowSHMDElements);
	lua_register(Script, "SetShowSHMDElements", SetShowSHMDElements);
	lua_register(Script, "SetWalkable", SetWalkable);
	lua_register(Script, "GetWalkable", GetWalkable);
}