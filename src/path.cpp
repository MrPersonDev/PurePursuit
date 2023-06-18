#include "path.hpp"

const int Path::LINE_WIDTH = 3;
const int Path::POINT_SIZE = 5;
const SDL_Color Path::LINE_COLOR = {255, 0, 0, 255};
const SDL_Color Path::POINT_COLOR = {255, 255, 0, 255};
const SDL_Color Path::GOAL_POINT_COLOR = {0, 255, 0, 255};

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

std::pair<double, double> Path::getPoint(double x, double y, double lookAheadDist)
{
    for (int i = lastFoundIndex; i < points.size()-1; i++)
    {
        double lineX1 = points[i].first - x, lineY1 = points[i].second - y;
        double lineX2 = points[i+1].first - x, lineY2 = points[i+1].second - y;

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

        bool sol1Valid = validPoint(sol1X, sol1Y, points[i].first, points[i].second, points[i+1].first, points[i+1].second);
        bool sol2Valid = validPoint(sol2X, sol2Y, points[i].first, points[i].second, points[i+1].first, points[i+1].second);

        if (!(sol1Valid || sol2Valid))
            return points[lastFoundIndex];
        
        std::pair<double, double> sol1 = {sol1X, sol1Y}, sol2 = {sol2X, sol2Y};
        
        std::pair<double, double> goalPoint;
        if (!sol1Valid || !sol2Valid)
            goalPoint = sol1Valid ? sol1 : sol2;

        if (sol1Valid && sol2Valid)
        {
            double firstPointDist = pointToPointDist(sol1X, sol1Y, points[i+1].first, points[i+1].second);
            double secondPointdist = pointToPointDist(sol2X, sol2Y, points[i+1].first, points[i+1].second);
            
            goalPoint = firstPointDist < secondPointdist ? sol1 : sol2;
        }
        
        double goalPointDist = pointToPointDist(goalPoint.first, goalPoint.second, points[i+1].first, points[i+1].second);
        double currentDist = pointToPointDist(x, y, points[i+1].first, points[i+1].second);
        
        if (currentDist > goalPointDist)
        {
            lastFoundIndex = i;
            return goalPoint;
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

void Path::render(SDL_Renderer *renderer, double scale)
{
    for (int i = 0; i < (int)points.size()-1; i++)
    {
        std::pair<double, double> curPoint = points[i], nextPoint = points[i+1];
        thickLineRGBA(renderer, curPoint.first * scale, curPoint.second * scale, nextPoint.first * scale, nextPoint.second * scale, LINE_WIDTH, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, LINE_COLOR.a);
    }

    for (std::pair<double, double> point : points)
        filledCircleRGBA(renderer, point.first * scale, point.second * scale, POINT_SIZE, POINT_COLOR.r, POINT_COLOR.g, POINT_COLOR.b, POINT_COLOR.a);

    filledCircleRGBA(renderer, goalX * scale, goalY * scale, POINT_SIZE, GOAL_POINT_COLOR.r, GOAL_POINT_COLOR.g, GOAL_POINT_COLOR.b, GOAL_POINT_COLOR.a);
}

void Path::free()
{
    
}
