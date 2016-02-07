#pragma once

#include <OgreSceneManager.h>


class ProceduralShape
{
public:
	static Ogre::ManualObject* createCube(
		Ogre::SceneManager* i_pSceneMngr,
		Ogre::SceneNode* i_pParent,
		Ogre::String& i_name,
		float i_size);

	static Ogre::ManualObject* createAvatar(
		Ogre::SceneManager* i_pSceneMngr,
		Ogre::SceneNode* i_pParent,
		Ogre::String& i_name,
		float i_height,
		float i_width,
		float i_depth,
		float i_sameHeight,
		float i_seamWidth,
		float i_resolution);
};


