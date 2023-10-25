#include "point.hpp"

Point::Point()
{
    Point(0.0, 0.0);
}

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
    this->stopping = false;
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

void Point::setStopping(bool stopping) {
    this->stopping = stopping;
}

bool Point::isStopping() {
    return stopping;
}
