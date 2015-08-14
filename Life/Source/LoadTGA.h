/******************************************************************************/
/*!
\file	LoadTGA.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Loads tga textures
*/
/******************************************************************************/

#ifndef LOAD_TGA_H
#define LOAD_TGA_H
#include <GL\glew.h>

GLuint LoadTGA(const char *file_path, bool Clamp = false, bool EnableMipmap = true);

#endif