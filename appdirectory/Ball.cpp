#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include "Ball.h"
#include "Physics.h"


Ball::Ball(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator) {

	this->name = name;
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;

    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
	geom = this->sceneMgr->createEntity(name, "sphere.mesh");
	rootNode->attachObject(geom);
	rootNode->scale(0.01f, 0.01f, 0.01f);
    	
    geom->setCastShadows(true);
    geom->setMaterialName("OceanHLSL_GLSL");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("OceanHLSL_GLSL");
    mp.get()->setReceiveShadows(false);

	shape = new btSphereShape(btScalar(1.0f));
	mass = 0.05f;
    shape->calculateLocalInertia(mass, inertia);
	motionState = new OgreMotionState(tr, rootNode);
        
	body = new btRigidBody(mass, motionState, shape, inertia);
    body->setRestitution(0.85f);
    simulator->dynamicsWorld->addRigidBody(body);
    simulator->gameObjects.push_back(this);
}