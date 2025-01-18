BrushName = "Color"
BrushData = {
    red = 0.0,
    green = 0.0,
    blue = 0.0
}

function Init(BrushPtr, world) 
end

function SetColor(ColorR, ColorG, ColorB)
    BrushData.red = ColorR
    BrushData.green=ColorG
    BrushData.blue = ColorB
end

function render(BrushPtr)
    local changed, ColorR,ColorG,ColorB = VertexColorPick(BrushData.red,BrushData.green, BrushData.blue,"Color")
    if changed then
        BrushData.red = ColorR
        BrushData.green=ColorG
        BrushData.blue = ColorB
    end
end

function algorithm(MiddleW,MiddleH, z, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h < SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        SetVertexColor(WorldPtr, w, h, BrushData.red,BrushData.green, BrushData.blue)
                    end
                end
            end
        end
    end
end





