BrushName = "Smoothing"

BrushData = { 
    KernelSize = 3,
    Cycles = 3,
    UpdateTime = 0.085,
    LastUpdateTime = 0.0,
    WholeMap = false,
    Degree = 10
}
function Init(BrushPtr , world)
    
end

function render(BrushPtr)
    if CheckBox("Update Whole Map (Can cause lag!)",BrushData.WholeMap) then
            if BrushData.WholeMap then
                BrushData.WholeMap = false
            else
                BrushData.WholeMap = true
            end
    end
    local changed, value = DragInt(BrushData["KernelSize"],"KernelSize",2,3,21)
    BrushData["KernelSize"] = value
    hanged, value = DragInt(BrushData["Cycles"],"Cycles",1,1,20)
    BrushData["Cycles"] = value
    changed, value = DragFloat(BrushData["UpdateTime"],"UpdateTime",0.001,0,1)
    BrushData["UpdateTime"] = value
    changed, value = DragInt(BrushData.Degree,"Degree",1,1,180)
    BrushData.Degree = value
end

function algorithm(MiddleW,MiddleH, z,zAtClick, SizeW, SizeH ,BrushSize,WorldPtr)

    CurTime = GetCurrentTime()
    if BrushData["UpdateTime"] + BrushData["LastUpdateTime"] > CurTime then
        return
    end
    BrushData["LastUpdateTime"] = CurTime
    if BrushData.WholeMap then
        if SizeW > SizeH then
            BrushSize = math.floor(SizeW / 2 + 2)
        else 
            BrushSize = math.floor(SizeH / 2 + 2) 
        end
    end

    local KernelValue = 1 / (BrushData["KernelSize"] * BrushData["KernelSize"])
    local StartI = math.floor(-1 * (BrushData["KernelSize"] / 2)) + 1
	local EndI =  math.floor(BrushData["KernelSize"] / 2)
	local StartJ =  math.floor(-1 * (BrushData["KernelSize"] / 2)) + 1
	local EndJ =  math.floor(BrushData["KernelSize"] / 2)
    local PointDist = GetPointDistFromShineIni(WorldPtr)
   
        
   
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h - BrushData["KernelSize"] / 2 > 0 then
                    if h >= 0 and h < SizeH then
                        if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                            
                            local Smooth = false
                            for i = -1 , 1, 1 do
                                for j= - 1, 1 , 1 do
                                    if i ~= 0 and j ~= 0 then
                                        if CalculateSlope(GetHTD(WorldPtr,w - i, h-j), GetHTD(WorldPtr,w,h), PointDist) then
                                            Smooth = true
                                        end
                                    end
                                end

                            end
                            if Smooth then
                                for cycle = 0, BrushData["Cycles"] , 1 do
                                    local Sum  = 0
                                    for i = StartI, EndI , 1 do
                                        for j = StartJ, EndJ , 1 do
                                            Sum = Sum + GetHTD(WorldPtr , w - i , h - j) * KernelValue
                                        end
                                    end
                                    SetHTD(WorldPtr,w,h,Sum)
                                end
                            end
                        end
                    end
                end
            end
        end
    end
end

function CalculateSlope(Point1, Point2, PointDist)
    local PointH, PointL
    if Point1 > Point2 then
        PointH = Point1
        PointL = Point2
    else
        PointH = Point2
        PointL = Point1
    end
    local degree = math.deg(math.atan((PointH - PointL) / PointDist))
    if degree > BrushData.Degree then
        return true
    end
    return false
end
