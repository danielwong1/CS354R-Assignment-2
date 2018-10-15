#include <SDL2/SDL.h>
#include "BallScoreCallback.h"
#include "Ball.h"
#include "Score.h"
#include "src/audio.h"

/**
 * Constructs the BallBoundaryCallback
 *
 * @constructor
 * @param pBall
 */
BallScoreCallback::BallScoreCallback(Ball* pBall, Score* pScore, btClock* pCollisionClock) : 
mBall(pBall), mScore(pScore), collisionClock(pCollisionClock){}

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
	if(!mBall->colliding) {
		mBall->colliding = true;
        playSound("sounds/ball_hit_wall.wav", SDL_MIX_MAXVOLUME / 10);
		collisionClock->reset();
		mScore->setScore(mScore->score + 1);
	}
}