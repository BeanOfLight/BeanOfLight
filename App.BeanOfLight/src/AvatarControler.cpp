#include "AvatarControler.h"

#include <OgreMath.h>

const float walkSpeed = /*140.f*/ 650.f;
const float runFactor = 4.f;
const float camDist = 900.f;

AvatarControler::AvatarControler()
	: m_lookAround(false),
	m_waklSpeed(walkSpeed),
	m_runSpeed(runFactor * walkSpeed),
	m_camDistance(camDist),
	m_angularSpeed(0.0015f),
	m_pCamera(nullptr),
	m_pAvatar(nullptr),
	m_avatarHeightOffset(0.f),
	m_pTerrainGroup(nullptr)
{

}

AvatarControler::~AvatarControler()
{

}

void AvatarControler::attachAvatar(Ogre::SceneNode* i_pAvatar, float i_heightOffset)
{
	m_pAvatar = i_pAvatar;
	m_avatarHeightOffset = i_heightOffset;
}

void AvatarControler::detachAvatar()
{
	m_pAvatar = nullptr;
}

void AvatarControler::attachCamera(Ogre::Camera* i_pCamera)
{
	m_pCamera = i_pCamera;

	if (!(m_pAvatar != nullptr && m_pCamera != nullptr))
		return;

	// Position camera
	m_pCamera->setPosition(m_pAvatar->getPosition() - m_camDistance * m_pAvatar->getOrientation().xAxis());
	m_pCamera->lookAt( m_pAvatar->getPosition() );
}

void AvatarControler::detachCamera()
{
	m_pCamera = nullptr;
}

void AvatarControler::attachTerrain(Ogre::TerrainGroup* i_pTerrainGroup)
{
	m_pTerrainGroup = i_pTerrainGroup;
}

void  AvatarControler::detachTerrain()
{
	AvatarControler::m_pTerrainGroup = nullptr;
}

void AvatarControler::alignAvatarToCamera()
{
	if (!(m_pAvatar != nullptr && m_pCamera != nullptr))
		return;

	// Find new orientation of avatar
	Ogre::Vector3 camY = m_pCamera->getDirection();
	Ogre::Vector3 avaX = m_pAvatar->getOrientation().xAxis();
	Ogre::Vector3 avaY = m_pAvatar->getOrientation().yAxis();
	Ogre::Vector3 avaZ = m_pAvatar->getOrientation().zAxis();

	Ogre::Vector3 newAvaX = camY.dotProduct(avaX) * avaX + camY.dotProduct(avaY) * avaY;
	if (newAvaX.isZeroLength())
		return;
	newAvaX.normalise();
	m_pAvatar->setOrientation(Ogre::Quaternion(newAvaX, avaZ.crossProduct(newAvaX), avaZ));
}

void AvatarControler::move(int i_frontDir, int i_sideDir, double i_timeSinceLastFrame, bool i_run)
{
	if (!(m_pAvatar != nullptr && m_pCamera != nullptr))
		return;

	if (i_frontDir == 0 && i_sideDir == 0)
		return;

	Ogre::Vector3 avaX = Ogre::Vector3::ZERO;
	Ogre::Vector3 avaY = Ogre::Vector3::ZERO;

	if (i_frontDir)
	{
		avaX = m_pAvatar->getOrientation().xAxis();
		if (i_frontDir < 0) avaX *= -1.f;
	}

	if (i_sideDir)
	{
		avaY = m_pAvatar->getOrientation().yAxis();
		if (i_sideDir < 0) avaY *= -1.f;
	}

	Ogre::Vector3 moveDir = (avaX + avaY).normalisedCopy();
	float speed = i_run ? m_runSpeed : m_waklSpeed;
	Ogre::Vector3 move = moveDir * speed * (float) i_timeSinceLastFrame / 1000.f;

	if (m_pTerrainGroup)
	{
		Ogre::Vector3 pos = m_pAvatar->getPosition() + move;
		Ogre::Ray ray(Ogre::Vector3(pos.x, 5000.0f, pos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
		Ogre::TerrainGroup::RayResult newPos = m_pTerrainGroup->rayIntersects(ray);
		if (newPos.hit)
		{
			move += newPos.position - pos + m_avatarHeightOffset * Ogre::Vector3::UNIT_Y;
		}
	}

	m_pAvatar->setPosition(m_pAvatar->getPosition() + move);
	m_pCamera->setPosition(m_pCamera->getPosition() + move);
}

void AvatarControler::orient(int i_xRel, int i_yRel)
{
	if (!(m_pAvatar != nullptr && m_pCamera != nullptr))
		return;

	Ogre::Vector3 camPos = m_pCamera->getPosition();
	
	Ogre::Radian angleX(i_xRel * -m_angularSpeed);
	Ogre::Radian angleY(i_yRel * -m_angularSpeed);

	Ogre::Vector3 avatarToCamera = m_pCamera->getPosition() - m_pAvatar->getPosition();
	
	// Do not go to the poles
	Ogre::Radian latitude = m_pAvatar->getOrientation().zAxis().angleBetween(avatarToCamera);
	if (latitude < Ogre::Radian(Ogre::Math::DegreesToRadians(10.f)) && angleY < Ogre::Radian(0.f))
		angleY = Ogre::Radian(0.f);
	else if (latitude > Ogre::Radian(Ogre::Math::DegreesToRadians(170.f)) && angleY > Ogre::Radian(0.f))
		angleY = Ogre::Radian(0.f);

	Ogre::Quaternion orient = Ogre::Quaternion(angleY, m_pCamera->getOrientation().xAxis()) * Ogre::Quaternion(angleX, m_pCamera->getOrientation().yAxis());
	
	m_pCamera->setPosition(m_pAvatar->getPosition() + orient * avatarToCamera);
	m_pCamera->lookAt(m_pAvatar->getPosition());

	if (!m_lookAround)
		alignAvatarToCamera();
}
