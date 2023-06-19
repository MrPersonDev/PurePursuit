#include "bezier.hpp"

const double Bezier::DELTA = 0.1;
const double Bezier::VELOCITY = 20.0;

std::pair<double, double> Bezier::bezier(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4, double t)
{
    double ax = pow(1 - t, 3) * p1.first;
    double ay = pow(1 - t, 3) * p1.second;
    double bx = 3 * t * pow(1 - t, 2) * p2.first;
    double by = 3 * t * pow(1 - t, 2) * p2.second;
    double cx = 3 * pow(t, 2) * (1 - t) * p3.first;
    double cy = 3 * pow(t, 2) * (1 - t) * p3.second;
    double dx = pow(t, 3) * p4.first;
    double dy = pow(t, 3) * p4.second;
    
    return {ax + bx + cx + dx, ay + by + cy + dy};
}

std::pair<double, double> Bezier::normalize(std::pair<double, double> point)
{
    double length = sqrt(pow(point.first, 2) + pow(point.second, 2));
    return {point.first / length, point.second / length};
}

std::pair<double, double> Bezier::getVelocityPoint(std::vector<std::pair<double, double>> curve)
{
    std::pair<double, double> lastPoint = curve[curve.size()-1], secondLastPoint = curve[curve.size()-2];
    std::pair<double, double> endDirection = normalize({lastPoint.first - secondLastPoint.first, lastPoint.second - secondLastPoint.second});
    return {endDirection.first * VELOCITY, endDirection.second * VELOCITY};
}

std::vector<std::pair<double, double>> Bezier::bezierCurve(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4)
{
    std::vector<std::pair<double, double>> curve;

    for (double t = 0; t <= 1.001; t += DELTA)
        curve.push_back(bezier(p1, p2, p3, p4, t));
    
    return curve;
}
