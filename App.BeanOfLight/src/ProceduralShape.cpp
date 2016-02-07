#include "ProceduralShape.h"

#include <OgreVector3.h>
#include <OgreManualObject.h>

// Create procedural shape as a strip of triangles
class ProceduralShapeBase
{
public:
	ProceduralShapeBase() {}
	virtual ~ProceduralShapeBase() {}

	// How to interpret the data
	virtual Ogre::RenderOperation::OperationType getRenderOperation() = 0;

	// The data
	virtual void create(std::vector<Ogre::Vector3>& strip) = 0;
};

// Create a procedural cube centered on 0
class ProceduralCube : public ProceduralShapeBase
{
public:
	ProceduralCube() : m_hs(1.f) {}
	virtual ~ProceduralCube() {}

	void setSize(float s) { m_hs = s / 2.f; }

	virtual Ogre::RenderOperation::OperationType getRenderOperation() override;
	virtual void create(std::vector<Ogre::Vector3>& strip) override;

protected:
	float m_hs; // half-size of the cube
};

Ogre::RenderOperation::OperationType ProceduralCube::getRenderOperation()
{
	return Ogre::RenderOperation::OT_TRIANGLE_STRIP;
}

//     7-----6
//    /|    /|
//   4-----5 |
//   | 3---|-2
//   |/    |/
//   0-----1


void ProceduralCube::create(std::vector<Ogre::Vector3>& strip)
{

	std::vector<Ogre::Vector3> points;
	points.push_back(Ogre::Vector3(m_hs, -m_hs, -m_hs));
	points.push_back(Ogre::Vector3(m_hs, m_hs, -m_hs));
	points.push_back(Ogre::Vector3(-m_hs, m_hs, -m_hs));
	points.push_back(Ogre::Vector3(-m_hs, -m_hs, -m_hs));
	points.push_back(Ogre::Vector3(m_hs, -m_hs, m_hs));
	points.push_back(Ogre::Vector3(m_hs, m_hs, m_hs));
	points.push_back(Ogre::Vector3(-m_hs, m_hs, m_hs));
	points.push_back(Ogre::Vector3(-m_hs, -m_hs, m_hs));


	std::vector<int> vi{ 0,3,1,2,6,3,7,0,4,1,5,6,4,7 };


	strip.clear();
	for (size_t i = 0; i < vi.size(); ++i)
	{
		strip.push_back(points[vi[i]]);
	}
}
	//-----------------------------------------------------------------

Ogre::ManualObject* ProceduralShape::createCube(
	Ogre::SceneManager* i_pSceneMngr, 
	Ogre::SceneNode* i_pParent, 
	Ogre::String& i_name,
	float i_size)
{
	// create ManualObject
	Ogre::ManualObject* manual = i_pSceneMngr->createManualObject(i_name);

	// define start and end point
	float &s = i_size;
	std::vector<Ogre::Vector3> p{
		Ogre::Vector3(s, -s, -s), 
		Ogre::Vector3(s, s, -s),
		Ogre::Vector3(-s, s, -s),
		Ogre::Vector3(-s, -s, -s),
		Ogre::Vector3(s, -s, s),
		Ogre::Vector3(s, s, s),
		Ogre::Vector3(-s, s, s),
		Ogre::Vector3(-s, -s, s)
	};

	std::vector<std::vector<int>> f{ 
		{ 0,3,1,2 },
		{ 1,2,5,6 },
		{ 2,3,6,7 },
		{ 3,0,7,4 },
		{ 0,1,4,5 },
		{ 4,5,7,6 }
	};
	std::vector<Ogre::Vector3> n{
		Ogre::Vector3(0.f, 0.f, -1.f),
		Ogre::Vector3(0.f, 1.f, 0.f),
		Ogre::Vector3(-1.f, 0.f, 0.f),
		Ogre::Vector3(0.f, -1.f, 0.f),
		Ogre::Vector3(1.f, 0.f, 0.f),
		Ogre::Vector3(0.f, 0.f, 1.f),
	};
	std::vector<Ogre::Vector2> t{
		Ogre::Vector2(0.f, 0.f),
		Ogre::Vector2(0.f, 1.f),
		Ogre::Vector2(1.f, 0.f),
		Ogre::Vector2(1.f, 1.f)
	};



	for (size_t i = 0; i < f.size(); ++i)
	{
		manual->begin("BeanOfLight/Blue", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
		for (size_t j = 0; j < f[i].size(); ++j)
		{
			manual->position(p[f[i][j]].x, p[f[i][j]].y, p[f[i][j]].z);
			manual->normal(n[i].x, n[i].y, n[i].z);
			manual->textureCoord(t[j].x, t[j].y);
		}
		manual->end();
	}

	i_pParent->attachObject(manual);

	return manual;
}

//Ogre::ManualObject* createBlock(float i_sizeX, float i_sizeY, float i_sizeZ);
//Ogre::ManualObject* createAvatar(float i_sizeX, float i_sizeY, float i_sizeZ);