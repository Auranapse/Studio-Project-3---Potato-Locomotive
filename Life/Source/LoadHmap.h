#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H

#include <vector>
bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

#endif