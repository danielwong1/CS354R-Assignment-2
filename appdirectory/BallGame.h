
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

class BallGame : public BaseApplication
{
public:
    static std::string ballString;
    static std::string botString;
    Physics* simulator;
    Score* scoreObj;
    
    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();
protected:
	Paddle* mPaddle;

    CEGUI::OgreRenderer* mRenderer;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    void setupCEGUI(void);
};

#endif 
