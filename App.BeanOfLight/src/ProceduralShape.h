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

	//static Ogre::ManualObject* createBlock(float i_sizeX, float i_sizeY, float i_sizeZ);
	//static Ogre::ManualObject* createAvatar(float i_sizeX, float i_sizeY, float i_sizeZ);
};


