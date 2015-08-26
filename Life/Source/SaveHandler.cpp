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

/****************************************************************************/
/*!
\brief
Default constructor
!*/
/****************************************************************************/
SaveHandler::SaveHandler() : FILE_LOCATION("GameData//GameData.GameData")
{

}

/****************************************************************************/
/*!
\brief
Default destructor
!*/
/****************************************************************************/
SaveHandler::~SaveHandler()
{

}

/****************************************************************************/
/*!
\brief
initializes the save file, create one if not available, and load if available
\param file
the file name and location
!*/
/****************************************************************************/
void SaveHandler::init(std::string file)
{
	FILE_LOCATION = file;

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

/****************************************************************************/
/*!
\brief
Assign a particular value to the save file, for loading and saving.
\param data
the value of the data to keep
\param default_data
the data to default to if it doesn't exist in save
\param ID
the ID of the data for storing later
\param save
if true, will overwrite savefile, if false it'll load from savefile
!*/
/****************************************************************************/
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
			std::stringstream ss;
			ss.str("");
			ss << data << "\0";
			Data[ID - 1] = ss.str();
		}
		else
		{
			data = static_cast<float>(std::stoi(Data[ID - 1]));
		}
	}
}

/****************************************************************************/
/*!
\brief
Assign a particular value to the save file, for loading and saving.
\param data
the value of the data to keep
\param default_data
the data to default to if it doesn't exist in save
\param ID
the ID of the data for storing later
\param save
if true, will overwrite savefile, if false it'll load from savefile
!*/
/****************************************************************************/
void SaveHandler::assign(int &data, int default_data, unsigned int ID, bool save)
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
			std::stringstream ss;
			ss.str("");
			ss << data << "\0";
			Data[ID - 1] = ss.str();
		}
		else
		{
			data = static_cast<int>(std::stoi(Data[ID - 1]));
		}
	}
}

/****************************************************************************/
/*!
\brief
Assign a particular value to the save file, for loading and saving.
\param data
the value of the data to keep
\param default_data
the data to default to if it doesn't exist in save
\param ID
the ID of the data for storing later
\param save
if true, will overwrite savefile, if false it'll load from savefile
!*/
/****************************************************************************/
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
			ss.str("");
			ss << data << "\0";
			Data[ID - 1] = ss.str();
		}
		else
		{
			char CN = std::stoi(Data[ID - 1]);
			data = CN;
		}
	}
}

/****************************************************************************/
/*!
\brief
Loads the save file into a vector
!*/
/****************************************************************************/
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

/****************************************************************************/
/*!
\brief
Saves the vector into the save file
!*/
/****************************************************************************/
void SaveHandler::saveData(void)
{
	std::fstream SaveFile;
	SaveFile.open(FILE_LOCATION, std::ofstream::out | std::ofstream::trunc);

	SaveFile << Data.size();

	for (unsigned i = 0; i < Data.size(); ++i)
	{
		SaveFile << " ";
		SaveFile << Data[i];
	}

	SaveFile.close();
}