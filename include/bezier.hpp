#include <SDL2/SDL.h>
#include <vector>
#include <utility>

class Bezier
{
    public:
        static std::pair<double, double> bezier(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4, double t);
        static std::pair<double, double> normalize(std::pair<double, double> point);
        static std::pair<double, double> getVelocityPoint(std::vector<std::pair<double, double>> curve);
        static std::vector<std::pair<double, double>> bezierCurve(std::pair<double, double> p1, std::pair<double, double> p2, std::pair<double, double> p3, std::pair<double, double> p4);

    private:
        static const double DELTA;
        static const double VELOCITY;
};