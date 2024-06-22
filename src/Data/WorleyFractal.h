#include "Fractal.h"
#include <random>
#include "../Algorithms/worley.h"

NiSmartPointer(WorleyFractal);
class WorleyFractal : public Fractal
{
private:
    DistType Distance = DistType::euclidean;
    int seed = 123456u;
    int _NumPoints = 10;
    WorleyGrid wg;
    int _PointLoc = 0;
public:
    WorleyFractal(int NumPoints, int PointLoc , int Seed = 0, int grid_size = 129) : Fractal(grid_size), wg(Distance, Seed)
    {
        _NumPoints = NumPoints;
        _PointLoc = PointLoc;
    }
    void generateGrid(int grid_size, int seed, float noise, float random_min, float random_max)
    {
        auto start = std::chrono::steady_clock::now();

        wg.resetPoints();
        wg.addRandomPoints(_NumPoints);


        CreateTexture(grid_size);
        Fractal::generateGrid(grid_size, seed, noise, random_min, random_max);

        std::vector<double> dists;
        for (int w = 0; w < grid_size; w++)
        {
            for (int h = 0; h < grid_size; h++)
            {
                wg.getNearestPoints(static_cast<double>(w) / grid_size, static_cast<double>(h) / grid_size, dists);
                grid[w][h] = dists[_PointLoc] * (random_max - random_min) + random_min * noise;
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

