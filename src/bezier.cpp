#include "bezier.hpp"

const double Bezier::DELTA = 0.1;
const double Bezier::VELOCITY = 20.0;

Point Bezier::bezier(Point p1, Point p2, Point p3, Point p4, double t)
{
    double ax = pow(1 - t, 3) * p1.getX();
    double ay = pow(1 - t, 3) * p1.getY();
    double bx = 3 * t * pow(1 - t, 2) * p2.getX();
    double by = 3 * t * pow(1 - t, 2) * p2.getY();
    double cx = 3 * pow(t, 2) * (1 - t) * p3.getX();
    double cy = 3 * pow(t, 2) * (1 - t) * p3.getY();
    double dx = pow(t, 3) * p4.getX();
    double dy = pow(t, 3) * p4.getY();
    
    return Point(ax + bx + cx + dx, ay + by + cy + dy);
}

Point Bezier::normalize(Point point)
{
    double length = sqrt(pow(point.getX(), 2) + pow(point.getY(), 2));
    return {point.getX() / length, point.getY() / length};
}

Point Bezier::getVelocityPoint(std::vector<Point> curve)
{
    Point lastPoint = curve[curve.size()-1], secondLastPoint = curve[curve.size()-2];
    Point endDirection = normalize(Point(lastPoint.getX() - secondLastPoint.getX(), lastPoint.getY() - secondLastPoint.getY()));
    return {endDirection.getX() * VELOCITY, endDirection.getY() * VELOCITY};
}

std::vector<Point> Bezier::bezierCurve(Point p1, Point p2, Point p3, Point p4)
{
    std::vector<Point> curve;

    for (double t = 0; t <= 1.001; t += DELTA)
        curve.push_back(bezier(p1, p2, p3, p4, t));
    
    return curve;
}
