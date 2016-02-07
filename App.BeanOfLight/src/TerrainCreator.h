#pragma once

#include <OgreSceneManager.h>

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

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
};

