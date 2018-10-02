
#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

class Ball : public GameObject {
	Ball(Ogre::SceneManager* pSceneManager, Physics* pSimulator);
};

#endif

