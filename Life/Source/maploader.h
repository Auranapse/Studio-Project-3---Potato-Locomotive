#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class MapLoader
{
private:

	std::string FILE_LOCATION;

public:
	MapLoader();
	~MapLoader();

	int map_height;
	int map_width;

	bool fileExists(std::string file);
	bool loadMap(std::string file);
	std::vector<std::vector<std::string>> map_data;
	
};
#endif