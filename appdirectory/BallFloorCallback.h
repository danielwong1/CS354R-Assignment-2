#ifndef __BallFloorCallback_h_
#define __BallFloorCallback_h_

#include <btBulletCollisionCommon.h>

class Ball;

struct BallFloorCallback : public btCollisionWorld::ContactResultCallback {

	BallFloorCallback(Ball* ball, btClock* pCollisionClock);
 public:
  btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1);

 private:
 	Ball* mBall;
	btClock* collisionClock;
};

#endif