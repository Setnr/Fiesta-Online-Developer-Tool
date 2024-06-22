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
    void CreateTerrain(TerrainWorldPtr world, int Size);

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
                PixelColorA[i] = RGBAColor((grid[w][h] - min) / (max - min));
                i++;
            }
        }
        data->MarkAsChanged();
    }

    void CreateTexture(int grid_size) 
    {
        if (data && data->GetWidth() == grid_size)
            return;
        Show(false);
        data = NiNew NiPixelData(grid_size, grid_size, NiPixelFormat::RGBA32);
        NiTexture::FormatPrefs BlendPref;
        BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
        BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
        BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;
        pkTexture = NiSourceTexture::Create(data, BlendPref);
        pkTexture->SetStatic(false);
        pkScreenTexture = NiNew NiScreenTexture(pkTexture);
        pkScreenTexture->AddNewScreenRect(20,0, pkTexture->GetWidth(), pkTexture->GetHeight(), 0, 0);
        Show(_Show);
    }
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

   
    bool _Show = false;
    NiNodePtr Node;
    NiPixelDataPtr data;
    NiSourceTexturePtr pkTexture;
    NiScreenTexturePtr pkScreenTexture;
};

