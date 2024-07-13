#include "Fractal.h"
#include <random>

NiSmartPointer(DiamondSquareFractal);
class DiamondSquareFractal : public Fractal 
{
private:
    /*long randRange(long min, long max, int seed) {
        std::random_device device;
        if (seed) {
            seed = device();
        }
        std::mt19937 generator(seed);
        std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);
        return distribution(generator);
    }*/
    float randRange(float min, float max, int seed) {
        constexpr long MIN = 0L;
        constexpr long MAX = 100L;
        return (distribution(generator) / (float)MAX)* (max - min) + min;
        long initial_random = distribution(generator);
        return ((float)initial_random / (float)MAX) * (max - min) + min;
    }
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
public:
    DiamondSquareFractal(int grid_size = 129) : Fractal(grid_size)
    {
    }
    void generateGrid(int grid_size, int seed, float noise, float random_min, float random_max) 
    {
        auto start = std::chrono::steady_clock::now();

        std::random_device device;
        if (!seed) {
            seed = device();
        }
        generator = std::mt19937(seed);
        distribution = std::uniform_real_distribution<float>(random_min, random_max);
        

        CreateTexture(grid_size);
        Fractal::generateGrid(grid_size, seed, noise, random_min, random_max);

        float s0, s1, s2, s3, d0, d1, d2, d3, cn;
        int s = grid_size - 1;
        float mod_noise = 0.0f;
        for (int i = s; i > 1; i /= 2) {
            // reduce the random range at each step
            mod_noise = (random_max - random_min) * noise * ((float)i / (float)s);

            // diamonds
            for (int y = 0; y < s; y += i) {
                for (int x = 0; x < s; x += i) {
                    s0 = grid[x][y];
                    s1 = grid[x + i][y];
                    s2 = grid[x][y + i];
                    s3 = grid[x + i][y + i];

                    // cn
                    grid[x + (i / 2)][y + (i / 2)] =
                        ((s0 + s1 + s2 + s3) / 4.0f) + randRange(-mod_noise, mod_noise, seed);
                }
            }

            // squares
            for (int y = 0; y < s; y += i) {
                for (int x = 0; x < s; x += i) {
                    s0 = grid[x][y];
                    s1 = grid[x + i][y];
                    s2 = grid[x][y + i];
                    s3 = grid[x + i][y + i];
                    cn = grid[x + (i / 2)][y + (i / 2)];

                    d0 = y <= 0 ? (s0 + s1 + cn) / 3.0f : (s0 + s1 + cn + grid[x + (i / 2)][y - (i / 2)]) / 4.0f;
                    d1 = x <= 0 ? (s0 + cn + s2) / 3.0f : (s0 + cn + s2 + grid[x - (i / 2)][y + (i / 2)]) / 4.0f;
                    d2 = x >= s - i ? (s1 + cn + s3) / 3.0f :
                        (s1 + cn + s3 + grid[x + i + (i / 2)][y + (i / 2)]) / 4.0f;
                    d3 = y >= s - i ? (cn + s2 + s3) / 3.0f :
                        (cn + s2 + s3 + grid[x + (i / 2)][y + i + (i / 2)]) / 4.0f;

                    grid[x + (i / 2)][y] = d0 + randRange(-mod_noise, mod_noise, seed);
                    grid[x][y + (i / 2)] = d1 + randRange(-mod_noise, mod_noise, seed);
                    grid[x + i][y + (i / 2)] = d2 + randRange(-mod_noise, mod_noise, seed);
                    grid[x + (i / 2)][y + i] = d3 + randRange(-mod_noise, mod_noise, seed);
                }
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

