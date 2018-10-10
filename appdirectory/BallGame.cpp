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
#include "Paddle.h"

std::string BallGame::ballString = "ball";
std::string BallGame::botString = "bot";
BallGame::BallGame() : mRenderer(0)
{
}

BallGame::~BallGame(void)
{
    CEGUI::OgreRenderer::destroySystem();
}

bool BallGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    double cameraSpeed = 0.01;
    double zoomSpeed = 0.01;
    Ogre::Radian rotationSpeed = Ogre::Radian(Ogre::Degree(.1));

    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    if(mKeyboard->isKeyDown(OIS::KC_A)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(-cameraSpeed, 0, 0));
        mPaddle->moveBy(Ogre::Vector3(-cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_D)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed, 0, 0));
        mPaddle->moveBy(Ogre::Vector3(cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_W)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, cameraSpeed, 0));
        mPaddle->moveBy(Ogre::Vector3(0, cameraSpeed, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_S)) {
        Ogre::Vector3 cameraPos = mCamera->getPosition();
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, -cameraSpeed, 0));
        mPaddle->moveBy(Ogre::Vector3(0, -cameraSpeed, 0));
    }

    mCamera->lookAt(Ogre::Vector3(0,-Wall::GRID_SIZE*3/4,-Wall::GRID_SIZE));

    if(mKeyboard->isKeyDown(OIS::KC_UP)) {
        mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(1, 0, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
        mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(0, 1, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_DOWN)) {
        mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(1, 0, 0)));
    }

    if(mKeyboard->isKeyDown(OIS::KC_LEFT)) {
        mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(0, 1, 0)));
    }

    mSceneMgr->setShadowFarDistance(mCamera->getPosition().z + Wall::GRID_SIZE);
    
     if(simulator != NULL) {
        simulator->dynamicsWorld->stepSimulation(simulator->physicsClock->getTimeSeconds());
        simulator->physicsClock->reset();
        /*int numManifolds = simulator->dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = simulator->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            int numContacts = contactManifold->getNumContacts();
            if (numContacts > 0)
            {
                const btCollisionObject* obA = contactManifold->getBody0();
                const btCollisionObject* obB = contactManifold->getBody1();
                
                GameObject* a = (GameObject*)obA->getUserPointer();
                GameObject* b = (GameObject*)obB->getUserPointer();
                
                printf("astring : %s\n", a->name.c_str());
                printf("bstring : %s\n", b->name.c_str());
                printf("\n");
                if (a->name == ballString) {
                    if(b->name == botString) {
                        scoreObj->setScore(scoreObj->score + 1);
                    }
                }
                
                //8
                if (b->name == ballString){
                    if(a->name == botString) {
                        scoreObj->setScore(scoreObj->score + 1);
                    }
                }
            }
        }*/
    }
    return true;
}

void BallGame::setupCEGUI(void) {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    scoreObj = new Score();
}

void BallGame::createScene(void)
{
    setupCEGUI();
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    Wall("leftWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_X);
    Wall("topWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_Y);
    Wall(botString, mSceneMgr, simulator, Ogre::Vector3::UNIT_Y);
    Wall("rightWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_X);
    Wall("backWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_Z);
    Ball(ballString, mSceneMgr, simulator);

    mPaddle = new Paddle(mSceneMgr, simulator);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    light->setPosition(Ogre::Vector3(0, Wall::GRID_SIZE / 4, 0));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);
}

void BallGame::go()
{

    simulator = new Physics();
    BaseApplication::go();
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