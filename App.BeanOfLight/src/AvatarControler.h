#pragma once

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <Terrain/OgreTerrainGroup.h>

// Avatar is a object in the scene controlled by the player
// 
class AvatarControler
{
public:
	AvatarControler();
	~AvatarControler();

	void attachAvatar(Ogre::SceneNode* i_pAvatar, float i_heightOffset);
	void detachAvatar();

	void attachCamera(Ogre::Camera* i_pCamera); // Avatar must be attached first
	void detachCamera();

	void attachTerrain(Ogre::TerrainGroup* i_pTerrainGroup);
	void detachTerrain();

	// Change state
	void alignAvatarToCamera(); // Adjust avatar orientation to face forward in camera view
	void orient(int i_xRel, int i_yRel);
	void move(int i_frontDir, int i_sideDir, double i_timeSinceLastFrame, bool i_run);

public:
	// States
	bool m_lookAround; // avatar does not rotate on orient

	// Properties
	float m_waklSpeed; // walk speed in cm/s
	float m_runSpeed;  // run speed in cm/s	
	float m_camDistance; // camera distance in cm
	float m_angularSpeed; 

private:

	Ogre::Camera* m_pCamera;
	Ogre::SceneNode* m_pAvatar;
	float m_avatarHeightOffset;
	Ogre::TerrainGroup* m_pTerrainGroup;

};

