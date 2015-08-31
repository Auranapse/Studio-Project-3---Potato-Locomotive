/****************************************************************************/
/*!
\file Maploader.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Handles loading a map into a 2D vector
!*/
/****************************************************************************/
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

	unsigned int map_height;
	unsigned int map_width;
	float worldSize;
	float worldHeight;

	bool fileExists(std::string file);
	bool loadMap(std::string file);
	bool saveMap(std::string file);
	std::vector<std::vector<std::string>> map_data;
};
#endif