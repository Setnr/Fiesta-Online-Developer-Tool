WindowName = "About"
function prepare(ElementPtr)

end

function render(ElementPtr)
    local ret = true

    Text("Fiesta Online DeveloperTool by SetNr12")
    Text("Switch Current Mode with Tab")
    Text("Press R to Reset the Camera")
    Text("Controls:")
    NewLine()
    if BeginTabBar("AboutTab") then
        if CreateTabWindow("SHMD") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Open SHMDMenu with Middle Mouse Click")
            Text("Select Objects with Left Click")
            Text("Copy and Paste with ctrl + c/v")
            Text("Move Up/Down with Q/E")
            Text("Switch to Translate Mode ctrl + w")
            Text("Switch to Rotate Mode ctrl + e")
            Text("Enable/Disable Snap Mode ctrl + d")
            Text("Undo/Redo ctrl + z/y")
            Text("Select all copies of the last selcted Object ctrl + a")
            Text("Select ALL Objects ctrl + alt + a")
            EndTabWindow()
        end
        if CreateTabWindow("SHBD") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Move Up/Down with Q/E")
            Text("Scroll to Move SHBD Up/Down")
            Text("Scrollt + alt to Resize Brush")
            Text("Change SHBD with Left Click")
            EndTabWindow()
        end
        if CreateTabWindow("HTDG") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Move Up/Down with Q/E")
            Text("Scrollt + alt to Resize Brush")
            Text("F1 to Show/Hide SHMD-Objects")
            EndTabWindow()
        end
        if CreateTabWindow("Texture") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Move Up/Down with Q/E")
            Text("Scrollt + alt to Resize Brush")
            Text("F1 to Show/Hide SHMD-Objects")
            EndTabWindow()
        end
        if CreateTabWindow("VertexColor") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Move Up/Down with Q/E")
            Text("Scrollt + alt to Resize Brush")
            Text("Copy the Color of the Current Hovered Vertex ctrl + c")
            Text("F1 to Show/Hide SHMD-Objects")
            EndTabWindow()
        end
        if CreateTabWindow("NPC") then
            Text("Move Camera with WASD")
            Text("Rotate Camera with Right Click")
            Text("Move Up/Down with Q/E")
            Text("Select Objects with Left Click")
            Text("Switch to Translate Mode ctrl + w")
            Text("Switch to Rotate Mode ctrl + e")
            Text("Add new NPC with Middle Mouse")
            EndTabWindow()
        end
    end
    EndTabBar()
    return ret
end
