#include "Fractal.h"
#include <random>
#include "PerlinNoise.hpp"

NiSmartPointer(PerlinFractal);
class PerlinFractal : public Fractal
{
private:
    int _Octave = 10;
    float _Persistance = 0.98f;
public:
    PerlinFractal(int Octave = 10, int grid_size = 129, float Perstistance = 0.98f) : Fractal(grid_size) {
        _Octave = Octave;
        _Persistance = Perstistance;
    }
    void generateGrid(int grid_size, int seed, float noise, float random_min, float random_max)
    {
        auto start = std::chrono::steady_clock::now();

        siv::PerlinNoise perlin{ (unsigned int)seed };


        CreateTexture(grid_size);
        Fractal::generateGrid(grid_size, seed, noise, random_min, random_max);

        for (int w = 0; w < grid_size; w++) 
        {
            for (int h = 0; h < grid_size; h++) 
            {
                grid[w][h] = perlin.octave2D_01(static_cast<float>(w) / grid_size, static_cast<float>(h) / grid_size, _Octave, _Persistance) * (random_max - random_min) + random_min;
            }
        }


        applyThermalErosion();

        auto diff = std::chrono::steady_clock::now() - start;
        std::ostringstream oss;
        oss << "Successfully Diamond Grid ("
            << std::round(std::chrono::duration<double, std::milli>(diff).count()) << "ms)";
        LogInfo(oss.str());
    }
};

