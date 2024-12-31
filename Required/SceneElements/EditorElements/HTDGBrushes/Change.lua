BrushName = "Change"
BrushData = {
    Level = 100
}
function render(BrushPtr)
    local changed, Level = DragFloat(BrushData["Level"],"Brush Level",0.01,-5000,5000)
    BrushData["Level"] = Level
end

function algorithm(MiddleW,MiddleH, z, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h <= SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        local CurHeight = GetHTD(WorldPtr,w,h)
                        SetHTD(WorldPtr,w,h,CurHeight + BrushData["Level"])
                    end
                end
            end
        end
    end
    
end





