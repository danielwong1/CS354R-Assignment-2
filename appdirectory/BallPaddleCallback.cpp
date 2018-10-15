
#include "BallPaddleCallback.h"
#include "Ball.h"
#include "Paddle.h"

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallPaddleCallback::BallPaddleCallback(Ball* pBall, Paddle* pPaddle) : mBall(pBall), mPaddle(pPaddle) {}

/**
 * Callback for whenever the ball collides with the Paddle
 *
 */
btScalar BallPaddleCallback::addSingleResult(btManifoldPoint& cp,
	const btCollisionObjectWrapper* colObj0Wrap,
	int partId0,
	int index0,
	const btCollisionObjectWrapper* colObj1Wrap,
	int partId1,
	int index1) {


	btQuaternion quaternion;
	mPaddle->getRotation(quaternion);

	btVector3 axis = quaternion.getAxis();
	btScalar angle = quaternion.getAngle();
	btVector3 initialNormal(0.0f, 0.0f, -1.0f);
	btVector3 rotatedNormal = initialNormal.rotate(axis, angle);

	mBall->body->applyCentralImpulse(0.1f * rotatedNormal);
}