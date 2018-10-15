
#ifndef __BallScoreCallback_h_
#define __BallScoreCallback_h_

#include <btBulletCollisionCommon.h>

class Ball;
class Score;

struct BallScoreCallback : public btCollisionWorld::ContactResultCallback {

	BallScoreCallback(Ball* ball, Score* score, btClock* pCollisionClock);
 public:
  btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1);

 private:
 	Score* mScore;
 	Ball* mBall;
	btClock* collisionClock;
};

#endif