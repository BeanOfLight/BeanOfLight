#include "AnimalThirdPersonControler.h"

#include <OgreMath.h>

#include "TerrainUtils.h"

const float camDist = 900.f;

AnimalThirdPersonControler::AnimalThirdPersonControler()
	: m_lookAround(false),
	m_camDistance(camDist),
	m_camAngularSpeed(0.0015f),
	m_camHeightOffset(0.f),
	m_pCamera(nullptr),
	m_pAnimal(nullptr),
	m_pTerrainGroup(nullptr)
{

}

AnimalThirdPersonControler::~AnimalThirdPersonControler()
{

}

void AnimalThirdPersonControler::attach(Animal* i_pAnimal, Ogre::Camera* i_pCamera, Ogre::TerrainGroup* i_pTerrainGroup)
{
	m_pAnimal = i_pAnimal;
	m_pCamera = i_pCamera;
	m_pTerrainGroup = i_pTerrainGroup;
	m_camHeightOffset = i_pAnimal->m_heightOffset;

	if (!(m_pAnimal != nullptr && m_pCamera != nullptr))
		return;

	// Position camera
	m_pCamera->setPosition(m_pAnimal->m_pNode->getPosition() - m_camDistance * m_pAnimal->m_pNode->getOrientation().xAxis());
	m_pCamera->lookAt(m_pAnimal->m_pNode->getPosition() );
}

void AnimalThirdPersonControler::detach()
{
	m_pAnimal = nullptr;
	m_pCamera = nullptr;
}

void AnimalThirdPersonControler::alignAnimalToCamera(double i_timeSinceLastFrame)
{
	if (!(m_pAnimal != nullptr && m_pCamera != nullptr))
		return;

	// Find new orientation of avatar
	Ogre::Vector3 camY = m_pCamera->getDirection();
	Ogre::Vector3 aniX = m_pAnimal->m_pNode->getOrientation().xAxis();
	Ogre::Vector3 aniY = m_pAnimal->m_pNode->getOrientation().yAxis();
	Ogre::Vector3 newAniX = camY.dotProduct(aniX) * aniX + camY.dotProduct(aniY) * aniY;
	if (newAniX.isZeroLength())
		return;
	newAniX.normalise();
	m_pAnimal->turn(newAniX, i_timeSinceLastFrame);
}

void AnimalThirdPersonControler::move(int i_frontDir, int i_sideDir, bool i_run, double i_timeSinceLastFrame)
{
	if (!(m_pAnimal != nullptr && m_pCamera != nullptr))
		return;

	if (i_frontDir == 0 && i_sideDir == 0)
		return;

	Ogre::Vector3 oldAniPos = m_pAnimal->m_pNode->getPosition();
	m_pAnimal->moveOnTerrain(i_frontDir, i_sideDir, i_run, i_timeSinceLastFrame, *m_pTerrainGroup);
	Ogre::Vector3 newAniPos = m_pAnimal->m_pNode->getPosition();

	Ogre::Vector3 newCamPos = m_pCamera->getPosition() + newAniPos - oldAniPos;
	newCamPos = collideWithTerrain(newCamPos, m_camHeightOffset, false, *m_pTerrainGroup);
	m_pCamera->setPosition(newCamPos);
	m_pCamera->lookAt(newAniPos);
}

void AnimalThirdPersonControler::orient(int i_xRel, int i_yRel, double i_timeSinceLastFrame)
{
	if (!(m_pAnimal != nullptr && m_pCamera != nullptr))
		return;

	Ogre::Vector3 camPos = m_pCamera->getPosition();
	
	Ogre::Radian angleX(i_xRel * -m_camAngularSpeed);
	Ogre::Radian angleY(i_yRel * -m_camAngularSpeed);

	Ogre::Vector3 avatarToCamera = m_pCamera->getPosition() - m_pAnimal->m_pNode->getPosition();

	// restore lenght
	if (avatarToCamera.length() != m_camDistance)
		avatarToCamera = avatarToCamera.normalisedCopy() * m_camDistance;
	
	// Do not go to the poles
	Ogre::Radian latitude = m_pAnimal->m_pNode->getOrientation().zAxis().angleBetween(avatarToCamera);
	if (latitude < Ogre::Radian(Ogre::Math::DegreesToRadians(10.f)) && angleY < Ogre::Radian(0.f))
		angleY = Ogre::Radian(0.f);
	else if (latitude > Ogre::Radian(Ogre::Math::DegreesToRadians(170.f)) && angleY > Ogre::Radian(0.f))
		angleY = Ogre::Radian(0.f);

	Ogre::Quaternion orient = Ogre::Quaternion(angleY, m_pCamera->getOrientation().xAxis()) * Ogre::Quaternion(angleX, m_pCamera->getOrientation().yAxis());
	
	Ogre::Vector3 newCamPos = m_pAnimal->m_pNode->getPosition() + orient * avatarToCamera;
	newCamPos = collideWithTerrain(newCamPos, m_camHeightOffset, false, *m_pTerrainGroup);
	m_pCamera->setPosition(newCamPos);
	m_pCamera->lookAt(m_pAnimal->m_pNode->getPosition());

	if (!m_lookAround)
		alignAnimalToCamera(i_timeSinceLastFrame);
}
