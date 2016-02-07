#pragma once

#include "Engine.h"


class App : public OIS::KeyListener
{
public:
	App();
	~App();

	void start();
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

private:
	void setupScene();
	void run();

	Ogre::SceneNode*			m_pOgreHeadNode;
	Ogre::Entity*				m_pOgreHeadEntity;

	bool						m_bShutdown;
};