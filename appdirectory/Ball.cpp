
#include <btBulletCollisionCommon.h>
#include "Ball.h"

Ball::Ball(Ogre::SceneManager* pSceneManager, Physics* pSimulator)
	: GameObject() {

	name = "Ball";
	sceneMgr = pSceneManager;
	simulator = pSimulator;

	geom = sceneMgr->createEntity("sphere", "sphere.mesh");
	geom->setCastShadows(true);
    geom->setMaterialName("OceanHLSL_GLSL");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("OceanHLSL_GLSL");
    mp.get()->setReceiveShadows(false);

	rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);

	rootNode->attachObject(geom);
	rootNode->scale(0.1f, 0.1f, 0.1f);

	shape = new btSphereShape(BT_ONE);
	mass = 5.0f;
	motionState = new OgreMotionState(tr, rootNode);
	intertia = btVector3(0.1f, 0.1f, 0.1f);

	body = new btRigidBody(mass, motionState, shape, intertia);
}
