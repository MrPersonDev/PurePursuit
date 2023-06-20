#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <utility>

#include "point.hpp"

class Bezier
{
    public:
        static Point bezier(Point p1, Point p2, Point p3, Point p4, double t);
        static Point normalize(Point point);
        static Point getVelocityPoint(std::vector<Point> curve);
        static std::vector<Point> bezierCurve(Point p1, Point p2, Point p3, Point p4);

    private:
        static const double DELTA;
        static const double VELOCITY;
};