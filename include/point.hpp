#pragma once
#include <SDL2/SDL.h>

class Point
{
    public:
        Point();
        Point(double x, double y);
        ~Point();
        std::pair<double, double> getPos();
        double getX();
        double getY();
        double getCurvature();
        void setCurvature(double curvature);
        bool isStopping();
        void setStopping(bool stopping);

    private:
        double x, y;
        double curvature;
        bool stopping;
};