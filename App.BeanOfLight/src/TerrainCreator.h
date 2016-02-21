#pragma once

#include <OgreSceneManager.h>

#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPagedWorldSection.h>

class TerrainCreator
{
public:
	TerrainCreator();
	~TerrainCreator();

	Ogre::TerrainGroup* initTerrain(Ogre::SceneManager& i_sceneManager);

private:
	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::SceneManager& i_sceneManager, Ogre::Light* light);

	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;

	Ogre::uint32 m_terrainSize; // must be 2^n+1
	Ogre::Real m_terrainWorldSize; 
};

class SimpleTerrainDefiner : public Ogre::TerrainPagedWorldSection::TerrainDefiner
{
public:
	virtual void define(Ogre::TerrainGroup* i_pTerrainGroup, long x, long y);
};

