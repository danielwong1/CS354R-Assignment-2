
#include "BallScoreCallback.h"
#include "Ball.h"
#include "Score.h"

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallScoreCallback::BallScoreCallback(Ball* pBall, Score* pScore) : mBall(pBall), mScore(pScore) {}

/**
 * Callback for whenever the ball collides with the Score
 *
 */
btScalar BallScoreCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {

	// reset the score
	mScore->setScore(mScore->score + 1);
	mBall->body->applyCentralImpulse(btVector3(0.0f, 0.0f, 0.1f));
}