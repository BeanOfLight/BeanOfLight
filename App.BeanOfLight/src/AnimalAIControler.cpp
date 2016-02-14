#include "AnimalAIControler.h"

#include "TerrainUtils.h"

float stopDistance = 100.f; // cm;
float visionRadius = 4000.f; // cm
float visionHalfAngle = Ogre::Math::PI / 4.f; // radians

AnimalAIControler::AnimalAIControler()
	: m_pAnimal(nullptr),
	m_pTerrainGroup(nullptr),
	m_pHero(nullptr)
{
}

AnimalAIControler::~AnimalAIControler()
{
}

void AnimalAIControler::attach(Animal* i_pAnimal, Animal* i_pHero, Ogre::TerrainGroup* i_pTerrainGroup)
{
	m_pAnimal = i_pAnimal;
	m_pHero = i_pHero;
	m_pTerrainGroup = i_pTerrainGroup;

	if (!(m_pAnimal != nullptr && m_pTerrainGroup != nullptr))
		return;

	m_pAnimal->dropOnTerrain(*i_pTerrainGroup);
}

void AnimalAIControler::detach()
{
	m_pAnimal = nullptr;
	m_pTerrainGroup = nullptr;
}

void AnimalAIControler::setWayPoints(std::vector<Ogre::Vector3> i_wayPoints)
{
	m_wayPoints.resize(i_wayPoints.size());
	for (size_t i = 0; i < i_wayPoints.size(); ++i)
	{
		m_wayPoints[i] = collideWithTerrain(i_wayPoints[i], m_pAnimal->m_heightOffset, true, *m_pTerrainGroup);
	}
}


void AnimalAIControler::move(double i_timeSinceLastFrame)
{
	// If we are close to the first way point, rotate the vector to the next one
	if (!m_wayPoints.empty() && (m_pAnimal->m_pNode->getPosition()- m_wayPoints[0]).length() < stopDistance)
	{
		std::rotate(m_wayPoints.begin(), m_wayPoints.begin() + 1, m_wayPoints.end());
	}

	// If the player is visible (near and in front), turn towards the player
	Ogre::Vector3 animalToHero = m_pHero->m_pNode->getPosition() - m_pAnimal->m_pNode->getPosition();
	Ogre::Real distanceToHero = animalToHero.length();
	Ogre::Radian angleToHero = animalToHero.angleBetween(m_pAnimal->m_pNode->getOrientation().xAxis());
	if(distanceToHero < visionRadius && angleToHero.valueRadians() < visionHalfAngle)
	{ 
		if (distanceToHero < stopDistance)
			return;
		else
		{
			m_pAnimal->turn(animalToHero.normalisedCopy(), i_timeSinceLastFrame);
			m_pAnimal->moveOnTerrain(1, 0, true, i_timeSinceLastFrame, *m_pTerrainGroup);
		}
	}
	else
	{
		Ogre::Vector3 animalToWaypoint = m_wayPoints[0] - m_pAnimal->m_pNode->getPosition();
		Ogre::Real distanceToWayPoint = animalToWaypoint.length();
		if (distanceToWayPoint < stopDistance)
			return;
		else
		{
			m_pAnimal->turn(animalToWaypoint.normalisedCopy(), i_timeSinceLastFrame);
			m_pAnimal->moveOnTerrain(1, 0, false, i_timeSinceLastFrame, *m_pTerrainGroup);
		}
	}
}
