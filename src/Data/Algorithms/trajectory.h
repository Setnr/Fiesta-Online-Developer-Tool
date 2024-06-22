#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <random>

class Trajectory {
public:
    virtual void getPosition(double time, double& x, double& y) = 0;
};

class LinearTrajectory : public Trajectory {
public:
    LinearTrajectory(double& startX, double& startY,
                     std::default_random_engine& generator);

    virtual void getPosition(double time, double& x, double& y);
private:

    double startX, startY, endX, endY;
    
};

class EllipticalTrajectory : public Trajectory {
public:
    EllipticalTrajectory(double& startX, double& startY,
                         std::default_random_engine& generator);

    virtual void getPosition(double time, double &x, double &y);

private:
    double xRadius;
    double yRadius;
    double xCenter;
    double yCenter;
    
};

#endif
