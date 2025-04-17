BrushName = "Change"
BrushData = {
    Color = 1.0,
    layer = nil
}

function Init(BrushPtr, world) 
end

function SetLayer(LayerPtr)
    BrushData.layer = LayerPtr
end

function render(BrushPtr)
    local changed, Color = TextureColorPick(BrushData["Color"],"Texture Color")
    BrushData.Color = Color
end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h < SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        SetTextureColor(BrushData.layer, w, h, BrushData["Color"])
                    end
                end
            end
        end
    end
    MarkAsChanged(BrushData.layer)
end





