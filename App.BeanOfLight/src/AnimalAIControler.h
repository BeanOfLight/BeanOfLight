#pragma once

#include "Animal.h"
#include <vector>

class AnimalAIControler
{
public:
	AnimalAIControler();
	~AnimalAIControler();

	void attach(Animal* i_pAnimal, Animal* i_pHero, Ogre::TerrainGroup* i_pTerrainGroup);
	void detach();

	// Configure
	void setWayPoints(std::vector<Ogre::Vector3> i_wayPoints); // place waypoints on the terrain
	bool m_huntHero;

	// State
	bool m_isAlerted;

	// Action
	void move(double i_timeSinceLastFrame);

private:
	std::vector<Ogre::Vector3> m_wayPoints;
	Ogre::TerrainGroup* m_pTerrainGroup;
	Animal* m_pAnimal;
	Animal* m_pHero;

};

