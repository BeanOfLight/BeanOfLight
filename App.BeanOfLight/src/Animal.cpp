#include "Animal.h"

#include "TerrainUtils.h"

Animal::Animal()
{
}


Animal::~Animal()
{
}

void Animal::dropOnTerrain(Ogre::TerrainGroup& i_terrain)
{
	Ogre::Vector3 oldPos = m_pNode->getPosition();
	Ogre::Vector3 newPos = collideWithTerrain(oldPos, m_heightOffset, m_stuckToTerrain, i_terrain);
	m_pNode->setPosition(newPos);
}

void Animal::moveOnTerrain(int i_moveForward, int i_straffeLeft, bool i_run, double i_timeSinceLastCall, Ogre::TerrainGroup& i_terrain)
{
	if (i_timeSinceLastCall < std::numeric_limits<double>::epsilon())
		return;

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
	newPos = collideWithTerrain(newPos, m_heightOffset, m_stuckToTerrain, i_terrain);

	// Adjust speed for slope
	newPos = oldPos + (newPos - oldPos).normalisedCopy() * speedFactor;
	m_pNode->setPosition(newPos);
}

void Animal::turn(Ogre::Vector3 i_newXDir, double i_timeSinceLastCall)
{
	if (i_timeSinceLastCall < std::numeric_limits<double>::epsilon())
		return;

	Ogre::Quaternion curOrient = m_pNode->getOrientation();
	Ogre::Vector3 x = curOrient.xAxis();
	Ogre::Vector3 z = curOrient.zAxis();

	// Ideal new X if we could turn at infinite speed
	Ogre::Vector3 newX = Ogre::Vector3(i_newXDir.x, 0.f, i_newXDir.z).normalisedCopy();
	Ogre::Quaternion finalOrient = Ogre::Quaternion(newX, z.crossProduct(newX), z);

	Ogre::Radian angle = x.angleBetween(newX);

	// Total time it will take to turn at max speed
	float turnTimeSecs = float(angle.valueRadians()) / m_maxTurnSpeed;
	float timeSecs = (float)i_timeSinceLastCall / 1000.f;

	if (turnTimeSecs < timeSecs)
		m_pNode->setOrientation(finalOrient);
	else
		m_pNode->setOrientation(Ogre::Quaternion::Slerp(Ogre::Real(timeSecs / turnTimeSecs), curOrient, finalOrient, true));
}
