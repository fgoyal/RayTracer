#ifndef MESH_H
#define MESH_H

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>

#include "vec3.h"
#include "hittables/triangle.h"

using namespace std;


class mesh {
    public: 
        mesh(const string filename, shared_ptr<material> m);

        vector<vec3> get_vertices() {
            return vertices;
        }
        
        vector<shared_ptr<hittable>> get_faces() {
            vector<shared_ptr<hittable>> output;
            for (size_t i = 0; i < faces.size(); ++i) {
                output.push_back(faces[i]);
            }
            return output;
        }

        void calculate_normals();

    public:
        vector<vec3> vertices;
        vector<shared_ptr<triangle>> faces;
        vector<vec3> indices;
};

/**
 * Constructor for mesh
 * @param filename: the obj file to load the mesh from
 */
mesh::mesh(const string filename, shared_ptr<material> m) {
    ifstream file(filename);
    string str;
    char a;
    float x, y, z;
    int i = 0;

    while (file >> a >> x >> y >> z) {
        if (a == 'v') {
            vertices.push_back(vec3(x, y, z));
        }
        if (a == 'f') {
            auto t = make_shared<triangle>(vertices[x - 1], 
                                           vertices[y - 1],
                                           vertices[z - 1], m);
            faces.push_back(t);
            indices.push_back(vec3(x - 1, y - 1, z - 1));
        }
        i++;
    }
    
    calculate_normals();
}

/**
 * Compute the per vertex normals using area weighted averaging of the surrounding triangle faces
 */
void mesh::calculate_normals() {
    vector<vec3> normals(vertices.size());
    
    for (unsigned i = 0; i < faces.size(); i++) {
        vec3 normal = 0.5 * faces[i]->surface_normal(point3(0.0,0.0,0.0));
        vec3 index = indices[i];

        normals[index.x()] = (normal) + normals[index.x()];
        normals[index.y()] = (normal) + normals[index.y()];
        normals[index.z()] = (normal) + normals[index.z()];
    }

    for (unsigned i = 0; i < normals.size(); i++) {
        normals[i] = unit_vector(normals[i]);
    }

    // store the per vertex normal in the triangle
    for (unsigned i = 0; i < faces.size(); i++) {
        auto t = faces[i];
        auto index = indices[i];
        t->set_vertex_normals(normals[index.x()],
                              normals[index.y()],
                              normals[index.z()]);
    }
}

#endif