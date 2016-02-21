#include "TerrainCreator.h"
#include <OgreMemoryAllocatorConfig.h>

TerrainCreator::TerrainCreator()
	: m_terrainSize(1025),
	m_terrainWorldSize(5*12800.0)
{

}

TerrainCreator::~TerrainCreator()
{

}

Ogre::TerrainGroup* TerrainCreator::initTerrain(Ogre::SceneManager& i_sceneManager)
{
	// Terrain
	Ogre::Vector3 lightdir(0.1f, -0.3f, -1.0f);
	lightdir.normalise();

	Ogre::Light* light = i_sceneManager.createLight("TestLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		&i_sceneManager,
		Ogre::Terrain::ALIGN_X_Z,
		m_terrainSize, m_terrainWorldSize);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	// mTerrainGroup->setAutoUpdateLod(Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod(Ogre::TerrainAutoUpdateLodStrategy::BY_DISTANCE));

	configureTerrainDefaults(i_sceneManager, light);

	//for (long x = 0; x <= 5; ++x)
	//	for (long y = 0; y <= 5; ++y)
	//		defineTerrain(x, y);

	//mTerrainGroup->loadAllTerrains(true);

	//if (mTerrainsImported)
	//{
	//	Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	//	while (ti.hasMoreElements())
	//	{
	//		Ogre::Terrain* t = ti.getNext()->instance;
	//		initBlendMaps(t);
	//	}
	//}

	return mTerrainGroup;
}

void getTerrainImage(Ogre::uint32 i_size, Ogre::Image& img)
{
	// Placeholder for terrain procedural generator
	// Generate a i_size x i_size

	float hfSize = (float)i_size/2.f;
	
	float* image = OGRE_ALLOC_T(float, i_size * i_size, Ogre::MEMCATEGORY_GENERAL);
	float height = Ogre::Math::RangeRandom(15.f, 5*35.f);
	for (Ogre::uint32 i = 0; i < i_size; i++)
	{
		for (Ogre::uint32  j = 0; j < i_size; j++)
		{
			float distFromCenter = (float)sqrt(abs(hfSize - i)*abs(hfSize - i) + abs(hfSize - j) * abs(hfSize - j));
			float f = std::max(hfSize - distFromCenter, 0.f) / hfSize;
			image[i * i_size + j] = f * f * (2 - f * f) * height;
		}
	}


	img.loadDynamicImage((Ogre::uchar*)image,
		i_size,
		i_size,
		1,
		Ogre::PF_FLOAT32_R,
		true);
}

void TerrainCreator::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	bool exists =
		Ogre::ResourceGroupManager::getSingleton().resourceExists(
			mTerrainGroup->getResourceGroup(),
			filename);

	if (exists)
		mTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(m_terrainSize, img);
		mTerrainGroup->defineTerrain(x, y, &img);

		mTerrainsImported = true;
	}
}

void TerrainCreator::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

#ifdef DEBUG
	Ogre::uint16 size = terrain->getLayerBlendMapSize();
#else
	Ogre::uint16 size = 1024;
	// Bug with Ogre 1.9: terrain->getLayerBlendMapSize() returns 0 in release mode.
#endif // DEBUG

	for (Ogre::uint16 y = 0; y < size; ++y)
	{
		for (Ogre::uint16 x = 0; x < size; ++x)
		{
			Ogre::Real tx, ty;

			// Compute max slope
			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			float maxDiff = FLT_MIN;
			for (int nx = -1; nx <= 1; ++nx)
			{
				for (int ny = -1; ny <= 1; ++ny)
				{
					if (nx == 0 && ny == 0)
						continue;

					blendMap0->convertImageToTerrainSpace(x + nx, y + ny, &tx, &ty);
					float tempDiff = abs(terrain->getHeightAtTerrainPosition(tx, ty) - height);
					if (nx != 0 && ny != 0)
						tempDiff /= 1.41421f;
					if (tempDiff > maxDiff) maxDiff = tempDiff;
				}
			}

			// Rock on steep parts
			Ogre::Real valSlope;
			if (maxDiff < 6.f) valSlope = 0.f;
			else if (maxDiff < 8.f)
				valSlope = (maxDiff - 6.f) / 2.f;
			else
				valSlope = 1.f;
			*pBlend0++ = valSlope;

			// Snow on high but not steep parts
			Ogre::Real valSnow;
			if (height < 900)
				valSnow = 0.f;
			else if (height < 1000)
				valSnow = (1 - valSlope)*(height - 900) / 100;
			else
				valSnow = (1-valSlope);
			*pBlend1++ = valSnow;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void TerrainCreator::configureTerrainDefaults(Ogre::SceneManager& i_sceneManager, Ogre::Light* light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(m_terrainWorldSize);
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(i_sceneManager.getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = m_terrainSize;
	importData.worldSize = m_terrainWorldSize;
	importData.inputScale = 100.0;
	importData.minBatchSize = 9;
	importData.maxBatchSize = 65;

	importData.layerList.resize(3);
	importData.layerList[0].worldSize = 400;
	importData.layerList[0].textureNames.push_back(
		"grass_green-01_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back(
		"grass_green-01_normalheight.dds");
	importData.layerList[1].worldSize = 400;
	importData.layerList[1].textureNames.push_back(
		"dirt_grayrocky_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back(
		"dirt_grayrocky_normalheight.dds");
	importData.layerList[2].worldSize = 800;
	importData.layerList[2].textureNames.push_back(
		"snow_1024.jpg");
	importData.layerList[2].textureNames.push_back(
		"dirt_grayrocky_normalheight.dds");
}

//-----------------------------------------------------------------------------

void SimpleTerrainDefiner::define(Ogre::TerrainGroup* i_pTerrainGroup, long x, long y)
{
	Ogre::Image img;
	getTerrainImage(129, img);
	i_pTerrainGroup->defineTerrain(x, y, &img);
}