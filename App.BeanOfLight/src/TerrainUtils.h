#pragma once

#include <Terrain/OgreTerrainGroup.h>

// Collide a position with the terrain
// if i_stick is false, only update position if there is a collision 
Ogre::Vector3 collideWithTerrain(const Ogre::Vector3& i_pos, float i_offset, bool i_stick, Ogre::TerrainGroup& i_terrain);

