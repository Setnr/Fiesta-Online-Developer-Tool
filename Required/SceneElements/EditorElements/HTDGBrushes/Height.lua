BrushName = "Height"
BrushData = {
    Height = 1
}
function Init(BrushPtr, world) end

function render(BrushPtr)
    local changed, Height = DragFloat(BrushData["Height"],"Brush Height",0.01,-5000,5000)
    BrushData["Height"] = Height
end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h < SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        SetHTD(WorldPtr,w,h,BrushData["Height"])
                    end
                end
            end
        end
    end
    
end

