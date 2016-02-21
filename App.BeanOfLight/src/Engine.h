#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>

#include <Paging/OgrePageManager.h>
#include <Terrain/OgreTerrainPaging.h>

#include "TerrainCreator.h"
#include "AnimalThirdPersonControler.h"
#include "AnimalAIControler.h"

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
	OgreFramework();
	~OgreFramework();

	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	void updateOgre(double timeSinceLastFrame);
	void moveAvatar(double timeSinceLastFrame);

	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}  

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id); 
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	
	Ogre::Root*					m_pRoot;
	Ogre::SceneManager*			m_pSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Camera*				m_pCamera;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;
    Ogre::OverlaySystem*        m_pOverlaySystem;
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;    

	Animal*						m_pAvatar;
	AnimalThirdPersonControler*	m_pAvatarControler;

	Animal*						m_pPeon;
	AnimalAIControler*			m_pPeonControler;

	Animal*						m_pGuard;
	AnimalAIControler*			m_pGuardControler;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);

	OgreBites::SdkTrayManager*	m_pTrayMgr;
    Ogre::FrameEvent            m_FrameEvent;
	int							m_iNumScreenShots;

	bool						m_bShutDownOgre;
	
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real					m_MoveSpeed; 
	Ogre::Degree				m_RotateSpeed; 
	float						m_MoveScale; 
	Ogre::Degree				m_RotScale;

	double						m_timeSinceLastFrame; // Animation scale

private:

	Ogre::TerrainGroup* m_pTerrainGroup;
	Ogre::TerrainPaging* m_pTerrainPaging;
	Ogre::PageManager* m_pPageManager;
	Ogre::PagedWorld* m_pPagedWorld;
	Ogre::TerrainPagedWorldSection* m_pTerrainPagedWorldSection;

	OgreBites::Label* mInfoLabel;

	/// This class just pretends to provide procedural page content to avoid page loading
	class DummyPageProvider : public Ogre::PageProvider
	{
	public:
		bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	};
	DummyPageProvider mDummyPageProvider;
};