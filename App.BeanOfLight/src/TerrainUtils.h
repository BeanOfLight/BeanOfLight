#pragma once

#include <Terrain/OgreTerrainGroup.h>

// Get terrain point at the vertical of the object
Ogre::Vector3 getTerrainPoint(
	const Ogre::Vector3& i_pos,
	float i_offset,
	bool i_stick,
	Ogre::TerrainGroup& i_terrain);

// Intersect ray with terrain
bool rayToTerrain(
	const Ogre::Vector3& i_orig,
	const Ogre::Vector3& i_dir,
	Ogre::TerrainGroup& i_terrain,
	Ogre::Vector3& o_collision);