#ifndef __PHYSICS_H_
#define __PHYSICS_H_

#include <vector>
#include "GameObject.h"
#include <btBulletDynamicsCommon.h>

class Physics {
    btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<GameObject *> gameObjects;

    public:
    Physics();
};

#endif