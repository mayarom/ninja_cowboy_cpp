#pragma once

#include <string>

namespace ariel
{
    class Point
    {
    private:
        double P_x, P_y;

    public:
        // Constructor
        Point(double P_x = 0, double P_y = 0);

        // Calculate the distance between two points
        double distance(const Point &p) const;

        // Get a string representation of the point
        std::string print() const;

        // Get the x-coordinate of the point
        double whatX() const;

        // Get the y-coordinate of the podouble calculateDeltaY(const Point &point1, const Point &point2);int
        double whatY() const;

        // Set the x-coordinate of the point
        void updateX(double P_x);

        // Set the y-coordinate of the point
        void updateY(double P_y);

        // Move towards a point by a specified distance
        static Point moveTowards(const Point &point1, const Point &point2, double dist);

        // Compare two points and check if they are the same
        bool compare(const Point &other) const;

    private:
        // Helper function to validate the distance parameter
        static void validateDistance(double dist);

        // Helper function to check if two points are within a specified distance
        static bool isWithinDistance(const Point &point1, const Point &point2, double dist);

        // Helper function to calculate the new x-coordinate when moving towards a point
        static double calculateNewX(const Point &point1, const Point &point2, double dist);

        // Helper function to calculate the new y-coordinate when moving towards a point
        static double calculateNewY(const Point &point1, const Point &point2, double dist);

        // Helper function to create a new Point object with the given coordinates
        static Point createNewPoint(double x, double y);
    };
}
