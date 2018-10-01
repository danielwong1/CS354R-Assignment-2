#include "BallApplication.h"
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

BallApplication::BallApplication() : mRoot(0), 
    mResourcesCfg(Ogre::StringUtil::BLANK), 
    mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

BallApplication::~BallApplication(void)
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//Adjust mouse clipping area
void BallApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BallApplication::windowClosed(Ogre::RenderWindow* rw)
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

bool BallApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

    mSceneMgr->setShadowFarDistance(mCamera->getPosition().z + gridSize);
    Ogre::SceneNode* ball = mSceneMgr->getSceneNode("Ball");
    Ogre::Vector3 bPosition = ball->getPosition();
    
    double bRadius = 10.0; 

    if (bPosition.y < -gridSize/2.0f + bRadius && bVelocity.y < 0.0f) bVelocity.y = -bVelocity.y;
    else if (bPosition.y > gridSize/2.0f - bRadius && bVelocity.y > 0.0f) bVelocity.y = -bVelocity.y;
    if (bPosition.z < -gridSize/2.0f + bRadius && bVelocity.z < 0.0f) bVelocity.z = -bVelocity.z;
    else if (bPosition.z > gridSize/2.0f - bRadius && bVelocity.z > 0.0f) bVelocity.z = -bVelocity.z;
    if (bPosition.x < -gridSize/2.0f + bRadius && bVelocity.x < 0.0f) bVelocity.x = -bVelocity.x;
    else if (bPosition.x > gridSize/2.0f - bRadius && bVelocity.x > 0.0f) bVelocity.x = -bVelocity.x;
    bPosition = ball->getPosition();

    ball->translate(bVelocity * evt.timeSinceLastFrame);
    bPosition = ball->getPosition();

    return true;
}

void BallApplication::createWall(std::string name, Ogre::Vector3 direction) {
    Ogre::MovablePlane plane( direction, -gridSize/2 );

    Ogre::MeshManager::getSingleton().createPlane(
        name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, gridSize, gridSize, 20, 20, true, 1, 1, 1, 
        direction.perpendicular()
    );

    Ogre::Entity* wallEntity = mSceneMgr->createEntity(name);
    Ogre::SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    sceneNode->attachObject(wallEntity);

    //Setting up materials and casted shadows for the wall
    wallEntity->setMaterialName("sibenik/poplocenje");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("sibenik/poplocenje");
    mp.get()->setReceiveShadows(true);
    wallEntity->setCastShadows(false);
}

void BallApplication::createScene(void)
{
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    mSceneMgr->setShadowFarDistance(gridSize);

    Ogre::Entity* ballEntity = mSceneMgr->createEntity("sphere", "sphere.mesh");
    Ogre::SceneNode* ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Ball");
    ballEntity->setCastShadows(true);
    ballEntity->setMaterialName("OceanHLSL_GLSL");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("OceanHLSL_GLSL");
    mp.get()->setReceiveShadows(false);
    ballNode->attachObject(ballEntity);
    ballNode->scale(0.1f,0.1f,0.1f); 

    createWall("frontPlane", Ogre::Vector3::NEGATIVE_UNIT_Z);
    createWall("backPlane", Ogre::Vector3::UNIT_Z);
    createWall("leftPlane", Ogre::Vector3::UNIT_X);
    createWall("rightPlane", Ogre::Vector3::NEGATIVE_UNIT_X);
    createWall("topPlane", Ogre::Vector3::NEGATIVE_UNIT_Y);
    createWall("botPlane", Ogre::Vector3::UNIT_Y);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);
    light->setPosition(0, 0, 0);
}

bool BallApplication::go()
{
    #ifdef _DEBUG
      mResourcesCfg = "resources_d.cfg";
      mPluginsCfg = "plugins_d.cfg";
    #else
      mResourcesCfg = "resources.cfg";
      mPluginsCfg = "plugins.cfg";
    #endif

    srand (time(NULL));
    double x = 20 * (double)rand() / RAND_MAX;
    double y = 20 * (double)rand() / RAND_MAX;
    double z = 20 * (double)rand() / RAND_MAX;
    gridSize=100;
    bVelocity = Ogre::Vector3(x, y, z);

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
    mWindow = mRoot->initialise(true, "BallApplication Render Window");

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    
    mCamera = mSceneMgr->createCamera("MainCam");

    mCamera->setPosition(0, 0, 100);
    mCamera->lookAt(0, 0, 0);
    mCamera->setNearClipDistance(5);

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);

    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    mCamera->setAspectRatio(
      Ogre::Real(vp->getActualWidth()) / 
      Ogre::Real(vp->getActualHeight()));

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
        BallApplication app;

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
