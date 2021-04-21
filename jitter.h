#ifndef JITTER_H
#define JITTER_H

#include "color.h"
#include <cstdlib>
#include <random>
#include <map>
#include <iostream>
#include "utils.h"

using std::map;
using std::pair;
using std::cout;
using std::cerr;

/**
 * Creates a 2D array indicating where to take samples within a pixel, using multi-jittered sampling. 
 * @param fine_grid the overall size of the grid. The coarse grid is sqrt(fine_grid) x sqrt(fine_grid)
 * @return a fine_grid x fine_grid boolean array, where True represents the position to take a sample at.
 **/
inline bool** get_multi_jitter_mask(int fine_grid) {
    int coarse_size = (int) sqrt(fine_grid);
     
    map<int, bool> row;
    map<int, bool> col;

    // initialize 2D array
    bool** sample;
    sample = new bool*[fine_grid];
    for (int i = 0; i < fine_grid; i++) {
        sample[i] = new bool[fine_grid];
        for (int j = 0; j < fine_grid; j++) {
            sample[i][j] = false;
        }
    }

    // for each coarse grid cell, choose a sample and make sure the row and col are unique
    for (int i = 0; i < coarse_size; i++) {
        for (int j = 0; j < coarse_size; j++) {
            int x;
            int y;
            do {
                x = random_int(0, coarse_size) + (i * coarse_size);
            } while (row.count(x) > 0);
            row.insert(pair<int,bool>(x,true));
            
            do {
                y = random_int(0, coarse_size) + (j * coarse_size);
            } while (col.count(y) > 0);
            col.insert(pair<int,bool>(y,true));
            
            sample[x][y] = true;
        }
    }
    return sample;
}

/**
 * Print a ppm file to display the multi-jitter sample grid, where black pixels represent the points to take a sample at. 
 * This is purely for visualization and testing purposes.
 * @param fine_grid the size of the resulting image.
 **/
inline void display_jitter_mask(int fine_grid) {
    cout << "P3\n" << fine_grid << ' ' << fine_grid << "\n255\n";
    bool** sample = get_multi_jitter_mask(fine_grid);
        
    for (int j = 0; j < fine_grid; ++j) {
        cerr << "\rScanlines done: " << j << ' ' << std::flush;
        for (int i = 0; i < fine_grid; ++i) {
            if (sample[i][j]) {
                write_color(cout, color(0,0,0));
            } else {
                write_color(cout, color(1,1,1));
            }
        }
    }
}

#endif