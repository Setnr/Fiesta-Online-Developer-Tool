WindowName = "SHMD-Editor"
function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

function render(ElementPtr)
    local ret = true
    local WorldPtr = GetIngameWorld(ElementPtr)

    SHMDList(WorldPtr,GetSkyNode,RemoveSky,"Sky Node")

    SHMDList(WorldPtr,GetWaterNode,RemoveWater,"Water Node")
    SHMDList(WorldPtr,GetGroundObjectNode,RemoveGroundObject,"Global GroundObjects")

    DrawColorEdit(WorldPtr, GetGlobalLight, SetGlobalLight,"Global Light")
    DrawColorEdit(WorldPtr, GetFogColor, SetFogColor,"Fog Color")
    DrawDragFloat(WorldPtr,GetFogDepth,SetFogDepth,"FogDepth",0.01,0,1)
    DrawColorEdit(WorldPtr, GetBackgroundColor, SetBackgroundColor,"Background Color")
    DrawDragFloat(WorldPtr,GetFrustum,SetFrustum,"Frustum",1,0,5000)
    DrawColorEdit(WorldPtr, GetAmbientLightColor, SetAmbientLightColor,"AmbientLight Color")
    DrawColorEdit(WorldPtr, GetDiffuseLightColor, SetDiffuseLightColor,"DiffuseLight Color")
end

function SHMDList(WorldPtr, GetFunc, RemoveFunc, Name)
    if CollapsingHeader(Name)
    then
        local Compact = false
        local NodeTable = GetFunc(WorldPtr)
        for entry = 0, #NodeTable, 1 do
            local Node = NodeTable[entry]
            if Selectable(GetNodeName(Node)) 
            then
                RemoveFunc(WorldPtr,Node)
                Compact = true
            end
        end
        if Compact 
        then
            CompactChildArray(Node)
        end
    end
end

function DrawColorEdit(WorldPtr, GetFunc, SetFunc, EditName)
    local Red,Green,Blue = GetFunc(WorldPtr)
    local Changed, Red,Green,Blue = ColorEdit3(EditName, Red,Green,Blue)
    if Changed 
    then
        SetFunc(WorldPtr,Red,Green,Blue)
    end
end

function DrawDragFloat(WorldPtr, GetFunc, SetFunc, EditName,Speed,Min,Max)
    local _Float = GetFunc(WorldPtr)
    local Changed, _Float =  DragFloat(_Float,EditName,Speed,Min,Max)
    if Changed then
        SetFunc(WorldPtr,_Float)
    end
    return ret
end

