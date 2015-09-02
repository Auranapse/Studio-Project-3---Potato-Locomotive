/****************************************************************************/
/*!
\file Maploader.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Handles loading a map into a 2D vector
!*/
/****************************************************************************/
#include "maploader.h"

/****************************************************************************/
/*!
\brief
Default constructor
!*/
/****************************************************************************/
MapLoader::MapLoader() : map_height(90), map_width(160)
{
	FILE_LOCATION = "GameData//Maps//001.csv";
}

/****************************************************************************/
/*!
\brief
Default destructor
!*/
/****************************************************************************/
MapLoader::~MapLoader()
{
	for (unsigned i = 0; i < map_data.size(); ++i)
	{
		for (unsigned j = 0; j < map_data[i].size(); ++j)
		{
			std::vector<std::string>().swap(map_data[i]);
		}
		std::vector<std::vector<std::string>>().swap(map_data);
	}
}

/****************************************************************************/
/*!
\brief
check if file exists
\param file
the file location to check for
!*/
/****************************************************************************/
bool MapLoader::fileExists(std::string file)
{
	std::ifstream infile(file);
	return infile.good();
}

/****************************************************************************/
/*!
\brief
loads the map from a file into a 2D vector
\param file
the file location to load map from
!*/
/****************************************************************************/
bool MapLoader::loadMap(std::string file)
{
	if (!fileExists(file))
	{
		return false;
	}

	FILE_LOCATION = file;

	for (unsigned i = 0; i < map_data.size(); ++i)
	{
		for (unsigned j = 0; j < map_data[i].size(); ++j)
		{
			std::vector<std::string>().swap(map_data[i]);
		}
		std::vector<std::vector<std::string>>().swap(map_data);
	}

	std::fstream mapData;
	mapData.open(FILE_LOCATION);

	std::string Line;
	while (std::getline(mapData, Line, '\n'))
	{
		std::istringstream csvStream(Line);
		std::vector<std::string> Column;
		std::string Element;

		while (std::getline(csvStream, Element, ','))
		{
			Column.push_back(Element);
		}
		map_width = Column.size();
		map_data.push_back(Column);

		std::vector<std::string>().swap(Column);
	}	

	worldSize = static_cast<float>(std::stoi(map_data[0][0]));
	worldHeight = static_cast<float>(std::stoi(map_data[0][1]));

	map_height = map_data.size();
	return true;
}

/****************************************************************************/
/*!
\brief
outputs the 2D vector into a file
\param file
the file location to save the map to
!*/
/****************************************************************************/
bool MapLoader::saveMap(std::string file)
{
	if (!fileExists(file))
	{
		std::fstream filecreate;
		filecreate.open(file, std::fstream::out);
		filecreate.close();
	}

	std::fstream mapFile;
	mapFile.open(file);

	for (unsigned i = 0; i < map_data.size(); ++i)
	{
		for (unsigned j = 0; j < map_data[i].size(); ++j)
		{
			mapFile << map_data[i][j];

			if (j != map_data[i].size() - 1)
			{
				mapFile << ",";
			}
		}
		mapFile << "\n";
	}

	mapFile.close();
	return true;
}