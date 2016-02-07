#include "App.h"

#include "ProceduralShape.h"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>


App::App()
{
	m_pOgreHeadNode			= 0;
	m_pOgreHeadEntity		= 0;
}

App::~App()
{
	delete OgreFramework::getSingletonPtr();
}

void App::start()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0))
		return;
	
	m_bShutdown = false;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

	setupScene();
	run();
}

void App::setupScene()
{
	OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/EarlyMorningSkyBox");
	OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);

	Ogre::SceneNode* ogreNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->setPosition(5400, 150, 5400);
	ProceduralShape::createCube(
		OgreFramework::getSingletonPtr()->m_pSceneMgr,
		ogreNode,
		Ogre::String("MyCube"),
		100.f);
}

void App::run()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
	
	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

		Ogre::WindowEventUtilities::messagePump();
	
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
					
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
		
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

bool App::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		 //do something
	}

	return true;
}

bool App::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	
	return true;
}