#pragma once
#include <NiMain.h>
#include<functional>
#include "../NiApplication/FiestaOnlineTool.h"
#include "../Data/TerrainWorld.h"

NiSmartPointer(Fractal);
struct Peak {
    float value;
    int x;
    int y;

    Peak(float value, int x, int y) : value{ value }, x{ x }, y{ y } {
    }
};

class Fractal : public NiRefObject {
protected:	
    struct RGBAColor
{
    RGBAColor(float c) {
        r = c * 0xFF;
        g = r;
        b = r;
        a = 0xFF;
    }
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

};
public:
    struct PointInfos
    {
        int BL = -1;
        int BR = -1;
        int TL = -1;
        int TR = -1;
        float Height;
        bool Used = false;
        NiColorA VertexColor = NiColorA(0.82f, 0.82f, 0.82f, 1.0f);
        bool Shadow = false;
    };
    explicit Fractal(int grid_size = 129);
    ~Fractal();
    virtual void generateGrid(int grid_size, int seed, float noise, float random_min = 0.0f, float random_max = 40.0f);
    
    void applyThermalErosion();

    void applyHydraulicErosion(float quantity, float sediment_factor);
    void applyThermalErosionTonNeighbour(float erosion_height);
    void Show(bool Show)
    {
        if (!pkScreenTexture)
            return;
        _Show = Show;
        if (_Show)
        {
            FiestaOnlineTool::AddScreenTexture(pkScreenTexture);
        }
        else
        {
            FiestaOnlineTool::RemoveScreenTexture(pkScreenTexture);
        }
    }
    void CreateTerrain(TerrainWorldPtr world, int Size, bool Shadow = false);
    bool ShowColorPickers();
    void UpdatePixelData(float min, float max) 
    {
        RGBAColor* PixelColorA = (RGBAColor*)data->GetPixels();
        int i = 0;
        int wmax = data->GetWidth();
        int hmax = data->GetHeight();
        for (int w = 0; w < wmax; w++)
        {
            for (int h = 0; h < hmax; h++)
            {
                PixelColorA[i] = RGBAColor((((grid[w][h] + grid[w+1][h+1]) / 2) - min) / (max - min));
                i++;
            }
        }
        data->MarkAsChanged();
    }

    void CreateTexture(int grid_size) 
    {
        if (data && data->GetWidth() == grid_size - 1)
            return;
        Show(false);
        data = NiNew NiPixelData(grid_size - 1, grid_size - 1, NiPixelFormat::RGBA32);
        NiTexture::FormatPrefs BlendPref;
        BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
        BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
        BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;
        pkTexture = NiSourceTexture::Create(data, BlendPref);
        pkTexture->SetStatic(false);
        if(!pkScreenTexture)
        {
            pkScreenTexture = NiNew NiScreenTexture(pkTexture);
            pkScreenTexture->AddNewScreenRect(20, 0, pkTexture->GetWidth(), pkTexture->GetHeight(), 0, 0);
        }
        Show(_Show);
    }
    void SaveHTD(std::string FilePath);
    void SaveShadows(std::string FilePath)
    {
        BMP bmp;
        bmp.SetSize(VertexMap.size(), VertexMap[0].size());
        for (int w = 0; w < VertexMap.size(); w++)
        {
            for (int h = VertexMap[0].size() - 1; h >= 0; h--)
            {
                RGBApixel pixel;
                pixel.Red = static_cast<unsigned char> (VertexMap[w][h].VertexColor.r * 255.f);
                pixel.Green = static_cast<unsigned char> (VertexMap[w][h].VertexColor.g * 255.f);
                pixel.Blue = static_cast<unsigned char> (VertexMap[w][h].VertexColor.b * 255.f);
                pixel.Alpha = static_cast<unsigned char> (VertexMap[w][h].VertexColor.a * 255.f);
                bmp.SetPixel(w, VertexMap[0].size() - h - 1, pixel);
            }
        }
        bmp.WriteToFile(FilePath.c_str());
    }
    NiSourceTexturePtr GetSourceTexture() { return pkTexture; }
protected:
    int grid_size;
    float BlockWidth = 50.f;
    float BlockHeight = 50.f;
    int QuadsWidth = 64;
    int QuadsHeight = 64;
    float** grid;
    float normalize(float value, float min, float max) {
        return (value - min) / (max - min);
    }

  
    bool isPeak(float value, int x, int y, bool upper = true);

    void applyHydraulicErosionFromPeak(const Peak& peak, bool** grid_checked, float sediment_factor,
        float** water_quantity, float** sediment_quantity);

    static float moveWater(float height, float neighbour_height, float water_quantity, float water_quantity_neighbour);

    void removeExcessWaterFromMargins(float** water_quantity);

    int countNeighbours(const Peak& peak);

    [[nodiscard]] bool isMargin(const Peak& peak, int i, int j) const;
  
    // utility methods
    template<typename T>
    T** createGrid(T default_value);

    template<typename T>
    void cleanUpGrid(T** grid);
    void CalculateRay(NiPoint3& StartPoint, NiPoint3& SunVector, std::vector<std::vector<bool>>& Shadowed);
    void CreateShadowMap(TerrainWorldPtr world);
        
    bool _Show = false;
    NiNodePtr Node;
    NiPixelDataPtr data;
    NiSourceTexturePtr pkTexture;
    NiScreenTexturePtr pkScreenTexture;
    
    std::vector<std::vector<PointInfos>> VertexMap;
    float minh = 0.0f;
    float mhax = 0.0f;

    static std::unordered_map<unsigned int, NiColorA> height_color_map;
    std::vector<std::vector<NiPoint3>> Normals;
    std::vector<std::vector<NiColorA>> Smoothed;
    bool SmoothShadows = true;

    void CreateNormalMap()
    {
        Normals = std::vector<std::vector<NiPoint3>>(grid_size, std::vector<NiPoint3>(grid_size));

        const float strength = 8.0f;
        const float dZ = 1.0 / strength;

        for (int w = 0; w < VertexMap.size(); w++)
        {
            for (int h = 0; h < VertexMap.size(); h++)
            {
                float tl = GetHeight(w - 1, h - 1); // top left
                float  l = GetHeight(w - 1, h);   // left
                float bl = GetHeight(w - 1, h + 1); // bottom left
                float  t = GetHeight(w, h - 1);   // top
                float  b = GetHeight(w, h + 1);   // bottom
                float tr = GetHeight(w + 1, h - 1); // top right
                float  r = GetHeight(w + 1, h);   // right
                float br = GetHeight(w + 1, h + 1); // bottom right

                // sobel filter
                const float dX = (tr + 2.0 * r + br) - (tl + 2.0 * l + bl);
                const float dY = (bl + 2.0 * b + br) - (tl + 2.0 * t + tr);

                NiPoint3 vec(dX, dY, dZ);
                vec.Unitize();
                vec.x += 1.f;
                vec.y += 1.f;
                vec.z += 1.f;
                vec = vec / 2;
                Normals[w][h] = vec;

            }
        }
    }

    inline float GetHeight(int w, int h)
    {
        if(w >= 0 && w < grid_size && h >= 0 && h < grid_size)
            return VertexMap[w][h].Height;

        if (w < 0)
            w = 0;
        if (w >= grid_size)
            w = grid_size - 1;

        if (h < 0)
            h = 0;

        if (h >= grid_size)
            h = grid_size - 1;

        return VertexMap[w][h].Height;
    }
    inline NiColorA GetColor(int w, int h)
    {
        w = (w < 0) ? 0 : (w >= grid_size ? grid_size - 1 : w);
        h = (h < 0) ? 0 : (h >= grid_size ? grid_size - 1 : h);

        return Smoothed[w][h];

        if (w >= 0 && w < grid_size && h >= 0 && h < grid_size)
            return Smoothed[w][h];

        if (w < 0)
            w = 0;
        if (w >= grid_size)
            w = grid_size - 1;

        if (h < 0)
            h = 0;

        if (h >= grid_size)
            h = grid_size - 1;

        return Smoothed[w][h];
    }

    NiColorA AmbientLight = NiColorA(98.f / 255.f, 192.f / 255.f, 255.f / 255.f, 1.f);
    NiColorA SunLight = NiColorA(255.f / 255.f, 244.f / 255.f, 222.f / 255.f, 1.f);

    NiPoint3 OldSunPos;
    int RayLoopCt;
};

