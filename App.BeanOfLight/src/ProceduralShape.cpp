#include "ProceduralShape.h"

#include <OgreVector3.h>
#include <OgreManualObject.h>

///////////////////////////////////////////////////////////////////////////////
// Helper Functions
//   0--1
//   | /|
//   |/ |
//   3--2

void addQuad(
	const std::vector<Ogre::Vector3>& i_clockwiseQuad,
	const Ogre::Vector3& i_normal,
	size_t i_nRows,
	size_t i_nCols,
	Ogre::String& i_material,
	Ogre::ManualObject& io_object)
{
	assert(i_clockwiseQuad.size() == 4);
	assert(i_nRows != 0 && i_nCols != 0);

	const std::vector<Ogre::Vector3>& box = i_clockwiseQuad;
	const Ogre::Vector3& n = i_normal;
	float ratioCol = 1.f / (float)(i_nCols);
	float ratioRow = 1.f / (float)(i_nRows);
	Ogre::Vector3 stepCol = (box[1] - box[0]) * ratioCol;
	Ogre::Vector3 stepRow = (box[3] - box[0]) * ratioRow;

	std::vector<Ogre::Vector3> cur(4);

	for (size_t r = 0; r < i_nRows; ++r)
	{
		io_object.begin(i_material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
		for (size_t c = 0; c < i_nCols; ++c)
		{
			cur[0] = box[0] + stepRow * (float)r + stepCol * (float)c;
			cur[1] = box[0] + stepRow * (float)r + stepCol * (float)(c+1);
			cur[2] = box[0] + stepRow * (float)(r+1) + stepCol * (float)(c+1);
			cur[3] = box[0] + stepRow * (float)(r+1) + stepCol * (float)(c);

			io_object.position(cur[0].x, cur[0].y, cur[0].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)r, ratioCol*(float)c);

			io_object.position(cur[3].x, cur[3].y, cur[3].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)(r+1), ratioCol*(float)c);

			io_object.position(cur[1].x, cur[1].y, cur[1].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)r, ratioCol*(float)(c+1));

			io_object.position(cur[1].x, cur[1].y, cur[1].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)r, ratioCol*(float)(c + 1));

			io_object.position(cur[3].x, cur[3].y, cur[3].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)(r + 1), ratioCol*(float)c);

			io_object.position(cur[2].x, cur[2].y, cur[2].z);
			io_object.normal(n.x, n.y, n.z);
			io_object.textureCoord(ratioRow*(float)(r+1), ratioCol*(float)(c + 1));

		}
		io_object.end();
	}
}

///////////////////////////////////////////////////////////////////////////////

//     7-----6
//    /|    /|
//   4-----5 |
//   | 3---|-2
//   |/    |/
//   0-----1

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

	std::vector<std::vector<int>> q{
		{ 0,1,2,3 },
		{ 1,5,6,2 },
		{ 2,6,7,3 },
		{ 3,7,4,0 },
		{ 0,4,5,1 },
		{ 4,7,6,5 }
	};

	std::vector<Ogre::Vector3> n{
		Ogre::Vector3(0.f, 0.f, -1.f),
		Ogre::Vector3(0.f, 1.f, 0.f),
		Ogre::Vector3(-1.f, 0.f, 0.f),
		Ogre::Vector3(0.f, -1.f, 0.f),
		Ogre::Vector3(1.f, 0.f, 0.f),
		Ogre::Vector3(0.f, 0.f, 1.f),
	};

	for (size_t i = 0; i < q.size(); ++i)
	{
		std::vector<Ogre::Vector3> clockwiseQuad{
			p[q[i][0]], p[q[i][1]], p[q[i][2]], p[q[i][3]] };
		addQuad(clockwiseQuad, n[i], 10, 10, Ogre::String("BeanOfLight/Blue"), *manual);
	}

	i_pParent->attachObject(manual);

	return manual;
}

//     19------------18
//    /|            /|
//   16------------17|
//   | |           | |
//   | 15---14--13-|-12
//   | |   /|  /|  | |        Z (h)
//   8----9---10---11|        |
//   | 3--|-2 | 7--|-6        0---Y (w)
//   |/   |/  |/   |/        /
//   0----1   4----5        X (d)

Ogre::ManualObject* ProceduralShape::createAvatar(
	Ogre::SceneManager* i_pSceneMngr,
	Ogre::SceneNode* i_pParent,
	Ogre::String& i_name,
	float i_height,
	float i_width,
	float i_depth,
	float i_sameHeight,
	float i_seamWidth,
	float i_resolution)
{
	// create ManualObject
	Ogre::ManualObject* manual = i_pSceneMngr->createManualObject(i_name);

	// define start and end point
	const float &h = i_height / 2.f;
	const float &w = i_width / 2.f;
	const float &d = i_depth / 2.f;
	const float &sh = i_sameHeight;
	const float &sw = i_seamWidth / 2.f;

	std::vector<Ogre::Vector3> p{
		Ogre::Vector3(d, -w, -h), Ogre::Vector3(d, -sw, -h), Ogre::Vector3(-d, -sw, -h), Ogre::Vector3(-d, -w, -h), // 0, 1, 2, 3
		Ogre::Vector3(d, sw, -h), Ogre::Vector3(d, w, -h), Ogre::Vector3(-d, w, -h), Ogre::Vector3(-d, sw, -h), // 4, 5, 6, 7
		Ogre::Vector3(d, -w, -h + sh), Ogre::Vector3(d, -sw, -h + sh), Ogre::Vector3(d, sw, -h + sh), Ogre::Vector3(d, w, -h + sh), // 8, 9, 10, 11
		Ogre::Vector3(-d, w, -h + sh), Ogre::Vector3(-d, sw, -h + sh), Ogre::Vector3(-d, -sw, -h + sh), Ogre::Vector3(-d, -w, -h + sh), // 12, 13, 14, 15
		Ogre::Vector3(d, -w, h), Ogre::Vector3(d, w, h), Ogre::Vector3(-d, w, h), Ogre::Vector3(-d, -w, h), // 16, 17, 18, 19
	};

	std::vector<std::vector<std::vector<int>>> q{
		{ { 0, 1, 2, 3 }, { 9, 10, 13, 14 }, { 4, 5, 6, 7 } },
		{ { 5, 17, 18, 6 }, { 1, 9, 14, 2 } },
		{ { 17, 16, 19, 18 } },
		{ { 19, 16, 0, 3 }, { 10, 4, 7, 13 } },
		{ { 0, 8, 9, 1 }, { 4, 10 , 11, 5}, { 8, 16, 17, 11 } },
		{ { 2 ,14 ,15 ,3 }, {6, 12, 13, 7}, {12, 18, 19, 15 } }
	};

	std::vector<Ogre::Vector3> n{
		Ogre::Vector3(0.f, 0.f, -1.f),
		Ogre::Vector3(0.f, 1.f, 0.f),
		Ogre::Vector3(0.f, 0.f, 1.f),
		Ogre::Vector3(0.f, -1.f, 0.f),
		Ogre::Vector3(1.f, 0.f, 0.f),
		Ogre::Vector3(-1.f, 0.f, 0.f),
	};

	for (size_t f = 0; f < q.size(); f++) // for each face
	{
		for (size_t iq = 0; iq < q[f].size(); iq++) // for each quad
		{
			std::vector<Ogre::Vector3> clockwiseQuad{
				p[q[f][iq][0]], p[q[f][iq][1]], p[q[f][iq][2]], p[q[f][iq][3]] };
			
			size_t nCols = ceil((clockwiseQuad[0] - clockwiseQuad[1]).length() / i_resolution);
			size_t nRows = ceil((clockwiseQuad[0] - clockwiseQuad[3]).length() / i_resolution);
			addQuad(clockwiseQuad, n[f], nRows, nCols, Ogre::String("BeanOfLight/Blue"), *manual);
		}
	}

	i_pParent->attachObject(manual);

	return manual;
}