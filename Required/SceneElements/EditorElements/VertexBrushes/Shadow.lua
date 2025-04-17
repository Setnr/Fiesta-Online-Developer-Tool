BrushName = "Shadow"
BrushData = {
    red = 0.0,
    green = 0.0,
    blue = 0.0,
    world = 0,
    red2 = 0.0,
    green2 = 0.0,
    blue2 = 0.0,
    LiveRender = false
}

function Init(BrushPtr, world) 
    BrushData.world = world
end

function SetColor(ColorR, ColorG, ColorB)
end

function render(BrushPtr)
    if Button("Recreate Shadow") then
        CreateShadow(BrushData.world, BrushData.red, BrushData.green, BrushData.blue, BrushData.red2, BrushData.green2, BrushData.blue2)
        
    end
    if CheckBox("Live Render While Left Click",BrushData.LiveRender) then
        if BrushData.LiveRender == true then
            BrushData.LiveRender = false
        else
            BrushData.LiveRender = true
        end 
    end
    local changed, ColorR,ColorG,ColorB = VertexColorPick(BrushData.red,BrushData.green, BrushData.blue,"Shadow Color")
    if changed then
        BrushData.red = ColorR
        BrushData.green=ColorG
        BrushData.blue = ColorB
    end
    changed, ColorR,ColorG,ColorB = VertexColorPick(BrushData.red2,BrushData.green2, BrushData.blue2,"SunLight Color")
    if changed then
        BrushData.red2 = ColorR
        BrushData.green2=ColorG
        BrushData.blue2 = ColorB
    end
end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)
    if BrushData.LiveRender then
        CreateShadow(WorldPtr, BrushData.red, BrushData.green, BrushData.blue, BrushData.red2, BrushData.green2, BrushData.blue2)
    end
end





