WindowName = "Node OverView"
function prepare(ElementPtr)

end

function render(ElementPtr)
    local ret = true

    local WorldPtr = GetWorldNode(ElementPtr)
    if VisualizeNode(WorldPtr) 
    then
        PrintChildNodes(WorldPtr)
        PopTree()
    end
    return ret
end

function PrintChildNodes(NodePtr)
    if HasChilds(NodePtr) 
    then 
        if VisualizeNode(NodePtr) 
        then
            local ChildCount = GetChildCount(NodePtr)
            for child = 0, ChildCount, 1 do
                local Child = GetChild(NodePtr, child)
                PrintChildNodes(Child)
            end
            PopTree()
        end
    end
end

function VisualizeNode(NodePtr)
    local ret = TreeView(NodePtr)
    SameLine()
    if Selectable(GetNodeName(NodePtr)) then
        Log(GetNodeName(NodePtr) ..  " got selected")
        --SelectNode(NodePtr)
        --SwitchCameraToNode(NodePtr)
    end
    return ret
end
