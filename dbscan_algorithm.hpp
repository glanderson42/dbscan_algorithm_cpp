#ifndef DBSCAN_ALGORITHM_HPP
#define DBSCAN_ALGORITHM_HPP

#include <vector>
#include <cmath>

namespace dbs {

enum PointType {
    failure      = -3,
    noise        = -2,
    unclassified = -1,
    success      =  0,
    core_point   =  1,
    border_point =  2
};

struct Point {
    float x;
    float y;
    float z;
    int cluster_id;

    Point(float x, float y, float z = 0) : x(x), y(y), z(z) {}

    #ifdef OPENCV_CORE_HPP
    Point(cv::Point p) : x(p.x), y(p.y), z(0)   {}
    Point(cv::Point2f p) : x(p.x), y(p.y), z(0) {}
    Point(cv::Point2d p) : x(p.x), y(p.y), z(0) {}
    #endif
};

class DBScan {
private:
    std::vector<Point> points;
    unsigned int point_size;
    unsigned int min_points;
    float epsilon;
    int run();

public:
    DBScan() = delete;
    DBScan(std::vector<Point>&& input_array, unsigned int min_points, float epsilon);

    void apply(std::vector<Point>& output_array);
    std::vector<int> calculate_claster(Point point);
    int expand_cluster(Point point, int cluster_id);
    inline double calculate_distance(Point core, Point target);

    inline unsigned int get_total_point_size() { return point_size; }
    inline unsigned int get_min_points()       { return min_points; }
    inline float get_epsilon()                 { return epsilon;    }
};

} // dbs

#endif // DBSCAN_ALGORITHM_HPP