/****************************************************************************/
/*!
\file savefile.h
\author Gabriel Wong Choon Jieh
\par email: AuraTigital\@gmail.com
\brief
Save file
!*/
/****************************************************************************/
#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class SaveFile
{
private:
	std::string FILE_LOCATION;
	
public:
	SaveFile();
	~SaveFile();

	std::vector<std::string*> Data;
	
	void init(std::string file);
	void loadData(void);
	void saveData(void);
	void CreateNew(void);
	bool fileExists(std::string file);
};
#endif