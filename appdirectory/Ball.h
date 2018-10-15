#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Ball : public GameObject {
    public:
    bool colliding;
    Ball(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator);
};

#endif