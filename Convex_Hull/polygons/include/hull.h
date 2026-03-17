#ifndef POLYGONS_HULL_H
#define POLYGONS_HULL_H

#include <vector>

namespace polygons {

struct Point {
    double x;
    double y;
};

std::vector<Point> computeConvexHull(const std::vector<Point>& points);

} // namespace polygons

#endif // POLYGONS_HULL_H
