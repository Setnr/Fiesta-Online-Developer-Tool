BrushName = "Leveling"

BrushData = { }
function Init(BrushPtr , world) end

function render(BrushPtr)

end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h < SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        SetHTD(WorldPtr,w,h,zAtClick)
                    end
                end
            end
        end
    end
    
end


