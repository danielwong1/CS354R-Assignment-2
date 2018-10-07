#include "BallGame.h"
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector3.h>
#include <OgreMeshManager.h>
#include <time.h>
#include <string>
#include "Wall.h"
#include "Ball.h"

BallGame::BallGame() : mRoot(0), 
    mResourcesCfg(Ogre::StringUtil::BLANK), 
    mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

BallGame::~BallGame(void)
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//Adjust mouse clipping area
void BallGame::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BallGame::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

bool BallGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    double cameraSpeed = 0.1;
    double zoomSpeed = 1.0;
    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    if(mKeyboard->isKeyDown(OIS::KC_Q)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, 0, zoomSpeed));
    }

    if(mKeyboard->isKeyDown(OIS::KC_E)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, 0, -zoomSpeed));
    }

    if(mKeyboard->isKeyDown(OIS::KC_A)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(-cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_D)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_W)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, cameraSpeed, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_S)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, -cameraSpeed, 0));
    }

    /*mSceneMgr->setShadowFarDistance(mCamera->getPosition().z + gridSize);
    Ogre::SceneNode* ball = mSceneMgr->getSceneNode("Ball");
    Ogre::Vector3 bPosition = ball->getPosition();*/
    
    if(simulator != NULL) {
        simulator->dynamicsWorld->stepSimulation(1.0f/60.0f);
    }
    return true;
}

void BallGame::createScene(void)
{
    /*mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    mSceneMgr->setShadowFarDistance(gridSize);*/

    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("General");
    CEGUI::Font::setDefaultResourceGroup("General");
    CEGUI::Scheme::setDefaultResourceGroup("General");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("General");
    CEGUI::WindowManager::setDefaultResourceGroup("General");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

    sheet->addChild(quit);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
  
    Wall("leftWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_X);
    Wall("topWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_Y);
    Wall("botWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_Y);
    Wall("rightWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_X);
    Wall("backWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_Z);
    Ball("mainBall", mSceneMgr, simulator);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);
    light->setPosition(0, 0, 0);
}

bool BallGame::go()
{
    #ifdef _DEBUG
      mResourcesCfg = "resources_d.cfg";
      mPluginsCfg = "plugins_d.cfg";
    #else
      mResourcesCfg = "resources.cfg";
      mPluginsCfg = "plugins.cfg";
    #endif

    mRoot = new Ogre::Root(mPluginsCfg);
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    Ogre::String name, locType;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
    while (secIt.hasMoreElements())
    {
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it)
        {
            locType = it->first;
            name = it->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
        }
    }
    
    if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
    {
        return false;
    }
    mWindow = mRoot->initialise(true, "BallGame Render Window");

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mCamera = mSceneMgr->createCamera("MainCam");
    mCamera->setPosition(0, 0, 200);
    mCamera->lookAt(0, 0, 0);
    mCamera->setNearClipDistance(5);

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    mCamera->setAspectRatio(
      Ogre::Real(vp->getActualWidth()) / 
      Ogre::Real(vp->getActualHeight()));


    simulator = new Physics();  
    createScene();

    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    
    mInputManager = OIS::InputManager::createInputSystem( pl );
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    mRoot->addFrameListener(this);
    mRoot->startRendering();
    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BallGame app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
