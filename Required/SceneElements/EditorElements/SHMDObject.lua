WindowName = "SHMD-Object"
TranslateMode = 7
RotateMode = 120


function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

function render(ElementPtr)
    local ScenePtr = GetCurrentScenePtr(ElementPtr)
    local EditModePtr, EditModeName = GetCurrentEditMode(ScenePtr)
    if EditModeName == "SHMD" then
        if HasSelectedObject(EditModePtr) then
            local CurEditMode = GetCurrentObjectMode(EditModePtr)
            if RadioButton(CurEditMode, "Translate" , 7) then
                SetOperationMode(EditModePtr,7)
            end
            SameLine()
            if RadioButton(CurEditMode, "Rotate" , 120) then
                SetOperationMode(EditModePtr,120)
            end
            local Node = GetSelectedNode(EditModePtr) -- If multiple selected this only returns the last selected
            local x,y,z = GetTranslate(Node)
            local pitch,yaw,roll = GetRotate(Node)
            local changed, x,y,z = DragFloat3("Position",x,y,z,0.0001,0.00000,5)
            if changed then 
                SetTranslate(Node, x,y,z)
            end
            local changed, pitch,yaw,roll = DragFloat3("Rotation",pitch,yaw,roll,0.01,0.0,50000)
            if changed then
                SetRotate(Node, pitch,yaw,roll)
            end
            local Scale = GetScale(Node)
            local changed, NewScale = DragFloat(Scale,"Scale",0.01,0.01,5)
            if changed then
                SetScale(EditModePtr, Scale - NewScale)
            end
            SnapMove = GetSnapMove(EditModePtr)
            if CheckBox("Snap",SnapMove) then
                if SnapMove then
                    SnapMove = false
                else
                    SnapMove = true
                end 
                SetSnapMove(EditModePtr,SnapMove)
            end
            if SnapMove then
                local SnapMoveStep = GetSnapMoveStep(EditModePtr)
                local changed, SnapMoveStep = DragFloat(SnapMoveStep,"Step-Size",0.01,0.01,5)
                if changed then
                    SetSnapMoveStep(EditModePtr,SnapMoveStep)
                end
            end
        end
    end
end

