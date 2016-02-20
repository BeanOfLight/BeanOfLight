#include "TerrainUtils.h"

Ogre::Vector3 getTerrainPoint(
	const Ogre::Vector3& i_pos,
	float i_offset,
	bool i_stick,
	Ogre::TerrainGroup& i_terrain)
{
	Ogre::Vector3 collide = i_pos;
	bool hit = rayToTerrain(
		Ogre::Vector3(i_pos.x, 5000.0f, i_pos.z),
		Ogre::Vector3::NEGATIVE_UNIT_Y,
		i_terrain,
		collide);
	if (!hit)
		return i_pos;
	
	collide.y += i_offset;
	if (!i_stick && (collide.y < i_pos.y))
		return i_pos; // object is above terrain

	return collide;
}

bool rayToTerrain(
	const Ogre::Vector3& i_orig,
	const Ogre::Vector3& i_dir,
	Ogre::TerrainGroup& i_terrain,
	Ogre::Vector3& o_collision)
{
	Ogre::Ray ray(i_orig, i_dir.normalisedCopy());
	Ogre::TerrainGroup::RayResult newPos = i_terrain.rayIntersects(ray);

	if (!newPos.hit)
		return false;
	
	o_collision = newPos.position;
	return true;
}