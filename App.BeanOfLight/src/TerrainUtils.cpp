#include "TerrainUtils.h"

Ogre::Vector3 collideWithTerrain(const Ogre::Vector3& i_pos, float i_offset, bool i_stick, Ogre::TerrainGroup& i_terrain)
{
	Ogre::Ray ray(Ogre::Vector3(i_pos.x, 5000.0f, i_pos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::TerrainGroup::RayResult newPos = i_terrain.rayIntersects(ray);

	if (!newPos.hit)
		return i_pos;; // not terrain collision

	newPos.position.y += i_offset; // account for offset

	if (!i_stick && (newPos.position.y < i_pos.y))
		return i_pos; // object is above terrain

	return newPos.position;
}