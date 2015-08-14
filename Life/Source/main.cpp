#include "Application.h"
/******************************************************************************/
/*!
\file	main.cpp
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Main to run the application
*/
/******************************************************************************/
int main(void)
{
	Application app;
	app.Init();
	app.Run();
	app.Exit();
}