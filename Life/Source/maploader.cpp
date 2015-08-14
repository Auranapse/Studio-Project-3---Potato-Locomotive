#include "maploader.h"

MapLoader::MapLoader() : map_height(90), map_width(160)
{
	FILE_LOCATION = "GameData//Maps//001.csv";
}

MapLoader::~MapLoader()
{
	std::vector<std::vector<std::string>>().swap(map_data);
}

bool MapLoader::fileExists(std::string file)
{
	std::ifstream infile(file);
	return infile.good();
}

bool MapLoader::loadMap(std::string file)
{
	if(!fileExists(file))
	{
		return false;
	}

	FILE_LOCATION = file;
	if(map_data.size() > 0)
	{
		std::vector<std::vector<std::string>>().swap(map_data);
	}

	std::fstream mapData;
	mapData.open(FILE_LOCATION);
	
	std::string Line;
	while(std::getline(mapData, Line, '\n'))
	{
		std::istringstream csvStream(Line);
		std::vector<std::string> Column;
		std::string Element;

		while(std::getline(csvStream, Element, ','))
		{
			Column.push_back(Element);
		}
		map_width = Column.size();
		map_data.push_back(Column);
		std::vector<std::string>().swap(Column);
	}
	
	map_height = map_data.size();
	return true;
}