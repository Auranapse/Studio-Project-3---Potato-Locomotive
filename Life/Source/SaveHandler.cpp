/****************************************************************************/
/*!
\file SaveHandler.cpp
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
A save file handler to ensure easy saving and loading
!*/
/****************************************************************************/
#include "SaveHandler.h"

SaveHandler::SaveHandler() : FILE_LOCATION("GameData//GameData.GameData")
{
	
}

SaveHandler::~SaveHandler()
{
	
}

void SaveHandler::init(std::string file)
{
	file = FILE_LOCATION;

	std::ifstream infile(FILE_LOCATION);
	if (!infile.good())
	{
		std::fstream filecreate;
		filecreate.open(FILE_LOCATION, std::fstream::out);
		filecreate.close();
		saveData();
	}
	
	loadData();
}

void SaveHandler::assign(float &data, float default_data, unsigned int ID, bool save)
{
	std::string temp;
	if (ID > Data.size())
	{
		data = default_data;
		temp = std::to_string(static_cast<long double>(data));
		Data.push_back(temp);
	}

	else
	{
		if (save)
		{
			Data[ID - 1] = std::to_string(static_cast<long double>(data));
		}
		else
		{
			data = static_cast<float>(std::stoi(Data[ID - 1]));
		}
	}
}

void SaveHandler::assign(unsigned short &data, unsigned short default_data, unsigned int ID, bool save)
{
	std::string temp;
	if (ID > Data.size())
	{
		data = default_data;
		std::stringstream ss;
		ss << data;
		temp = ss.str();

		Data.push_back(temp);
	}

	else
	{
		if (save)
		{
			std::stringstream ss;
			ss << data;
			Data[ID - 1] = ss.str();
		}
		else
		{
			char CN = std::stoi(Data[ID - 1]);
			data = CN;
		}
	}
}

void SaveHandler::loadData(void)
{
	while (Data.size() > 0)
	{
		Data.pop_back();
	}

	std::fstream Savefile;
	Savefile.open(FILE_LOCATION);

	int fileSize;
	Savefile >> fileSize;

	std::string tempdata;

	for (int i = 0; i < fileSize; i++)
	{
		Savefile >> tempdata;
		Data.push_back(tempdata);
	}

	Savefile.close();
}

void SaveHandler::saveData(void)
{
	std::fstream SaveFile;
	SaveFile.open(FILE_LOCATION);

	SaveFile << Data.size();

	for (unsigned i = 0; i < Data.size(); ++i)
	{
		SaveFile << " ";
		SaveFile << Data[i];
	}

	SaveFile.close();
}