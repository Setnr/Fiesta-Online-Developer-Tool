WindowName = "NPC-Editor"
function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

Data = {
    
}

function render(ElementPtr)
    local ret = true
    local WorldPtr = GetIngameWorld(ElementPtr)
    local ScenePtr = GetCurrentScenePtr(ElementPtr)
    local EditModePtr, EditModeName = GetCurrentEditMode(ScenePtr)
    if EditModeName == "NPC" then
        if HasSelectedObject(EditModePtr) then
            local CurEditMode = GetCurrentObjectMode(EditModePtr)
            if RadioButton(CurEditMode, "Translate" , 1|2) then
                SetOperationMode(EditModePtr,1|2)
            end
            SameLine()
            if RadioButton(CurEditMode, "Rotate" , 32) then
                SetOperationMode(EditModePtr,32)
            end
            local Node = GetSelectedNode(EditModePtr)
            local x,y,z = GetTranslate(Node)
            local pitch,yaw,roll = GetRotate(Node)
            BeginDisabled()
            DragFloat3("Position",x,y,z,0.0001,0.00000,5)
            DragFloat3("Rotation",pitch,yaw,roll,0.01,0.0,50000)
            EndDisabled()
            DrawObjectMenu(Node)
        end
    end
   
end

