WindowName = "SHMD-Middle-Mouse"


function prepare(ElementPtr)
    MakeNoCollapse(ElementPtr)
end

function render(ElementPtr)
    local ret = true
    local ScenePtr = GetCurrentScenePtr(ElementPtr)
    local EditModePtr, EditModeName = GetCurrentEditMode(ScenePtr)
    if Selectable("Add Sky") then
        CreateAddElement(EditModePtr,"Sky")
        ret = false
    end
    if Selectable("Add Water") then
        CreateAddElement(EditModePtr,"Water")
        ret = false
    end
    if Selectable("Add Ground") then
        CreateAddElement(EditModePtr,"Ground")
        ret = false
    end
    if Selectable("Add Moveable Object") then
        CreateAddElement(EditModePtr,"Moveable Object")
        ret = false
    end
    return ret
end

