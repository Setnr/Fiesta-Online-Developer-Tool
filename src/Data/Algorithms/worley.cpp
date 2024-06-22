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
 

#include "worley.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <chrono>
#include "unistd.h"
#include <stdlib.h>

#define MAX_DIST (sqrt(2.0))

WorleyGrid::WorleyGrid(DistType D, int seed) : D(D) {
    if(!seed)
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    else
        generator.seed(seed);
}

void WorleyGrid::addRandomPoint() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    Point ptr;
    ptr.x = dist(generator);
    ptr.y = dist(generator);

    points.push_back(ptr);
}

void WorleyGrid::addRandomPoints(unsigned int n) {
    for (unsigned int i = 0; i < n; i++)
        addRandomPoint();
}

void WorleyGrid::resetPoints() {
    points.clear();
}





void WorleyGrid::getNearestPoints(double x, double y,
                                  std::vector<double>& outVal) {
    outVal.resize(points.size());
    // sort the points by their distance to (x, y)
    for (size_t i = 0; i < points.size(); i++) {
        double d;
        switch (D) {
        case euclidean:
            d = pow(points[i].x - x, 2)
                + pow(points[i].y - y, 2);
            break;
        case manhattan:
            d = abs(points[i].x - x) + abs(points[i].y - y);
            break;
        default:
            fprintf(stderr, "Invalid distance enum option!\n");
            abort();
        }
        outVal[i] = d;
    }

    // next sort the outVals themselves.
    // TODO we could recover the sorted order from pointIDs
    std::sort(outVal.begin(), outVal.end());

    for (size_t i = 0; i < points.size(); i++) {
        outVal[i] = sqrt(outVal[i]);
    }
   
}

