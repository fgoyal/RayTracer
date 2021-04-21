#ifndef MESH_H
#define MESH_H

#include "vec3.h"
#include "triangle.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class mesh {
    public: 
        mesh(const string filename, const color& kDiffuse, material* m);

        vector<vec3*> get_vertices() {
            return vertices;
        }
        
        vector<objs*> get_faces() {
            return faces;
        }

        void calculate_normals();

    public:
        vector<vec3*> vertices;
        vector<objs*> faces;
        vector<vec3*> indices;
};

/**
 * Constructor for mesh
 * @param filename: the obj file to load the mesh from
 * @param kDiffuse: the color to shade the mesh
 */
mesh::mesh(const string filename, const color& kDiffuse, material* m) {
    ifstream file(filename);
    string str;
    char a;
    float x, y, z;
    int i = 0;

    while (file >> a >> x >> y >> z) {
        if (a == 'v') {
            vertices.push_back(new vec3(x, y, z));
        }
        if (a == 'f') {
            triangle* t = new triangle(*vertices[x - 1], *vertices[y - 1], *vertices[z - 1], kDiffuse, m);
            faces.push_back(t);
            indices.push_back(new vec3(x - 1, y - 1, z - 1));
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
    
    for (int i = 0; i < faces.size(); i++) {
        vec3 normal = 0.5 * faces[i]->surface_normal(point3(0.0,0.0,0.0));
        vec3 index = *indices[i];

        normals[index.x()] = (normal) + normals[index.x()];
        normals[index.y()] = (normal) + normals[index.y()];
        normals[index.z()] = (normal) + normals[index.z()];
    }

    for (int i = 0; i < normals.size(); i++) {
        normals[i] = unit_vector(normals[i]);
    }

    // store the per vertex normal in the triangle
    for (int i = 0; i < faces.size(); i++) {
        triangle* t = (triangle*) faces[i];
        vec3* index = indices[i];
        t->set_vertex_normals(normals[index->x()], normals[index->y()], normals[index->z()]);
    }
}

#endif