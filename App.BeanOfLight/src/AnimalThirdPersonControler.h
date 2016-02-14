#pragma once

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <Terrain/OgreTerrainGroup.h>

#include "Animal.h"

// Class enables player to control an animal in 3rd person view
class AnimalThirdPersonControler
{
public:
	AnimalThirdPersonControler();
	~AnimalThirdPersonControler();

	void attach(Animal* i_pAnimal, Ogre::Camera* i_pCamera, float i_camHeightOffset, Ogre::TerrainGroup* i_pTerrainGroup);
	void detach();

	// Change state
	void alignAnimalToCamera(); // Adjust avatar orientation to face forward in camera view
	void orient(int i_xRel, int i_yRel);
	void move(int i_frontDir, int i_sideDir, bool i_run, double i_timeSinceLastFrame);

public:
	bool m_lookAround; // avatar does not rotate on orient

private:

	Ogre::TerrainGroup* m_pTerrainGroup;
	Ogre::Camera* m_pCamera;
	float m_camAngularSpeed;
	float m_camDistance;
	float m_camHeightOffset;
	
	Animal* m_pAnimal;
};

