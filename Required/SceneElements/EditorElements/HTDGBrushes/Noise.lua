BrushName = "Noise"

NoiseType = {
    OpenSimplex2 = {ID = 0, Name = "Simplex2" },
    OpenSimplex2S = {ID = 1, Name = "Simplex2S" },
    Celular = {ID = 2, Name = "Celular" },
    Perlin = {ID = 3, Name = "Perlin" },
    ValueCubic = {ID = 4, Name = "ValueCubic" },
    Value = {ID = 5, Name = "Value" }
}

FractalType = {
    None = {ID = 0, Name = "None"},
    FBm = {ID = 1, Name = "FBm"},
    Ridged = {ID = 2, Name = "Ridged"},
    PingPong = {ID = 3, Name = "PingPong"},
    DomianWarpProgressive = {ID = 4, Name = "DomianWarpProgressive"},
    DomianWarpIndependent = {ID = 5, Name = "DomianWarpIndependent"}
}

CellularDistanceFunction = {
    Euclidian = {ID = 0, Name = "Euclidian"},
    EuclidianSq = {ID = 1, Name = "EuclidianSq"},
    Manhattan = {ID = 2, Name = "Manhattan"},
    Hybrid = {ID = 3, Name = "Hybrid"}
}

CellularReturnType = {
    CellValue = {ID = 0, Name = "CellValue"},
    Distance = {ID = 1, Name = "Distance"},
    Distance2 = {ID = 2, Name = "Distance2"},
    DistanceAdd = {ID = 3, Name = "DistanceAdd"},
    DistanceSub = {ID = 4, Name = "DistanceSub"},
    DistanceMult = {ID = 5, Name = "DistanceMult"},
    DistanceDiv = {ID = 6, Name = "DistanceDiv"}
}

DomainWarpType = {
    OpenSmilpex2 = {ID = 0, Name = "OpenSimplex2"},
    OpenSimplex2Reduced = {ID = 1, Name = "OpenSimplex2Reduced"},
    BasicGrid = {ID = 2, Name = "BasicGrid"}
}

BrushData = { 
    Height = 10.0,
    Seed = 195201418,
    Frequency = 0.01,
    NoiseType = NoiseType.Perlin.Name,
    FractalType = FractalType.None.Name,
    Octaves = 3,
    Lacunarity = 2.0,
    Gain = 0.5,
    WeightedStrength = 0.0,
    PingPongStrength = 2.0,
    CellularDistanceFunction = CellularDistanceFunction.EuclidianSq.Name,
    CellularReturnType = CellularReturnType.Distance.Name,
    DomainWarpType = DomainWarpType.OpenSmilpex2.Name,
    DomainWarpAmp = 1.0,
    Show = false
}
function Init(BrushPtr, world)
    CreateNoise(BrushPtr, BrushData.Seed)
    SetNoiseType(BrushPtr,BrushData.NoiseType)
    CreateTexture(BrushPtr, 350)
end

function render(BrushPtr)
    if Button("Random Seed") then
        BrushData.Seed = RandomInt()
        SetNoiseSeed(BrushPtr, BrushData.Seed)
        if HasNoise(BrushPtr) then
            CreateTexture(BrushPtr, 350)
        end
    end
    SameLine()
    local msg = "Show Preview"
    if BrushData.Show then
        msg = "Hide Preview"
    end
    if Button(msg) then
        if BrushData.Show then
            BrushData.Show = false
        else
            BrushData.Show = true
        end
    end

    changed, BrushData.Height = DragFloat(BrushData["Height"],"Brush Height",0.01,-5000,5000)
    BrushData.Seed = ShowEntry(BrushPtr, BrushData.Seed,"Seed", 1.0, math.mininteger, math.maxinteger,SetNoiseSeed,DragInt)
    BrushData.Frequency = ShowEntry(BrushPtr, BrushData.Frequency,"Frequency", 0.01,-2.0, 2.0,SetNoiseFrequency,DragFloat)
    BrushData.NoiseType = ShowTable("NoiseType",NoiseType,BrushData.NoiseType, SetNoiseType,BrushPtr)
    BrushData.FractalType = ShowTable("FractalType",FractalType,BrushData.FractalType, SetFractalType,BrushPtr)
    if BrushData.FractalType ~= FractalType.None.Name then
        BrushData.Octaves = ShowEntry(BrushPtr, BrushData.Octaves,"Octaves", 1.0,math.mininteger,25,SetNoiseOctaves,DragInt)
        BrushData.Lacunarity = ShowEntry(BrushPtr, BrushData.Lacunarity,"Lacunarity", 0.01,-2.0, 2.0,SetNoiseLacunarity,DragFloat)
        BrushData.Gain = ShowEntry(BrushPtr, BrushData.Gain,"Gain", 0.01,-2.0, 2.0,SetNoiseGain,DragFloat)
        BrushData.WeightedStrength = ShowEntry(BrushPtr, BrushData.WeightedStrength,"WeightedStrength", 0.01,-2.0, 2.0,SetNoiseWeightedStrength,DragFloat)
        if BrushData.FractalType == FractalType.PingPong.Name then
            BrushData.PingPongStrength = ShowEntry(BrushPtr, BrushData.PingPongStrength,"PingPongStrength", 0.01,-2.0, 2.0,SetNoisePingPongStrength,DragFloat)
        end
    end
    if BrushData.NoiseType == NoiseType.Celular.Name then
        BrushData.CellularDistanceFunction = ShowTable("CellularDistanceFunction",CellularDistanceFunction,BrushData.CellularDistanceFunction, SetCellularDistanceFunction,BrushPtr)
        BrushData.CellularReturnType = ShowTable("CellularReturnType",CellularReturnType,BrushData.CellularReturnType, SetCellularReturnType,BrushPtr)
    end
    
    if HasNoise(BrushPtr) and BrushData.Show then
        RenderNoise(BrushPtr)
    end
end

function ShowTable(Name,Table, TableValue, Func,BrushPtr) 
    local c,v,n = Combo(Name,TableValue,Table)
    if c then
        Func(BrushPtr,n)
        if HasNoise(BrushPtr) then
            CreateTexture(BrushPtr, 350)
        end
    end
    return v
    
end
function ShowEntry(BrushPtr, value,name, step,min,max, SetFunc,DragFunc)
    local c, v = DragFunc(value,name,step,min, max)
    if c then
        SetFunc(BrushPtr, value)
        if HasNoise(BrushPtr) then
            CreateTexture(BrushPtr, 350)
        end
    end
    return v
end

function algorithm(MiddleW,MiddleH, z, SizeW, SizeH ,BrushSize,WorldPtr, BrushPtr)
    for w = MiddleW - BrushSize, MiddleW + BrushSize , 1 do
        if w >= 0 and w < SizeW then
            for h = MiddleH - BrushSize, MiddleH + BrushSize , 1 do
                if h >= 0 and h < SizeH then
                    if (((w - MiddleW) * (w - MiddleW) + (h - MiddleH) * (h - MiddleH) <= BrushSize * BrushSize)) then
                        noise = GetNoiseValue(BrushPtr,w ,h )
                        height = GetHTD(WorldPtr,w,h)
                        SetHTD(WorldPtr,w,h, height + noise * BrushData.Height)
                    end
                end
            end
        end
    end
end

