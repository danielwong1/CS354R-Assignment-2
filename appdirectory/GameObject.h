#include <OgreRoot.h>
#include <btScalar.h>
#include <btTransform.h>
#include <btVector3.h>
#include "Physics.h"

class GameObject {
    protected:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Physics* simulator;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    btCollisionShape* shape;
    btScalar mass;
    btRigidBody* body;
    btTransform tr;
    btVector3 inertia;
    OgreMotionState* motionState;
}