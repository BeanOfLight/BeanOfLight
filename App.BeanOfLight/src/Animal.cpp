#include "Animal.h"

#include "TerrainUtils.h"

Animal::Animal()
{
}


Animal::~Animal()
{
}

void Animal::moveOnTerrain(int i_moveForward, int i_straffeLeft, bool i_run, double i_timeSinceLastCall, Ogre::TerrainGroup& i_terrain)
{
	if (m_pNode == nullptr)
		return;

	if (i_moveForward == 0 && i_straffeLeft == 0)
		return;

	Ogre::Vector3 movX = Ogre::Vector3::ZERO;
	Ogre::Vector3 movY = Ogre::Vector3::ZERO;

	if (i_moveForward)
	{
		movX = m_pNode->getOrientation().xAxis();
		if (i_moveForward < 0) movX *= -1.f;
	}

	if (i_straffeLeft)
	{
		movY = m_pNode->getOrientation().yAxis();
		if (i_straffeLeft < 0) movY *= -1.f;
	}

	Ogre::Vector3 moveDir = (movX + movY).normalisedCopy();
	float speedFactor = (i_run ? m_maxRunSpeed : m_maxWaklSpeed) * (float)i_timeSinceLastCall / 1000.f;

	Ogre::Vector3 oldPos = m_pNode->getPosition();
	Ogre::Vector3 newPos = oldPos + moveDir * speedFactor;
	newPos = collideWithTerrain(newPos, m_heightOffset, true, i_terrain);

	// Adjust speed for slope
	newPos = oldPos + (newPos - oldPos).normalisedCopy() * speedFactor;
	m_pNode->setPosition(newPos);
}

void Animal::turn(Ogre::Vector3 i_newXDir)
{
	Ogre::Vector3 z = m_pNode->getOrientation().zAxis();
	Ogre::Vector3 newX = Ogre::Vector3(i_newXDir.x, 0.f, i_newXDir.z).normalisedCopy();
	m_pNode->setOrientation(Ogre::Quaternion(newX, z.crossProduct(newX), z));
}
