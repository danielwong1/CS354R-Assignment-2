#include <btBulletCollisionCommon.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Paddle.h"
#include "Physics.h"
#include <iostream>


Paddle::Paddle(Ogre::SceneManager* sceneMgr, 
    Physics* simulator) {

	this->name = "paddle";
    this->sceneMgr = sceneMgr;
    this->simulator = simulator;

    rootNode = this->sceneMgr->getRootSceneNode()->createChildSceneNode();
	geom = this->sceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
	rootNode->attachObject(geom);
    rootNode->scale(0.06f, 0.04f, 0.01f);

    geom->setCastShadows(false);
    geom->setMaterialName("Examples/Water0");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("Examples/Water0");
    mp.get()->setReceiveShadows(false);

	shape = new btBoxShape(btVector3(3.0f, 2.0f, 0.5f));
	mass = 0.0f;
	motionState = new OgreMotionState(tr, rootNode);

	body = new btRigidBody(mass, motionState, shape, inertia);

    body->setRestitution(1.05f);
    simulator->dynamicsWorld->addRigidBody(body);
    simulator->gameObjects.push_back(this);

    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
    moveBy(Ogre::Vector3(0.0f, 2.0f, 8.0f));    
}

void Paddle::moveBy(const Ogre::Vector3 &distance) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);

    btVector3 origin = worldTransform.getOrigin();
    worldTransform.setOrigin(origin + btVector3(distance.x, distance.y, distance.z));
    motionState->setWorldTransform(worldTransform);
}

void Paddle::rotateBy(const Ogre::Quaternion &rotation) {
    btTransform worldTransform;
    motionState->getWorldTransform(worldTransform);

    Ogre::Radian angle;
    Ogre::Vector3 axis;
    rotation.ToAngleAxis(angle, axis);

    btQuaternion bulletRotation(btVector3(axis.x, axis.y, axis.z), btScalar(angle.valueRadians()));
    btQuaternion currentRotation = worldTransform.getRotation();
    worldTransform.setRotation(currentRotation * bulletRotation);
    motionState->setWorldTransform(worldTransform);
}

