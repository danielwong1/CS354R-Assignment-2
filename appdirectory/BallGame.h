
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

class BallGame : public BaseApplication
{
public:
    Physics* simulator;
    
    BallGame(void);
    virtual ~BallGame(void);
    virtual void go();
protected:
    CEGUI::OgreRenderer* mRenderer;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
    void setupCEGUI(void);
};

#endif 
