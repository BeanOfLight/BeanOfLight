#include "AnimalFactory.h"

#include "ProceduralShape.h"


const float walkSpeed = /*140.f*/ 650.f;
const float runFactor = 4.f;
const float turnSpeed = Ogre::Math::PI;

Animal* AnimalFactory::createHeroBean(
	Ogre::SceneManager* i_pSceneManager,
	const Ogre::Vector3& i_location,
	const Ogre::Quaternion& i_orientation,
	const Ogre::String& i_name)
{
	Animal* pHero = new Animal();

	pHero->m_heightOffset = 90.f;
	pHero->m_stuckToTerrain = true;
	pHero->m_maxWaklSpeed = walkSpeed;
	pHero->m_maxRunSpeed = walkSpeed * runFactor;
	pHero->m_maxTurnSpeed = turnSpeed;

	pHero->m_pNode = i_pSceneManager->getRootSceneNode()->createChildSceneNode();
	pHero->m_pNode->setPosition(i_location.x, i_location.y, i_location.z);
	pHero->m_pNode->pitch(Ogre::Degree(-90));
	pHero->m_pNode->roll(Ogre::Degree(90));
	
	ProceduralShape::createAvatar(
		i_pSceneManager,
		pHero->m_pNode,
		i_name,
		180.f, 70.f, 40.f, 60.f, 10.f, 10.f,
		Ogre::String("BeanOfLight/HeroBlue"));

	ProceduralShape::createCS(
		i_pSceneManager,
		pHero->m_pNode,
		i_name + Ogre::String("CS"),
		Ogre::Vector3::ZERO,
		150.f);

	return pHero;
}

Animal* AnimalFactory::createGuardBean(
	Ogre::SceneManager* i_pSceneManager,
	const Ogre::Vector3& i_location,
	const Ogre::Quaternion& i_orientation,
	const Ogre::String& i_name)
{
	Animal* pGuard = new Animal();

	pGuard->m_heightOffset = 110.f;
	pGuard->m_stuckToTerrain = true;
	pGuard->m_maxWaklSpeed = walkSpeed * 0.8f;
	pGuard->m_maxRunSpeed = walkSpeed * runFactor * 0.5f;
	pGuard->m_maxTurnSpeed = turnSpeed * 0.8f;

	pGuard->m_pNode = i_pSceneManager->getRootSceneNode()->createChildSceneNode();
	pGuard->m_pNode->setPosition(i_location.x, i_location.y, i_location.z);
	pGuard->m_pNode->pitch(Ogre::Degree(-90));
	pGuard->m_pNode->roll(Ogre::Degree(90));

	ProceduralShape::createAvatar(
		i_pSceneManager,
		pGuard->m_pNode,
		i_name,
		220.f, 100.f, 60.f, 80.f, 10.f, 10.f,
		Ogre::String("BeanOfLight/GuardRed"));

	ProceduralShape::createCS(
		i_pSceneManager,
		pGuard->m_pNode,
		i_name + Ogre::String("CS"),
		Ogre::Vector3::ZERO,
		150.f);

	return pGuard;
}

Animal* AnimalFactory::createPeonBean(
	Ogre::SceneManager* i_pSceneManager,
	const Ogre::Vector3& i_location,
	const Ogre::Quaternion& i_orientation,
	const Ogre::String& i_name)
{
	Animal* pPeon = new Animal();

	pPeon->m_heightOffset = 85.f;
	pPeon->m_stuckToTerrain = true;
	pPeon->m_maxWaklSpeed = walkSpeed * 0.8f;
	pPeon->m_maxRunSpeed = walkSpeed * runFactor * 0.5f;
	pPeon->m_maxTurnSpeed = turnSpeed * 0.8f;

	pPeon->m_pNode = i_pSceneManager->getRootSceneNode()->createChildSceneNode();
	pPeon->m_pNode->setPosition(i_location.x, i_location.y, i_location.z);
	pPeon->m_pNode->pitch(Ogre::Degree(-90));
	pPeon->m_pNode->roll(Ogre::Degree(90));

	ProceduralShape::createAvatar(
		i_pSceneManager,
		pPeon->m_pNode,
		i_name,
		170.f, 80.f, 50.f, 50.f, 10.f, 10.f,
		Ogre::String("BeanOfLight/PeonYellow"));

	ProceduralShape::createCS(
		i_pSceneManager,
		pPeon->m_pNode,
		i_name + Ogre::String("CS"),
		Ogre::Vector3::ZERO,
		150.f);

	return pPeon;
}
