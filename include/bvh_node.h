#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "objs.h"
#include "vec3.h"
#include "ray.h"
#include "utils.h"
#include "aabb.h"
#include <algorithm>
#include <vector>
#include <cstdlib>

using std::vector;

class bvh_node : public objs {
    public: 
        bvh_node() {};
        bvh_node(const vector<objs*>& objects);

        std::string type() const {
            return "bvh node";
        }

        virtual color kDiffuse() const;
        virtual vec3 surface_normal(const point3 position) const;
        virtual bool ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const;
        virtual aabb bounding_box() const;

    public:
        objs* left;
        objs* right;
        aabb bbox;
};

/**
 * This function should never be used
 */
color bvh_node::kDiffuse() const {
    return color(-10,-10,-10);
}

/**
 * This function should never be used
 */
vec3 bvh_node::surface_normal(const point3 position) const {
    return vec3(-10.0,-10.0,-10.0);
}


bool bvh_node::ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const {
    if (!bbox.ray_intersection(r, tmin, tmax)) {
        return false;
    }
    
    double hit_left = left->ray_intersection(r, rec, tmin, tmax);
    double hit_right = right->ray_intersection(r, rec, tmin, hit_left ? rec.t : tmax);
    return hit_left || hit_right;
}


aabb bvh_node::bounding_box() const {
    return bbox;
}

/**
 * BVH node constructor
 * Recursively creates sub trees for both left and right sides
 * Uses the midpoint method to partition
 * @param objects: the list of objects to separate into subtrees
 * @param start: the starting index of objects to look at
 * @param end: the ending index of objects to look at
 */
bvh_node::bvh_node(const vector<objs*>& objects) {
    vector<objs*> objs_list = objects;
    if (objs_list.size() == 0) {
        return;
    }
    if (objs_list.size() == 1) {
        left = right = objs_list[0];
    } else if (objs_list.size() == 2) {
        right = objs_list[0];
        left = objs_list[1];
    } else {
        // Compute (xmin, ymin, zmin) and (xmax, ymax, zmax) for centroids
        double min[3];
        double max[3];
        bool first = true;
        for (unsigned o = 0; o < objs_list.size(); o++) {
            for (int i = 0; i < 3; i++) {
                double var = objs_list[o]->bounding_box().centroid()[i];
                if (first) {
                    min[i] = max[i] = var;
                } else {
                    if (var < min[i]) {
                        min[i] = var;
                    }
                    if (var > max[i]) {
                        max[i] = var;
                    }
                }
            }
            first = false;
        }

        // pick axis based on largest spread
        int axis = 0;
        double range = max[0] - min[0];
        double yrange = max[1] - min[1];
        double zrange = max[2] - min[2];
        if (yrange > range) {
            axis = 1;
            range = yrange;
        }

        if (zrange > range) {
            axis = 2;
            range = zrange;
        }

        // sort objects based on median split
        auto median_split = (max[axis] + min[axis]) / 2;
        vector<objs*> left_split;
        vector<objs*> right_split;
        for (unsigned o = 0; o < objs_list.size(); o++) {
            double curr = objs_list[o]->bounding_box().centroid()[axis];
            if (curr >= median_split) {
                right_split.push_back(objs_list[o]);
            } else {
                left_split.push_back(objs_list[o]);
            }
        }

        if (left_split.size() == 1) {
            left = left_split[0];
        } else {
            left = new bvh_node(left_split);
        }

        if (right_split.size() == 1) {
            right = right_split[0];
        } else {
            right = new bvh_node(right_split);
        }
    }

    aabb box_left = left->bounding_box();
    aabb box_right = right->bounding_box();
    
    bbox = surrounding_box(box_left, box_right);
}

#endif