#include "Engine.h"

#include "AnimalFactory.h"

using namespace Ogre;

template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

OgreFramework::OgreFramework()
{
    m_MoveSpeed			= 2.f;
    m_RotateSpeed		= 0.2f;

    m_bShutDownOgre		= false;
    m_iNumScreenShots	= 0;

    m_pRoot				= 0;
    m_pSceneMgr			= 0;
    m_pRenderWnd		= 0;
    m_pCamera			= 0;
	m_pAvatar           = 0;
    m_pViewport			= 0;
    m_pLog				= 0;
    m_pTimer			= 0;

    m_pInputMgr			= 0;
    m_pKeyboard			= 0;
    m_pMouse			= 0;

    m_pTrayMgr          = 0;
    m_FrameEvent        = Ogre::FrameEvent();

	m_timeSinceLastFrame = 0.f;
}

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    if(!m_pRoot->showConfigDialog())
        return false;
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pOverlaySystem = new Ogre::OverlaySystem();
    m_pSceneMgr->addRenderQueueListener(m_pOverlaySystem);

    m_pCamera = m_pSceneMgr->createCamera("Camera");
    m_pCamera->setPosition(Vector3(6300.f, 150.0f, 6300.f));
    m_pCamera->lookAt(Vector3(0, 150.0f, 0.f));
    m_pCamera->setNearClipDistance(0.1f);

    m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
    m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));

    m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));

    m_pViewport->setCamera(m_pCamera);

    size_t hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    OgreBites::InputContext inputContext;
    inputContext.mMouse = m_pMouse;
    inputContext.mKeyboard = m_pKeyboard;
    m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, inputContext, this);
    m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    m_pTrayMgr->hideCursor();

    m_pRenderWnd->setActive(true);

	// Initialize Terrain
	TerrainCreator terrain;
	m_pTerrainGroup = terrain.initTerrain(*m_pSceneMgr);

	// Create Avatar & third person controler
	m_pAvatar = AnimalFactory::createHeroBean(
		OgreFramework::getSingletonPtr()->m_pSceneMgr,
		Ogre::Vector3(6400, 0.f, 5400),
		Ogre::Quaternion::IDENTITY,
		Ogre::String("MyAvatar"));
	m_pAvatarControler = new AnimalThirdPersonControler();
	m_pAvatarControler->attach(m_pAvatar, m_pCamera, m_pTerrainGroup);


	// Create a peon & AI controler
	m_pPeon = AnimalFactory::createPeonBean(
		OgreFramework::getSingletonPtr()->m_pSceneMgr,
		Ogre::Vector3(5000, 0.f, -4000),
		Ogre::Quaternion::IDENTITY,
		Ogre::String("Peon1"));
	m_pPeonControler = new AnimalAIControler();
	m_pPeonControler->attach(m_pPeon, m_pAvatar, m_pTerrainGroup);
	std::vector<Ogre::Vector3> w{
		Ogre::Vector3(0.f, 0.f, 0.f),
		Ogre::Vector3(0.f, 0.f, -12800.f),
		Ogre::Vector3(12800.f, 0.f, -12800.f),
		Ogre::Vector3(12800.f, 0.f, 0.f)
	};
	m_pPeonControler->setWayPoints(w);

    return true;
}

OgreFramework::~OgreFramework()
{
    if (m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if (m_pTrayMgr)  delete m_pTrayMgr;
    if (m_pRoot)     delete m_pRoot;
	if (m_pAvatarControler)   delete m_pAvatarControler;
	if (m_pAvatar)   delete m_pAvatar;
	if (m_pPeon)     delete m_pPeon;
}

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bShutDownOgre = true;
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".jpg");
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;

        if(mode == 2)
        {
            m_pCamera->setPolygonMode(PM_SOLID);
            mode = 0;
        }
        else if(mode == 0)
        {
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
        }
        else if(mode == 1)
        {
            m_pCamera->setPolygonMode(PM_POINTS);
            mode = 2;
        }
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
    }

    return true;
}

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pAvatarControler->orient(evt.state.X.rel, evt.state.Y.rel, m_timeSinceLastFrame);

	return true;
}

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pAvatarControler->m_lookAround = true;
	return true;
}

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	m_pAvatarControler->m_lookAround = false;
	return true;
}

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_timeSinceLastFrame = timeSinceLastFrame;
	// Do not go under 10PFS
	m_timeSinceLastFrame = std::min(m_timeSinceLastFrame, 100.);

    m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

	m_pPeonControler->move(m_timeSinceLastFrame);
	moveAvatar(m_timeSinceLastFrame);

    m_FrameEvent.timeSinceLastFrame = m_timeSinceLastFrame;
    m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

void OgreFramework::moveAvatar(double i_timeSinceLastFrame)
{
	int moveForward = 0;
	int straffe = 0;

	if (m_pKeyboard->isKeyDown(OIS::KC_A))
		straffe = 1;

	if (m_pKeyboard->isKeyDown(OIS::KC_D))
		straffe = -1;

	if (m_pKeyboard->isKeyDown(OIS::KC_W))
		moveForward = 1;

	if (m_pKeyboard->isKeyDown(OIS::KC_S))
		moveForward = -1;
	
	bool run = false;
	if (m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		run = true;

	m_pAvatarControler->move(moveForward, straffe, run, i_timeSinceLastFrame);
}