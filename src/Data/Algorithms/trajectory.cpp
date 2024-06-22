#include "trajectory.h"
#include <random>
#include "math.h"

LinearTrajectory::LinearTrajectory(double& sX, double& sY,
                       std::default_random_engine& generator) {
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    sX = dist(generator);
    sY = dist(generator);

    startX = sX;
    startY = sY;
    
    endX = dist(generator);
    endY = dist(generator);
}

void LinearTrajectory::getPosition(double t, double& x, double& y) {
    if (t == 0.0 || t == 1.0) {
        x = startX;
        y = startY;
        return;
    }

    double scaled = sin(3.1415 * t);
    x = startX + ((startX - endX) * scaled);
    y = startY + ((startY - endY) * scaled);
}


EllipticalTrajectory::EllipticalTrajectory(double& sX,
                                           double& sY,
                                           std::default_random_engine& generator) {
    std::uniform_real_distribution<double> dist(0.0, 0.5);

    xCenter = dist(generator) * 2;
    yCenter = dist(generator) * 2;
    xRadius = dist(generator);
    yRadius = dist(generator);

    getPosition(0, sX, sY);

}

void EllipticalTrajectory::getPosition(double t, double& x, double& y) {
    x = xRadius * cos(t * 2 * 3.1415) + xCenter;
    y = yRadius * sin(t * 2 * 3.1415) + yCenter;
}
