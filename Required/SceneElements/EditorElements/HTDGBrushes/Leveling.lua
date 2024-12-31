BrushName = "Leveling"

BrushData = { }
function render(BrushPtr)

end

function algorithm(MiddleW,MiddleH, z, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h <= SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        SetHTD(WorldPtr,w,h,z)
                    end
                end
            end
        end
    end
    
end


