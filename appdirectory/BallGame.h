
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

class BallGame : public BaseApplication
{
public:
    static std::string ballString;
    static std::string botString;
    static int rotationBound;
    Physics* simulator;
    Score* scoreObj;
    btClock* collisionClock;
    Ogre::Vector2 mRot;

    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();

    void createCollisionCallbacks(void);
protected:
	Paddle* mPaddle;
	Ball* mBall;
	Wall* mWall;

	BallScoreCallback* mBallScoreCallback;
	BallPaddleCallback* mBallPaddleCallback;

    CEGUI::OgreRenderer* mRenderer;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    virtual bool mouseMoved(const OIS::MouseEvent &ev);
    void setupCEGUI(void);
    void setupSDL(void);
private:
    int currentRotationX;
    int currentRotationY;
};

#endif 
