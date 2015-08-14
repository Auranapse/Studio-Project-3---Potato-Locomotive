/****************************************************************************/
/*!
\file savefile.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Save file
!*/
/****************************************************************************/
#include "savefile"

SaveFile::SaveFile()
{
	FILE_LOCATION = "Default";
}

SaveFile::~SaveFile()
{
	for(unsigned i = 0; i < Data.size(); ++i)
	{
		if(Data[i] != NULL)
		{
			delete Data[i];
		}
	}
}

bool SaveFile::fileExists(std::string file)
{
	std::ifstream infile(file);
	return infile.good();
}

void SaveFile::init(std::string file)
{
	FILE_LOCATION = file;

	if(!fileExists(file))
	{
		CreateNew();
	}

	loadData();
}

void SaveFile::loadData(void)
{
	for(unsigned i = 0; i < Data.size(); ++i)
	{
		if(Data[i] != NULL)
		{
			delete Data[i];
		}
	}

	while(Data.size() > 0)
	{
		Data.pop_back();
	}

	std::fstream Savefile;
	Savefile.open(FILE_LOCATION);

	int fileSize;
	Savefile >> fileSize;

	std::string *tempdata;

	for(unsigned i = 0; i < fileSize; i++)
	{
		tempdata = new std::string;
		Savefile >> *tempdata;
		Data.push_back(tempdata);
	}

	Savefile.close();
}

void SaveFile::saveData(void)
{
	std::fstream SaveFile;
	SaveFile.open(FILE_LOCATION);
	
	SaveFile << Data.size();

	for(unsigned i = 0; i < Data.size(); ++i)
	{
		SaveFile << " ";
		SaveFile << *Data[i];
	}

	SaveFile.close();
}

void SaveFile::CreateNew(void)
{
	std::fstream filecreate;
	filecreate.open(FILE_LOCATION, std::fstream::out);
	filecreate.close();
	saveData();
}