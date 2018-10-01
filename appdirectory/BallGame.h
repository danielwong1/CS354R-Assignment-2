
#ifndef __BallGame_h_
#define __BallGame_h_

#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <string>
//---------------------------------------------------------------------------

class BallGame : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
    OIS::InputManager* mInputManager;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;
    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Camera* mCamera;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

    Ogre::Vector3 bVelocity;
    Ogre::Entity* ballEntity;
    int gridSize;
    BallGame(void);
    virtual ~BallGame(void);
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    bool go();
    void createWall(std::string name, Ogre::Vector3 direction);
protected:
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual void createScene(void);
};

//---------------------------------------------------------------------------

#endif // #ifndef __BallGame_h_

//---------------------------------------------------------------------------
