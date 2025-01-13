WindowName = "Texture-Editor"
function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

function render(ElementPtr)
    local ret = true
    local ScenePtr = GetCurrentScenePtr(ElementPtr)
    local EditModePtr, EditModeName = GetCurrentEditMode(ScenePtr)

    if EditModeName == "Texture" then
        if Button("Save Selected Layer") then
            SaveLayer(EditModePtr)
        end
        local BrushSize = GetBrushSize(EditModePtr)
        local changed, BrushSize = DragInt(BrushSize,"Brush Size",1.0,0,100)
        if changed then
            SetBrushSize(EditModePtr, BrushSize)
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
        RenderLayers(EditModePtr)
    end
end

