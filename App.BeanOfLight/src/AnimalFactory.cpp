#include "AnimalFactory.h"

#include "ProceduralShape.h"


const float walkSpeed = /*140.f*/ 650.f;
const float runFactor = 4.f;

Animal* AnimalFactory::createHeroBean(
	Ogre::SceneManager* i_pSceneManager,
	const Ogre::Vector3& i_location,
	const Ogre::Quaternion& i_orientation,
	const Ogre::String& i_name)
{
	Animal* pAvatar = new Animal();

	pAvatar->m_heightOffset = 90.f;
	pAvatar->m_stuckToTerrain = true;
	pAvatar->m_maxWaklSpeed = walkSpeed;
	pAvatar->m_maxRunSpeed = walkSpeed * runFactor;
	pAvatar->m_maxTurnSpeed = 0.f;

	pAvatar->m_pNode = i_pSceneManager->getRootSceneNode()->createChildSceneNode();
	float avatarHeightOffset = 90.f;
	pAvatar->m_pNode->setPosition(i_location.x, i_location.y, i_location.z);
	pAvatar->m_pNode->pitch(Ogre::Degree(-90));
	pAvatar->m_pNode->roll(Ogre::Degree(90));
	
	ProceduralShape::createAvatar(
		i_pSceneManager,
		pAvatar->m_pNode,
		i_name,
		180.f, 70.f, 40.f, 60.f, 10.f, 10.f,
		Ogre::String("BeanOfLight/HeroBlue"));

	ProceduralShape::createCS(
		i_pSceneManager,
		pAvatar->m_pNode,
		i_name + Ogre::String("CS"),
		Ogre::Vector3::ZERO,
		150.f);

	return pAvatar;
}

Animal* AnimalFactory::createPeonBean(
	Ogre::SceneManager* i_pSceneManager,
	const Ogre::Vector3& i_location,
	const Ogre::Quaternion& i_orientation,
	const Ogre::String& i_name)
{
	return nullptr;
}
