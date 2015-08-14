/******************************************************************************/
/*!
\file	Scene.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Parent class of all scenes, initialise, update and render
*/
/******************************************************************************/
#ifndef SCENE_H
#define SCENE_H

#include "Application.h"

class Scene
{
public:
	/******************************************************************************/
	/*!
	struct ViewPort:
	\brief	A struct to store information about the view port
	*/
	/******************************************************************************/
	struct ViewPort
	{
		/******************************************************************************/
		/*!
		float f_posX:
		\brief	Controls the leftmost position of the view port
		*/
		/******************************************************************************/
		float f_posX;
		/******************************************************************************/
		/*!
		float f_posY:
		\brief	Controls the topmost position of the view port
		*/
		/******************************************************************************/
		float f_posY;
		/******************************************************************************/
		/*!
		float f_sizeX:
		\brief	Controls the horizontal scale of the view port
		*/
		/******************************************************************************/
		float f_sizeX;
		/******************************************************************************/
		/*!
		float f_sizeY:
		\brief	Controls the vertical scale of the view port
		*/
		/******************************************************************************/
		float f_sizeY;
	};
	/******************************************************************************/
	/*!
	\brief
	Default constructor
	*/
	/******************************************************************************/
	Scene() 
	{
		e_nextScene = Application::E_SCENE_TOTAL;
		b_enableViewPort2 = false;
	}
	/******************************************************************************/
	/*!
	\brief
	Destructor
	*/
	/******************************************************************************/
	~Scene() 
	{

	}
	/******************************************************************************/
	/*!
	\brief
	Function to initialize the scene
	*/
	/******************************************************************************/
	virtual void Init() = 0;

	/******************************************************************************/
	/*!
	\brief
	Function to initialize the shaders
	*/
	/******************************************************************************/
	virtual void InitShaders() = 0;

	/******************************************************************************/
	/*!
	\brief
	Function to update the scene every frame

	\param dt - frame time
	*/
	/******************************************************************************/
	virtual void Update(double dt) = 0;
	/******************************************************************************/
	/*!
	\brief
	Function to render the scene every frame
	*/
	/******************************************************************************/
	virtual void Render() = 0;
	/******************************************************************************/
	/*!
	\brief
	Function to do clean up
	*/
	/******************************************************************************/
	virtual void Exit() = 0;

	/******************************************************************************/
	/*!
	E_SCENE_TYPE e_nextScene:
	\brief	Controls the next scene to go to.
	If is equal to E_SCENE_TOTAL, it will not go to the next scene
	*/
	/******************************************************************************/
	Application::E_SCENE_TYPE e_nextScene;
	/******************************************************************************/
	/*!
	\brief
	Getter for e_nextScene

	\return The next scene in Application that the Application should go to
	A value of Application::E_SCENE_TOTAL means that the scene should not change
	*/
	/******************************************************************************/
	Application::E_SCENE_TYPE GetNextScene()
	{
		return e_nextScene;
	}
	/******************************************************************************/
	/*!
	\brief
	Checks if the secondary view port is enabled

	\return if the secondary view port is enabled
	*/
	/******************************************************************************/
	bool ViewPort2Enabled()
	{
		return b_enableViewPort2;
	}
	/******************************************************************************/
	/*!
	\brief
	Initializes the secondary view port
	*/
	/******************************************************************************/
	virtual void Init2()
	{
		viewport2.f_posX = static_cast<float>(Application::GetWindowWidth())/2;
		viewport2.f_posY = 0.0f;
		viewport2.f_sizeX = static_cast<float>(Application::GetWindowWidth())/2;
		viewport2.f_sizeY = static_cast<float>(Application::GetWindowHeight());
	}
	/******************************************************************************/
	/*!
	\brief
	Updates the secondary view port
	*/
	/******************************************************************************/
	virtual void Update2()
	{

	}
	/******************************************************************************/
	/*!
	\brief
	Renders the secondary view port
	*/
	/******************************************************************************/
	virtual void Render2()
	{

	}
	/******************************************************************************/
	/*!
	\brief
	Returns the specifications of the secondary view port

	\return the specifications of the secondary view port
	*/
	/******************************************************************************/
	ViewPort GetViewPort2()
	{
		return viewport2;
	}

protected:
	/******************************************************************************/
	/*!
	ViewPort viewport2:
	\brief	Stores information about the secondary view port
	*/
	/******************************************************************************/
	ViewPort viewport2;
	/******************************************************************************/
	/*!
	bool b_enableViewPort2:
	\brief	Controls if the secondary view port should be rendered
	*/
	/******************************************************************************/
	bool b_enableViewPort2;
};

#endif