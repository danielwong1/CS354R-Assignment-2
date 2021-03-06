#include "BallGame.h"
#include <SDL2/SDL.h>
#include "src/audio.h"
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
#include "BallScoreCallback.h"
#include "BallPaddleCallback.h"


std::string BallGame::ballString = "ball";
std::string BallGame::botString = "bot";
int BallGame::rotationBound = 400;

BallGame::BallGame() : mRenderer(0)
{
    currentRotationX = 0;
    currentRotationY = 0;
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

    double cameraSpeed = 0.05;
    Ogre::Radian rotationSpeed = Ogre::Radian(Ogre::Degree(.1));
    Ogre::Vector3 cameraPos = mCamera->getPosition();



    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    if(mKeyboard->isKeyDown(OIS::KC_A)) {
        mCamera->setPosition(cameraPos + Ogre::Vector3(-cameraSpeed, 0, 0));
        mPaddle->moveBy(Ogre::Vector3(-cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_D)) {
        mCamera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed, 0, 0));
        mPaddle->moveBy(Ogre::Vector3(cameraSpeed, 0, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_W)) {
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, cameraSpeed, 0));
        mPaddle->moveBy(Ogre::Vector3(0, cameraSpeed, 0));
    }

    if(mKeyboard->isKeyDown(OIS::KC_S)) {
        mCamera->setPosition(cameraPos + Ogre::Vector3(0, -cameraSpeed, 0));
        mPaddle->moveBy(Ogre::Vector3(0, -cameraSpeed, 0));
    }

    mCamera->lookAt(Ogre::Vector3(0,-Wall::GRID_SIZE*3/4,-Wall::GRID_SIZE));

    if(mKeyboard->isKeyDown(OIS::KC_UP)) {
        if (currentRotationY < rotationBound) {
            currentRotationY++;
            mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(1, 0, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
        if (currentRotationX < rotationBound) {
            currentRotationX++;
            mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(0, 1, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_DOWN)) {
        if (currentRotationY > -rotationBound) {
            currentRotationY--;
            mPaddle->rotateBy(Ogre::Quaternion(rotationSpeed, Ogre::Vector3(1, 0, 0)));
        }
    }

    if(mKeyboard->isKeyDown(OIS::KC_LEFT)) {
        if (currentRotationX > -rotationBound) {
            currentRotationX--;
            mPaddle->rotateBy(Ogre::Quaternion(-rotationSpeed, Ogre::Vector3(0, 1, 0)));
        }
    }


    cameraPos = mCamera->getPosition();
    mCamera->setPosition(cameraPos + Ogre::Vector3(cameraSpeed * mRot.x, -cameraSpeed * mRot.y, 0));
    mPaddle->moveBy(Ogre::Vector3(cameraSpeed * mRot.x, -cameraSpeed * mRot.y, 0));
    mRot = Ogre::Vector2::ZERO;
    btVector3 position = mPaddle->getPosition();
    mPaddle->moveTo(Ogre::Vector3(
        Ogre::Math::Clamp(position.x(), -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
        Ogre::Math::Clamp(position.y(), -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
        position.z()
    ));
    cameraPos = mCamera->getPosition();
    mCamera->setPosition(Ogre::Vector3(
        Ogre::Math::Clamp(cameraPos.x, -(Wall::GRID_SIZE * 0.5f) + 3.0f, Wall::GRID_SIZE * 0.5f - 3.0f),
        Ogre::Math::Clamp(cameraPos.y, (Wall::GRID_SIZE * 0.125f) + 2.75f, Wall::GRID_SIZE * 1.125f - 3.0f),
        cameraPos.z
    ));
    mSceneMgr->setShadowFarDistance(mCamera->getPosition().z + Wall::GRID_SIZE);
    
     if(simulator != NULL) {
        simulator->dynamicsWorld->stepSimulation(simulator->physicsClock->getTimeSeconds());
        simulator->physicsClock->reset();


        // this collision handler is done in math rather than bullet
        btTransform ballTransform;
        mBall->motionState->getWorldTransform(ballTransform);
        btVector3 origin = ballTransform.getOrigin();

        if (origin.z() > Wall::GRID_SIZE) {
            ballTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
            ballTransform.setRotation(btQuaternion::getIdentity());

            mBall->motionState->setWorldTransform(ballTransform);
            mBall->body->setWorldTransform(ballTransform);
            mBall->body->clearForces();
            mBall->body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            mBall->body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));

            mBall->body->applyCentralImpulse(btVector3(0.0f, 0.0f, -1.0f));
            playSound("sounds/ball_miss.wav", SDL_MIX_MAXVOLUME / 10);
            scoreObj->setScore(0);
        }

        simulator->dynamicsWorld->contactPairTest(mBall->body, mWall->body, *mBallScoreCallback);
        if(mBall->colliding) {
            bool away = mBall->rootNode->getPosition().distance(mWall->rootNode->getPosition()) > Wall::GRID_SIZE / 4;
            if(collisionClock->getTimeSeconds() > .2 && away) {
                mBall->colliding = false;
            }
        }
        simulator->dynamicsWorld->contactPairTest(mBall->body, mPaddle->body, *mBallPaddleCallback);
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

void BallGame::setupSDL()
{
    /* Initialize only SDL Audio on default device */
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return;
    }

    initAudio();

    /* Play music and a sound */
    playMusic("music/highlands.wav", SDL_MIX_MAXVOLUME);
    //playSound("sounds/door1.wav", SDL_MIX_MAXVOLUME / 2);
}

void BallGame::createScene(void)
{
    setupCEGUI();
    setupSDL();
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

    Wall("leftWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_X);
    Wall("topWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_Y);
    Wall(botString, mSceneMgr, simulator, Ogre::Vector3::UNIT_Y);
    Wall("rightWall", mSceneMgr, simulator, Ogre::Vector3::NEGATIVE_UNIT_X);
    mWall = new Wall("backWall", mSceneMgr, simulator, Ogre::Vector3::UNIT_Z);
    mBall = new Ball(ballString, mSceneMgr, simulator);

    mPaddle = new Paddle(mSceneMgr, simulator);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    Ogre::Light* light = mSceneMgr->createLight("SpotLight");
    light->setPosition(Ogre::Vector3(0, Wall::GRID_SIZE / 4, 0));
    mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    light->setCastShadows(true);

    createCollisionCallbacks();
}

void BallGame::createCollisionCallbacks(void) {
    mBallScoreCallback = new BallScoreCallback(mBall, scoreObj, collisionClock);
    mBallPaddleCallback = new BallPaddleCallback(mBall, mPaddle);
}

bool BallGame::mouseMoved(const OIS::MouseEvent &ev)
{
    mRot.x = ev.state.X.rel;
    mRot.y = ev.state.Y.rel;
}

void BallGame::go()
{
    simulator = new Physics();
    collisionClock = new btClock();
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
