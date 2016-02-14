#pragma once

#include <OgreSceneNode.h>
#include <Terrain/OgreTerrainGroup.h>


// Class represent a controlable animal (for example the avatar)
class Animal
{
public:
	Animal();
	virtual ~Animal();

	// Move the animal
	// set i_forward to 1 for forward
	// set i_forward to -1 for backwards
	// set i_straffe to 1 for left
	// set i_straffe to -1 for right
	void moveOnTerrain(int i_forward, int i_straffeLeft, bool i_run, double i_timeSinceLastCall, Ogre::TerrainGroup& i_terrain);
	void turn(Ogre::Vector3 i_newXDir, double i_timeSinceLastCall);

	// Shape and body parts
	// X is front
	// Y is left
	// Z is up
	Ogre::SceneNode* m_pNode; // The parent scene node of the animal
	float m_heightOffset; // Resting height above terrain

	// Physical Characteristics
	bool m_stuckToTerrain; // cannot stay in the air if false
	float m_maxWaklSpeed; // walk speed in cm/s
	float m_maxRunSpeed;  // run speed in cm/s	
	float m_maxTurnSpeed; // turn speed in radians/s
};

