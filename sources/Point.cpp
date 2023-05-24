#include "Point.hpp"
#include <string>
#include <cmath>
#include <stdexcept>
#include <iomanip>

using namespace ariel;
using namespace std;

Point::Point(double P_x, double P_y) : P_x(P_x), P_y(P_y){};

double Point::distance(const Point &p) const
{
    // Calculate the differences in x and y coordinates
    double dx = P_x - p.whatX();
    double dy = P_y - p.whatY();

    // Calculate the Euclidean distance
    return sqrt(dx * dx + dy * dy);
}
std::string Point::print() const
{
    // Format the point coordinates as a string
    std::ostringstream out;
    out << "\033[1;33m" << std::fixed << std::setprecision(3) << "(" << P_x << ", " << P_y << ")"
        << "\033[0m";
    return out.str();
}

double Point::whatX() const
{
    return P_x;
}

double Point::whatY() const
{
    return P_y;
}

void Point::updateX(double P_x)
{
    this->P_x = P_x;
}

Point Point::moveTowards(const Point &point1, const Point &point2, double dist)
{
    validateDistance(dist);
    if (isWithinDistance(point1, point2, dist))
    {
        return point2;
    }
    else
    {
        double new_x = calculateNewX(point1, point2, dist);
        double new_y = calculateNewY(point1, point2, dist);
        return createNewPoint(new_x, new_y);
    }
}
void Point::updateY(double P_y)
{
    this->P_y = P_y;
}

void Point::validateDistance(double dist)
{
    if (dist < 0)
    {
        throw invalid_argument("Distance cannot be negative");
    }
}

bool Point::isWithinDistance(const Point &point1, const Point &point2, double dist)
{
    double d = point1.distance(point2);
    return (d <= dist);
}

double Point::calculateNewX(const Point &point1, const Point &point2, double dist)
{
    double dx = point2.whatX() - point1.whatX();
    double d = point1.distance(point2);
    double ratio = dist / d;
    return point1.whatX() + dx * ratio;
}

double Point::calculateNewY(const Point &point1, const Point &point2, double dist)
{
    double dy = point2.whatY() - point1.whatY();
    double d = point1.distance(point2);
    double ratio = dist / d;
    return point1.whatY() + dy * ratio;
}

Point Point::createNewPoint(double x_cordi, double y_cordi)
{
    return Point(x_cordi, y_cordi);
}

bool Point::compare(const Point &other) const
{
    return (P_x == other.P_x) && (P_y == other.P_y);
}
