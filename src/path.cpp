#include "path.hpp"

const int Path::LINE_WIDTH = 3;
const int Path::POINT_SIZE = 5;
const SDL_Color Path::LINE_COLOR = {255, 0, 0, 255};
const SDL_Color Path::POINT_COLOR = {255, 255, 0, 255};
const SDL_Color Path::GOAL_POINT_COLOR = {0, 255, 0, 255};
const double Path::MAX_VELOCITY_DIST = 10.0;

Path::Path()
{ 
    reset();
    resetPath();
}

Path::~Path()
{
    free();
}

void Path::reset()
{
    lastFoundIndex = 0;
    goalX = 0.0;
    goalY = 0.0;
}

void Path::resetPath()
{
    points.clear();
}

void Path::addPoint(double x, double y)
{
    points.push_back({x, y});
}

bool Path::hasPoint()
{
    return points.size() > 0;
}

Point Path::getVelocityPoint(int pointIdx, int direction)
{
    Point prevPoint = points[pointIdx-1], curPoint = points[pointIdx], nextPoint = points[pointIdx+1];
    Point directionPoint = Bezier::normalize(Point(nextPoint.getX() - prevPoint.getX(), nextPoint.getY() - prevPoint.getY()));
    Point pointInDirection = points[pointIdx + direction];

    double dist = pointToPointDist(pointInDirection.getX(), pointInDirection.getY(), curPoint.getX(), curPoint.getY());
    dist = std::min(MAX_VELOCITY_DIST, dist);
    double velocityPointX = direction * directionPoint.getX() * dist + curPoint.getX();
    double velocityPointY = direction * directionPoint.getY() * dist + curPoint.getY();

    Point velocityPoint(velocityPointX, velocityPointY);
    return velocityPoint;
}

void Path::smoothPoints()
{
    std::vector<Point> smoothedPoints;
    Point velocityPoint = {0.0, 0.0};
    for (int i = 0; i < points.size()-1; i++)
    {
        Point p1 = points[i], p2 = points[i];
        Point p3 = points[i+1], p4 = points[i+1];
        
        if (i > 0)
            p2 = getVelocityPoint(i, 1);
        if (i < points.size()-2)
            p3 = getVelocityPoint(i+1, -1);
        
        std::vector<Point> curve = Bezier::bezierCurve(p1, p2, p3, p4);
        for (int i = 0; i < curve.size(); i++)
            smoothedPoints.push_back(Point(curve[i].getX(), curve[i].getY()));
    
        Point velocityPointPair = Bezier::getVelocityPoint(curve);
        velocityPoint = Point(velocityPointPair.getX(), velocityPointPair.getY());
    }
    
    points = smoothedPoints;
}

void Path::setStopPoint()
{
    points.back().setStopping(true);
}

void Path::setPointCurvature()
{
    if (points.size() == 0)
        return;

    for (int i = 1; i < points.size(); i++)
    {
        Point prevPoint = points[i-1], curPoint = points[i], nextPoint = points[i+1];
        double dist1 = pointToPointDist(curPoint.getX(), curPoint.getY(), prevPoint.getX(), prevPoint.getY());
        double dist2 = pointToPointDist(curPoint.getX(), curPoint.getY(), nextPoint.getX(), nextPoint.getY());
        double dist3 = pointToPointDist(prevPoint.getX(), prevPoint.getY(), nextPoint.getX(), nextPoint.getY());
        
        double productOfSides = dist1 * dist2 * dist3;
        double semiPerimeter = (dist1 + dist2 + dist3) / 2.0;
        double triangleArea = sqrt(semiPerimeter * (semiPerimeter - dist1) * (semiPerimeter - dist2) * (semiPerimeter - dist3));
        
        double r = productOfSides / (4 * triangleArea);
        double curvature = isnan(1 / r) ? 0 : 1/r;
        points[i].setCurvature(curvature);
    }
    
    points[0].setCurvature(0);
    points[points.size()-1].setCurvature(0);
}

int Path::sign(double n)
{
    return n < 0 ? -1 : 1;
}

bool Path::validPoint(double pointX, double pointY, double x1, double y1, double x2, double y2)
{
    bool xValid = std::min(x1, x2) <= pointX && pointX <= std::max(x1, x2);
    bool yValid = std::min(y1, y2) <= pointY && pointY <= std::max(y1, y2);
    
    xValid |= abs(x1 - pointX) + abs(pointX - x2) < 0.0001;
    yValid |= abs(y1 - pointY) + abs(pointY - y2) < 0.0001;
    
    return xValid && yValid;
}

double Path::pointToPointDist(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

Point Path::getPoint(double x, double y, double lookAheadDist)
{
    if (lastFoundIndex >= (int)points.size()-2 && pointToPointDist(points.back().getX(), points.back().getY(), x, y) < lookAheadDist)
        return points.back();

    for (int i = lastFoundIndex; i < points.size()-1; i++)
    {
        double lineX1 = points[i].getX() - x, lineY1 = points[i].getY() - y;
        double lineX2 = points[i+1].getX() - x, lineY2 = points[i+1].getY() - y;

        double dx = lineX2 - lineX1, dy = lineY2 - lineY1;
        double dr = sqrt(pow(dx, 2) + pow(dy, 2));
        double D = lineX1 * lineY2 - lineX2 * lineY1;
        double discriminant = pow(lookAheadDist, 2) * pow(dr, 2) - pow(D, 2);

        if (discriminant < 0)
            return points[lastFoundIndex];
    
        double sol1X = (D * dy + sign(dy) * dx * sqrt(discriminant)) / pow(dr, 2) + x;
        double sol1Y = (-D * dx + abs(dy) * sqrt(discriminant)) / pow(dr, 2) + y;
        double sol2X = (D * dy - sign(dy) * dx * sqrt(discriminant)) / pow(dr, 2) + x;
        double sol2Y = (-D * dx - abs(dy) * sqrt(discriminant)) / pow(dr, 2) + y;

        bool sol1Valid = validPoint(sol1X, sol1Y, points[i].getX(), points[i].getY(), points[i+1].getX(), points[i+1].getY());
        bool sol2Valid = validPoint(sol2X, sol2Y, points[i].getX(), points[i].getY(), points[i+1].getX(), points[i+1].getY());

        if (!(sol1Valid || sol2Valid))
        {
            double dist1 = pointToPointDist(x, y, points[i].getX(), points[i].getY());
            double dist2 = pointToPointDist(x, y, points[i+1].getX(), points[i+1].getY());

            if (std::min(dist1, dist2) > lookAheadDist)
                return points[lastFoundIndex];
            else
                continue;
        }

        Point sol1 = Point(sol1X, sol1Y), sol2 = Point(sol2X, sol2Y);
        
        Point *goalPoint = NULL;
        if (!sol1Valid || !sol2Valid)
            goalPoint = sol1Valid ? &sol1 : &sol2;

        if (sol1Valid && sol2Valid)
        {
            double firstPointDist = pointToPointDist(sol1X, sol1Y, points[i+1].getX(), points[i+1].getY());
            double secondPointdist = pointToPointDist(sol2X, sol2Y, points[i+1].getX(), points[i+1].getY());
            
            goalPoint = firstPointDist < secondPointdist ? &sol1 : &sol2;
        }
        
        double goalPointDist = pointToPointDist(goalPoint->getX(), goalPoint->getY(), points[i+1].getX(), points[i+1].getY());
        double currentDist = pointToPointDist(x, y, points[i+1].getX(), points[i+1].getY());
        
        if (currentDist > goalPointDist)
        {
            lastFoundIndex = i;
            double curvature = (points[i].getCurvature() + points[i+1].getCurvature()) / 2.0;
            goalPoint->setCurvature(curvature);
            return *goalPoint;
        }
        else
            lastFoundIndex = i+1;
    }
    
    return points[lastFoundIndex];
}

void Path::setGoalPoint(double x, double y)
{
    goalX = x;
    goalY = y;
}

void Path::renderPath(SDL_Renderer *renderer, double scale)
{
    for (int i = 0; i < (int)points.size()-1; i++)
    {
        Point curPoint = points[i], nextPoint = points[i+1];
        thickLineRGBA(renderer, curPoint.getX() * scale, curPoint.getY() * scale, nextPoint.getX() * scale, nextPoint.getY() * scale, LINE_WIDTH, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, LINE_COLOR.a);
    }

    for (Point point : points)
        filledCircleRGBA(renderer, point.getX() * scale, point.getY() * scale, POINT_SIZE, POINT_COLOR.r, POINT_COLOR.g, POINT_COLOR.b, POINT_COLOR.a);
}

void Path::renderGoal(SDL_Renderer *renderer, double scale)
{
    filledCircleRGBA(renderer, goalX * scale, goalY * scale, POINT_SIZE, GOAL_POINT_COLOR.r, GOAL_POINT_COLOR.g, GOAL_POINT_COLOR.b, GOAL_POINT_COLOR.a);
}

void Path::free()
{
    
}
