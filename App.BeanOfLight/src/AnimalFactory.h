#pragma once

#include "Animal.h"

// Factory class to create animals
class AnimalFactory
{
public:

	// Creates the hero bean
	static Animal* createHeroBean(
		Ogre::SceneManager* i_pSceneManager,
		const Ogre::Vector3& i_location, 
		const Ogre::Quaternion& i_orientation,
		const Ogre::String& i_name);

	// Creates a guard bean
	static Animal* createGuardBean(
		Ogre::SceneManager* i_pSceneManager,
		const Ogre::Vector3& i_location,
		const Ogre::Quaternion& i_orientation,
		const Ogre::String& i_name);

	// Creates a peon bean
	static Animal* createPeonBean(
		Ogre::SceneManager* i_pSceneManager,
		const Ogre::Vector3& i_location,
		const Ogre::Quaternion& i_orientation,
		const Ogre::String& i_name);

};

