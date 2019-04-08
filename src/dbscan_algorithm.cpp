#include "../dbscan_algorithm.hpp"

dbs::DBScan::DBScan(std::vector<Point>&& input_array, 
                    unsigned int min_points,
                    float epsilon) : points(std::move(input_array)), min_points(min_points), epsilon(epsilon), point_size(input_array.size()) {}

int dbs::DBScan::run()
{
    int cluster_id = 1;
    for(auto it = std::begin(points); it != std::end(points); ++it) {
        if(it->cluster_id == PointType::unclassified) {
            if(expand_cluster(*it, cluster_id) != PointType::failure) {
                cluster_id += 1;
            }
        }
    }

    return 0;
}

void dbs::DBScan::apply(std::vector<Point>& output_array)
{
    run();
    output_array = points;
}

int dbs::DBScan::expand_cluster(Point point, int cluster_id)
{
    std::vector<int> cluster_seeds = calculate_claster(point);

    if(cluster_seeds.size() < min_points) {
        point.cluster_id = PointType::noise;
        return PointType::failure;
    }

    int idx = 0;
    int idx_core_pt = 0;

    for(auto it = std::begin(cluster_seeds); it != std::end(cluster_seeds); ++it) {
        points.at(*it).cluster_id = cluster_id;
        if(points.at(*it).x == point.x && points.at(*it).y == point.y && points.at(*it).z == point.z) {
            idx_core_pt = idx;
        }

        ++idx;
    }

    cluster_seeds.erase(std::begin(cluster_seeds) + idx_core_pt);

    for(int i = 0; i < cluster_seeds.size(); ++i) {
        std::vector<int> neighbors = calculate_claster(points.at(cluster_seeds[i]));

        if(neighbors.size() >= min_points) {
            for(auto it = std::begin(neighbors); it != std::end(neighbors); ++it) {
                if(points.at(*it).cluster_id == PointType::unclassified ||
                   points.at(*it).cluster_id == PointType::noise) {
                       if(points.at(*it).cluster_id == PointType::unclassified) {
                           cluster_seeds.push_back(*it);
                       }

                       points.at(*it).cluster_id = cluster_id;
                   }
            }
        }
    }
    return PointType::success;
}

std::vector<int> dbs::DBScan::calculate_claster(Point point)
{
    int idx = 0;
    std::vector<int> cluster_idx;
    for(auto it = std::begin(points); it != std::end(points); ++it) {
        if(calculate_distance(point, *it) <= epsilon) {
            cluster_idx.push_back(idx);
        }
        ++idx;
    }

    return cluster_idx;
}

inline double dbs::DBScan::calculate_distance(Point core, Point target)
{
    return (
        std::pow((core.x - target.x), 2) +
        std::pow((core.y - target.y), 2) +
        std::pow((core.z - target.z), 2)
    );
}