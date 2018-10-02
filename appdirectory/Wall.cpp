#include "Wall.h"

Wall::Wall(Ogre::String name, 
    Ogre::SceneManager* sceneMgr, 
    Physics* simulator,
    Ogre::Vector3 direction) {
    this.name = name;
    this.sceneMgr = sceneMgr;
    this.simulator = simulator;
    
    Ogre::MovablePlane plane(direction, -GRID_SIZE/2);
    Ogre::MeshManager::getSingleton().createPlane(
        name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, GRID_SIZE, GRID_SIZE, 20, 20, true, 1, 1, 1, 
        direction.perpendicular()
    );

    rootNode = this.sceneMgr->getRootSceneNode()->createChildSceneNode();
    geom = this.sceneMgr->createEntity(name);
    rootNode->attachObject(geom);

    wallEntity->setMaterialName("sibenik/poplocenje");
    Ogre::MaterialPtr mp = Ogre::MaterialManager::getSingleton().getByName("sibenik/poplocenje");
    mp.get()->setReceiveShadows(true);
    wallEntity->setCastShadows(false);

    /*shape = new btPlaneShape(btVector3(direction.x, direction.y, direction.z), 0.0f);
    mass = 0.0;
    tr = 
    motionState = new OgreMotionState(tr, rootNode);
    body = btRigidBody(mass, motionState, shape);
    
    inertia =*/
}