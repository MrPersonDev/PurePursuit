#include "point.hpp"

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

Point::~Point() { }

std::pair<double, double> Point::getPos()
{
    return {x, y};
}

double Point::getX()
{
    return x;
}

double Point::getY()
{
    return y;
}

double Point::getCurvature()
{
    return curvature;
}

void Point::setCurvature(double curvature)
{
    this->curvature = curvature;
}
