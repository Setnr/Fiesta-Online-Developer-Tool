WindowName = "SHBD-Editor"
function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

function render(ElementPtr)
    local ret = true
    local ScenePtr = GetCurrentScenePtr(ElementPtr)
    local EditModePtr, EditModeName = GetCurrentEditMode(ScenePtr)

    if EditModeName == "SHBD" then
        local BrushSize = GetBrushSize(EditModePtr)
        local changed, BrushSize = DragInt(BrushSize,"Brush Size",1.0,0,100)
        if changed then
            SetBrushSize(EditModePtr, BrushSize)
        end
        local Walkable = GetWalkable(EditModePtr)
        if CheckBox("Switch to Walkable",Walkable) then
            if Walkable then
                Walkable = false
            else
                Walkable = true
            end 
            SetWalkable(EditModePtr,Walkable)
        end

        local SnapMove = ShowSHMDElements(EditModePtr)
        if CheckBox("Show SHMD Elements",SnapMove) then
            if SnapMove then
                SnapMove = false
            else
                SnapMove = true
            end 
            SetShowSHMDElements(EditModePtr,SnapMove)
        end
    end
end

