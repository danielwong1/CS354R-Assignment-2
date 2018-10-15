#include "BallFloorCallback.h"
#include "Ball.h"
#include "Score.h"

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallFloorCallback::BallFloorCallback(Ball* pBall, btClock* pCollisionClock) : 
mBall(pBall), collisionClock(pCollisionClock){}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallFloorCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {

	// reset the score
	if(!mBall->f_colliding) {
		mBall->f_colliding = true;
        mBall->floorBounces++;
		collisionClock->reset();
	}
}