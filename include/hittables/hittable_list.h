/**
 * @file hittable_list.h
 * @author Ian Rudnick
 * A list of hittable objects to render together.
 */

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittables/hittable.h"
#include "ray.h"

class hittable_list : public hittable {
public:
	/**
	 * Constructs an empty hittable list.
	 */
    hittable_list() {}

	/**
	 * Constucts a hittable list and puts the given object in it.
	 * @param object The first object to add to the list.
	 */
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

	/**
	 * Removes all objects from the list.
	 */
    void clear() { objects_.clear(); }

	/**
	 * Adds an object to the list.
	 * @param object The object to add.
	 */
    void add(std::shared_ptr<hittable> object) { objects_.push_back(object); }

	/**
	 * Determines whether a ray hits any of the objects in the list, and what
	 * happens.
	 * @return Whether or not the ray hits any objects in the list.
	 */
    virtual bool hit(const ray& ray,
					 hit_record& record,
                     double tmin, double tmax) const override
	{
		hit_record temp_record;
		bool hit_anything = false;
		auto closest = tmax;

		// Go through the objects vector
		for (const auto & object : objects_) {
			if (object->hit(ray, temp_record, tmin, closest)) {
				hit_anything = true;
				closest = temp_record.t;
				record = temp_record;
			}
		}

		return hit_anything;
	}

	/**
	 * This function should not be a virtual function in the hittable class,
	 * but it is, so we have to implement it.
	 * @return An empty vector.
	 */
	virtual vec3 surface_normal(const point3 position) const override {
		return vec3(0, 0, 0);
	}
    
	/**
	 * Gets the axis-alligned bounding box surrounding all the objects in the
	 * list.
	 * @return The AABB of the list.
	 */
    virtual aabb bounding_box() const override {
		// If the list is empty, return an infinitely small volume
		// (empty AABB).
		if (this->objects_.empty()) {
			return aabb(point3(0, 0, 0), point3(0, 0, 0));
		}

		aabb output_box;
		aabb temp_box;
		bool first_box = true;

		for (const auto& object : this->objects_) {
			temp_box = object->bounding_box();
			if (first_box) {
				output_box = temp_box;
			}
			else {
				output_box = surrounding_box(output_box, temp_box);
			}
			first_box = false;
		}

		return output_box;
	}

	/**
	 * @return The string "hittable_list".
	 */
	virtual std::string type() const override {
		return "hittable_list";
	}

public:
    std::vector<std::shared_ptr<hittable>> objects_;

}; // class HittableList

#endif
