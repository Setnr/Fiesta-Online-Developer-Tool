#include "Fractal.h"
#include <NiMemObject.h>
#include <NiVertexColorProperty.h>

Fractal::Fractal(int grid_size)
    : grid_size{ grid_size },
    grid{ nullptr } 
{

    CreateTexture(grid_size);
    
}
Fractal::~Fractal() {
    Show(false);
    cleanUpGrid<float>(grid);
}

void Fractal::generateGrid(int grid_size, int seed, float noise, float random_min, float random_max) {
    this->grid_size = grid_size;
    int s = grid_size - 1;
    if (!((s & (s - 1)) == 0) || random_min >= random_max) {
        return;
    }

    grid = createGrid<float>(0);

}

void Fractal::applyThermalErosion() {
    if (grid == nullptr) {
        return;
    }

    bool** grid_checked = createGrid<bool>(false);

    float peak = grid[0][0];
    float low = grid[0][0];

    // look for the highest point
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (grid[i][j] > peak) {
                peak = grid[i][j];
            }
            if (grid[i][j] < low) {
                low = grid[i][j];
            }
        }
    }
    float erosion_height = (peak - low) / 1000.0f;
    applyThermalErosionTonNeighbour(erosion_height);

    cleanUpGrid<bool>(grid_checked);
    UpdatePixelData(low, peak);
}

void Fractal::applyHydraulicErosion(float quantity, float sediment_factor) {
    if (grid == nullptr) {
        return;
    }

    std::vector<Peak> peaks;
    bool** grid_checked = createGrid<bool>(false);

    for (int x = 0; x < grid_size; x++) {
        for (int y = 0; y < grid_size; y++) {
            if (isPeak(grid[x][y], x, y)) {
                peaks.emplace_back(grid[x][y], x, y);
            }
        }
    }

    float** water_quantity = createGrid<float>(quantity);
    float** sediment_quantity = createGrid<float>(quantity * sediment_factor);

    for (const auto& peak : peaks) {
        applyHydraulicErosionFromPeak(peak, grid_checked, sediment_factor, water_quantity, sediment_quantity);
    }
    removeExcessWaterFromMargins(water_quantity);

    cleanUpGrid<float>(sediment_quantity);
    cleanUpGrid<float>(water_quantity);
    cleanUpGrid<bool>(grid_checked);
}

void Fractal::applyThermalErosionTonNeighbour(float erosion_height) {
    for (int x = 0; x < grid_size; x++) {
        for (int y = 0; y < grid_size; y++) {
            for (int i = -1; i <= 1; i++) {
                int position_x = x;
                int position_y = y;
                for (int j = -1; j <= 1; j++) {
                    // check if neighbour is inside the matrix
                    if (position_x + i >= 0 &&
                        position_x + i < grid_size &&
                        position_y + j >= 0 &&
                        position_y + j < grid_size) {
                        // check if material needs to be moved to the neighbour
                        if (grid[position_x + i][position_y + j] < grid[position_x][position_y]) {
                            grid[position_x + i][position_y + j] += erosion_height;
                            grid[position_x][position_y] -= erosion_height;

                        }
                    }
                }
            }
        }
    }
}

bool Fractal::isPeak(float value, int x, int y, bool upper) {
    bool isPeak = true;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // check if neighbour is inside the matrix
            if (x + i >= 0 &&
                x + i < grid_size &&
                y + j >= 0 &&
                y + j < grid_size) {
                if (upper) {
                    if (grid[x + i][y + j] > value) {
                        isPeak = false;
                        continue;
                    }
                }
                else {
                    if (grid[x + i][y + j] < value) {
                        isPeak = false;
                        continue;
                    }
                }
            }
        }
        if (!isPeak) {
            continue;
        }
    }
    return isPeak;
}

void Fractal::applyHydraulicErosionFromPeak(const Peak& peak, bool** grid_checked, float sediment_factor,
    float** water_quantity, float** sediment_quantity) {
    grid_checked[peak.x][peak.y] = true;

    int neighbour_count = countNeighbours(peak);
    if (neighbour_count == 0) {
        return;
    }

    float kd = 0.1f;
    float kc = 5.0f;
    float ks = 0.3f;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (peak.x + i >= 0 &&
                peak.x + i < grid_size &&
                peak.y + j >= 0 &&
                peak.y + j < grid_size) {
                if (!(i == 0 || j == 0)) {
                    continue;
                }
                // we do this only if the peak is higher
                if (grid[peak.x][peak.y] > grid[peak.x + i][peak.y + j]) {
                    if (!isMargin(peak, i, j)) {
                        // 1. Move water from the peak to the neighbours
                        float height = grid[peak.x][peak.y];
                        float neighbour_height = grid[peak.x + i][peak.y + j];
                        float wt = moveWater(height, neighbour_height, water_quantity[peak.x][peak.y],
                            water_quantity[peak.x + i][peak.y + j]);

                        if (wt > 0) {
                            water_quantity[peak.x][peak.y] -= wt;
                            water_quantity[peak.x + i][peak.y + j] += wt;

                            // 2. Move sediment to the neighbours
                            grid[peak.x][peak.y] -= wt * sediment_factor;
                            grid[peak.x + i][peak.y + j] += wt * sediment_factor;

                            float cs = kc * wt;

                            if (sediment_quantity[peak.x][peak.y] >= cs) {
                                sediment_quantity[peak.x + i][peak.y + j] += cs;
                                grid[peak.x][peak.y] += kd * (sediment_quantity[peak.x][peak.y] - cs);
                                sediment_quantity[peak.x][peak.y] = (1 - kd) * (sediment_quantity[peak.x][peak.y] - cs);
                            }
                            else {
                                sediment_quantity[peak.x + i][peak.y + j] +=
                                    sediment_quantity[peak.x][peak.y] +
                                    ks * (cs - sediment_quantity[peak.x][peak.y]);
                                grid[peak.x][peak.y] += -ks * (cs - sediment_quantity[peak.x][peak.y]);
                                sediment_quantity[peak.x][peak.y] = 0;
                            }
                        }
                        else {
                            grid[peak.x][peak.y] += ks * sediment_quantity[peak.x][peak.y];
                        }
                    }
                    else {
                        water_quantity[peak.x + i][peak.y + j] = 0;
                        sediment_quantity[peak.x + i][peak.y + j] = 0;
                    }
                    if (!grid_checked[peak.x + i][peak.y + j]) {
                        applyHydraulicErosionFromPeak(Peak{
                                                              grid[peak.x + i][peak.y + j],
                                                              peak.x + i,
                                                              peak.y + j },
                                                              grid_checked,
                                                              sediment_factor,
                                                              water_quantity,
                                                              sediment_quantity);
                    }
                }
            }
        }
    }
}

float Fractal::moveWater(float height, float neighbour_height, float water_quantity,  float water_quantity_neighbour) 
{
    return (((water_quantity) < ((water_quantity + height) - (water_quantity_neighbour + neighbour_height))) ? (water_quantity) : ((water_quantity + height) - (water_quantity_neighbour + neighbour_height)));
}

void Fractal::removeExcessWaterFromMargins(float** water_quantity) {
    std::vector<Peak> low_peaks;
    for (int x = 0; x < grid_size; x++) {
        for (int y = 0; y < grid_size; y++) {
            if (isPeak(grid[x][y], x, y, false)) {
                low_peaks.emplace_back(grid[x][y], x, y);
            }
        }
    }

    for (const auto& peak : low_peaks) {
        water_quantity[peak.x][peak.y] = 0.0f;
    }
}

int Fractal::countNeighbours(const Peak& peak) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (peak.x + i >= 0 &&
                peak.x + i < grid_size &&
                peak.y + j >= 0 &&
                peak.y + j < grid_size) {
                if (grid[peak.x + i][peak.y + j] < grid[peak.x][peak.y])
                    count++;
            }
        }
    }
    return count;
}
bool Fractal::isMargin(const Peak& peak, int i, int j) const {
    return peak.x + i == 0 ||
        peak.x + i == grid_size ||
        peak.y + j == 0 ||
        peak.y + j == grid_size;
}

// Returns a pointer to a 2D grid of type T with every value initialized to `default_value`.
template<typename T>
T** Fractal::createGrid(T default_value) {
    auto** grid = new T * [grid_size];
    for (int i = 0; i < grid_size; i++) {
        grid[i] = new T[grid_size];
        for (int j = 0; j < grid_size; j++) {
            grid[i][j] = default_value;
        }
    }
    return grid;
}

// Cleans up the memory for a 2D grid. The size of the grid is expected to be `grid_size`.
template<typename T>
void Fractal::cleanUpGrid(T** grid) {
    if (grid) {
        for (int i = 0; i < grid_size; i++) {
            delete[] grid[i];
        }
        delete[] grid;
    }
}

void Fractal::SaveHTD(std::string FilePath) 
{
    std::ofstream HTDFile;
    HTDFile.open(FilePath, std::ios::binary);
    unsigned short Size = static_cast<unsigned short>(grid_size);
    int PointCounter = grid_size * grid_size;
    HTDFile.write((char*)&PointCounter, sizeof(PointCounter));

    std::ofstream HTDGFile;
    HTDGFile.open(FilePath + "G", std::ios::binary);
    HTDGFile.write((char*)&PointCounter, sizeof(PointCounter));
    float DeadHeight = 0.f;
    for (int h = 0; h < grid_size; h++)
    {
        for (int w = 0; w < grid_size; w++)
        {
            float Height = grid[w][h] * 25.f;
            HTDFile.write((char*)&Height, sizeof(Height));
            HTDGFile.write((char*)&DeadHeight, sizeof(float));
        }
    }
    HTDFile.close();
    HTDGFile.close();
}

void Fractal::CreateTerrain(TerrainWorldPtr world, int Size)
{
    auto start = std::chrono::steady_clock::now();
    struct PointInfos
    {
        int BL = -1;
        int BR = -1;
        int TL = -1;
        int TR = -1;
        float Height;
        bool Used = false;
        NiColorA VertexColor = NiColorA(0.82f,0.82f,0.82f, 1.0f);
    };
    struct Triangle
    {
        unsigned short one;
        unsigned short two;
        unsigned short three;
    };
    std::vector<std::vector<PointInfos>> VertexMap;
    float minh = 0.0f;
    float mhax = 0.0f;
    for (int w = 0; w < grid_size; w++)
    {
        VertexMap.push_back(std::vector<PointInfos>(grid_size));
        for (int h = 0; h < grid_size; h++)
        {
            VertexMap[w][h].Height = grid[w][h] * 25.f;
            if (grid[w][h] < minh)
                minh = grid[w][h];
            if (grid[w][h] > mhax)
                mhax = grid[w][h];
        }
    }
    std::unordered_map<unsigned int, NiColorA> height_color_map;
    height_color_map.insert(std::pair<unsigned int, NiColorA>(0, NiColorA((unsigned char)0x00,0xFA, 0x9A, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(1, NiColorA((unsigned char)0x00,0xFF, 0x66, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(2, NiColorA((unsigned char)0x00, 0xFF, 0x33, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(3, NiColorA((unsigned char)0x0, 0x0FF, 0x00, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(4, NiColorA((unsigned char)0x00, 0xFF, 0x00, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(5, NiColorA((unsigned char)0x00, 0xFA, 0x9A, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(6, NiColorA((unsigned char)0xD1, 0xE2, 0x31, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(7, NiColorA((unsigned char)0xCD, 0xC6, 0x73, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(8, NiColorA((unsigned char)0x8B, 0x69, 0x14, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(9, NiColorA((unsigned char)0x8B, 0x45, 0x00, 0xFF)));
    height_color_map.insert(std::pair<unsigned int, NiColorA>(10, NiColorA((unsigned char)0x5E, 0x26, 0x05, 0xFF)));
    for (int w = 0; w < grid_size; w++)
    {
        for (int h = 0; h < grid_size; h++)
        {
            float normalized = normalize(grid[w][h], minh, mhax) * 10.0f;
            auto round = static_cast<unsigned int>(std::round(normalized));
            VertexMap[w][h].VertexColor = height_color_map.at(round);
        }
    }
    for (int BlockX = 0; BlockX < (grid_size - 1) / QuadsHeight; BlockX++) //19
    {
        for (int BlockY = 0; BlockY < (grid_size - 1) / QuadsWidth; BlockY++) //19
        {
            std::vector<NiPoint3> VerticesList;
            std::vector<std::pair<int, int>> WHList;
            std::vector<NiPoint3> NormalList;
            std::vector<NiColorA> ColorList;
            std::vector<Triangle> TriangleList;
            int TriCt = 0;
            for (int h = 0; h < QuadsHeight; h++)
            {
                for (int w = 0; w < QuadsWidth; w++)
                {
                    int ActiveW = BlockX * QuadsWidth + w;
                    int ActiveH = BlockY * QuadsHeight + h;
                    int PixelW = ActiveW;
                    int PixelH = ActiveH;

                    PointInfos* PixelInfo = &VertexMap[PixelW][PixelH];
                   

                    PointInfos* info = &VertexMap[ActiveW][ActiveH];
                    info->Used = true;
                    bool AddBL = true;
                    bool AddBR = true;
                    bool AddTL = true;
                    bool AddTR = true;

                    if (h != 0 || w != 0)
                    {
                        if (w != 0)
                        {
                            PointInfos* PointInfosLeft = &VertexMap[ActiveW - 1][ActiveH];
                            if (PointInfosLeft->Used)
                            {
                                info->BL = PointInfosLeft->BR;
                                info->TL = PointInfosLeft->TR;
                                AddBL = false;
                                AddTL = false;
                            }
                        }
                        if (h != 0)
                        {
                            PointInfos* LowerInfos = &VertexMap[ActiveW][ActiveH - 1];
                            if (LowerInfos->Used)
                            {
                                info->BL = LowerInfos->TL;
                                info->BR = LowerInfos->TR;
                                AddBL = false;
                                AddBR = false;
                            }
                            if (w != 0)
                            {
                                PointInfos* LowerInfosL = &VertexMap[ActiveW - 1][ActiveH - 1];
                                if (LowerInfosL->Used)
                                {
                                    info->BL = LowerInfosL->TR;
                                    AddBL = false;
                                }
                            }
                            if (ActiveW + 1 < grid_size)
                            {
                                PointInfos* LowerInfosR = &VertexMap[ActiveW + 1][ActiveH - 1];
                                if (LowerInfosR->Used)
                                {
                                    info->BR = LowerInfosR->TL;
                                    AddBR = false;
                                }
                            }
                        }
                    }


                    if (AddBL)
                    {
                        info->BL = TriCt;
                        TriCt++;
                        VerticesList.push_back(NiPoint3(ActiveW * BlockWidth, ActiveH * BlockHeight, info->Height));
                        WHList.push_back({ ActiveW,ActiveH });
                        NormalList.push_back(NiPoint3(0.0f,0.0f,1.0f));
                        ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
                    }
                    if (AddBR)
                    {
                        info->BR = TriCt;
                        TriCt++;
                        VerticesList.push_back(NiPoint3((ActiveW + 1) * BlockWidth, ActiveH * BlockHeight, VertexMap[ActiveW + 1][ActiveH].Height));
                        WHList.push_back({ ActiveW + 1,ActiveH });
                        NormalList.push_back(NiPoint3(0.0f,0.0f,1.0f));
                        ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
                    }
                    if (AddTL)
                    {
                        info->TL = TriCt;
                        TriCt++;
                        VerticesList.push_back(NiPoint3(ActiveW * BlockWidth, (ActiveH + 1) * BlockHeight, VertexMap[ActiveW][ActiveH + 1].Height));
                        WHList.push_back({ ActiveW,ActiveH + 1 });
                        NormalList.push_back(NiPoint3(0.0f,0.0f,1.0f));
                        ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
                    }
                    if (AddTR)
                    {
                        info->TR = TriCt;
                        TriCt++;
                        VerticesList.push_back(NiPoint3((ActiveW + 1) * BlockWidth, (ActiveH + 1) * BlockHeight, VertexMap[ActiveW + 1][ActiveH + 1].Height));
                        WHList.push_back({ ActiveW + 1,ActiveH + 1 });
                        NormalList.push_back(NiPoint3(0.0f,0.0f,1.0f));
                        ColorList.push_back(VertexMap[ActiveW][ActiveH].VertexColor);
                    }
                    TriangleList.push_back(Triangle(info->BL, info->BR, info->TL));
                    TriangleList.push_back(Triangle(info->TL, info->BR, info->TR));
                    
                }
            }

            if (!VerticesList.size() > 0)
                continue;
            /*Create NiTriStripsData*/
            /*Create RenderMaps*/

            NiPoint3* pkVertix = NiNew NiPoint3[(int)VerticesList.size()];
            NiPoint3* pkNormal = NiNew NiPoint3[(int)NormalList.size()];
            NiColorA* pkColor = NiNew NiColorA[(int)ColorList.size()];
            NiPoint2* pkTexture = nullptr;

            unsigned short* pusTriList = (unsigned short*)NiAlloc(char, 12 * (TriangleList.size() / 2));// NiNew NiPoint3[TriangleList.size() / 2];

            memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));
            memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
            memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
            memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));

            NiTriShapeDataPtr data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 0, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
            NiTriShapePtr Shape = NiNew NiTriShape(data);

            Shape->SetSortObject(false);

            NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
            NiMaterialPropertyPtr mat = NiNew  NiMaterialProperty;
            mat->SetAmbientColor(NiColor::BLACK);
            mat->SetDiffuseColor(NiColor::BLACK);
            mat->SetSpecularColor(NiColor::BLACK);
            mat->SetEmittance(NiColor::BLACK);
            NiVertexColorPropertyPtr mat2 = NiNew NiVertexColorProperty;
            mat2->SetSourceMode(NiVertexColorProperty::SourceVertexMode::SOURCE_AMB_DIFF);
            mat2->SetLightingMode(NiVertexColorProperty::LIGHTING_E_A_D);
            Shape->AttachProperty(mat);
            Shape->AttachProperty(mat2);

            Shape->CalculateNormals();

            //Shape->ApplyAndSetActiveMaterial("PgTerrain");
            Shape->Update(0.0);
            Shape->UpdateEffects();
            Shape->UpdateProperties();
            Shape->Update(0.0);
            Shape->SetConsistency(NiGeometryData::Consistency::MUTABLE);

            world->AttachGroundTerrain(Shape);
        }

    }
    auto diff = std::chrono::steady_clock::now() - start;
    std::ostringstream oss;
    oss << "Successfully Created New Map ("
        << std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
    LogInfo(oss.str());
}