BrushName = "Smoothing"
BrushData = {
    KernelSize = 3,
    Cycles = 1,
    UpdateTime = 0.05,
    LastUpdateTime = 0.0
}

function Init(BrushPtr, world) 
end

function SetColor(ColorR, ColorG, ColorB)
   
end

function render(BrushPtr)
    changed, value = DragInt(BrushData["KernelSize"],"KernelSize",2,3,21)
    BrushData["KernelSize"] = value
    changed, value = DragInt(BrushData["Cycles"],"Cycles",1,1,20)
    BrushData["Cycles"] = value
    changed, value = DragFloat(BrushData["UpdateTime"],"UpdateTime",0.001,0,1)
    BrushData["UpdateTime"] = value
end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)
    CurTime = GetCurrentTime()
    if BrushData["UpdateTime"] + BrushData["LastUpdateTime"] > CurTime then
        return
    end
    BrushData["LastUpdateTime"] = CurTime
    
    local KernelValue = 1 / (BrushData["KernelSize"] * BrushData["KernelSize"])
    local StartI = math.floor(-1 * (BrushData["KernelSize"] / 2)) + 1
	local EndI =  math.floor(BrushData["KernelSize"] / 2)
	local StartJ =  math.floor(-1 * (BrushData["KernelSize"] / 2)) + 1
	local EndJ =  math.floor(BrushData["KernelSize"] / 2)

    for cycle = 0, BrushData["Cycles"] , 1 do
        for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
            if w >= 0 and w < SizeW then
                for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                    if h - BrushData["KernelSize"] / 2 > 0 then
                        if h >= 0 and h < SizeH then
                            if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                                local red  = 0
                                local green  = 0
                                local blue  = 0
                                for i = StartI, EndI , 1 do
                                    for j = StartJ, EndJ , 1 do
                                        local CurR,CurG,CurB  = GetVertexColor(WorldPtr,w- i,h -j)
                                        red = red + CurR * KernelValue
                                        green = green + CurG * KernelValue
                                        blue = blue + CurB * KernelValue
                                    end
                                end
                                SetVertexColor(WorldPtr,w, h, red,green,blue)
                            end
                        end
                    end
                end
            end
        end
    end
end





