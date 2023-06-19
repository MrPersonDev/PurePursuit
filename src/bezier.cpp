#include "bezier.hpp"

const double Bezier::DELTA = 0.1;

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

std::vector<std::pair<double, double>> Bezier::bezierCurve(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4)
{
    std::vector<std::pair<double, double>> curve;

    for (double t = 0; t <= 1.001; t += DELTA)
        curve.push_back(bezier(p1, p2, p3, p4, t));
    
    return curve;
}
