// < begin copyright > 
// Copyright Ryan Marcus 2017
// 
// This file is part of worley.
// 
// worley is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// worley is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with worley.  If not, see <http://www.gnu.org/licenses/>.
// 
// < end copyright > 

#include <vector>
#include <random>
#include <memory>
#include <time.h>

enum DistType {
    euclidean, manhattan
};

struct Point {
    double x;
    double y;
};


class WorleyGrid {
public:
    WorleyGrid(DistType d, int seed = 0);

    void addRandomPoint();
    void addRandomPoints(unsigned int n);

    void resetPoints();

    void getNearestPoints(double x, double y,
                          std::vector<double>& dists);

    std::vector<Point> GetPoints() { return points; }
private:
    DistType D;

    std::default_random_engine generator;
    std::vector<Point> points;
    
};


