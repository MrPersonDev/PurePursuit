#pragma once
#include <SDL2/SDL.h>

class Point
{
    public:
        Point(double x, double y);
        ~Point();
        std::pair<double, double> getPos();
        double getX();
        double getY();
        double getCurvature();
        void setCurvature(double curvature);

    private:
        double x, y;
        double curvature;
};