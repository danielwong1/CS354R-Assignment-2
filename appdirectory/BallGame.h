
#ifndef __BallGame_h_
#define __BallGame_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <string>
#include "Physics.h"
#include "BaseApplication.h"
#include "Score.h"

class Paddle;
class Ball;
class Wall;
class BallScoreCallback;
class BallPaddleCallback;
class BallFloorCallback;

class BallGame : public BaseApplication
{
public:
    static std::string ballString;
    static std::string botString;
    Physics* simulator;
    Score* scoreObj;
    btClock* collisionClock;
    btClock* f_collisionClock;
    bool started;

    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();

    void createCollisionCallbacks(void);
protected:
	Paddle* mPaddle;
	Ball* mBall;
	Wall* mWall;
    Wall* bWall;

	BallScoreCallback* mBallScoreCallback;
	BallPaddleCallback* mBallPaddleCallback;
    BallFloorCallback* mBallFloorCallback;
    CEGUI::OgreRenderer* mRenderer;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    void setupCEGUI(void);
<<<<<<< HEAD
    void reset(btTransform ballTransform, btVector3 origin);
=======
    void setupSDL(void);
>>>>>>> 79e53e9c51f94d2e82c48658d776f06a1d0d0ef7
};

#endif 
