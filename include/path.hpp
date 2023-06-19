#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <vector>
#include <utility>

#include "bezier.hpp"

class Path
{
    public:
        Path();
        ~Path();
        void reset();
        void resetPath();
        void addPoint(double x, double y);
        bool hasPoint();
        void smoothPoints();
        int sign(double n);
        bool validPoint(double pointX, double pointY, double x1, double y1, double x2, double y2);
        double pointToPointDist(double x1, double y1, double x2, double y2);
        std::pair<double, double> getPoint(double x, double y, double lookAheadDist);
        void setGoalPoint(double x, double y);
        void render(SDL_Renderer *renderer, double scale);
        void free();
        
    private:
        static const int LINE_WIDTH;
        static const int POINT_SIZE;
        static const SDL_Color LINE_COLOR;
        static const SDL_Color POINT_COLOR;
        static const SDL_Color GOAL_POINT_COLOR;
        std::vector<std::pair<double, double>> points;
        int lastFoundIndex;
        double goalX, goalY;
};