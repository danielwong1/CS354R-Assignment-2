# CS354R-Assignment-2
Game of Ball

# Building

`$ cd ./appdirectory && ./buildit`

`$./makeit`

# Playing

Once you've [built](#building) the application, just run `$ ./OgreApp` from `appdirectory`

# Controls

Pan the paddle and the camera using WASD. Rotate the paddle using the arrow keys

# Implementation Progress:

	1. GUI - Placeholder GUI currently, on track to update and keep score by turn in
	2. Sound - No sound currently, but on track to finish the two sounds by turn in
	3. Physics - All major pieces implemented, just need special collision callbacks for scoring and paddle + ball collision
	4. Camera controls - Done. Will probably restrict the camera/paddle movement but all logic is implemented

	Misc: Will also want to include a reset mechanic and probably update graphics before turn in, but this will be as simple as swapping some materials

# Distribution of remaining work:

Sound and GUI can be handled by 1 to 2 people. The hardest part of this will be including the sound library, but we've had good luck integrating our third party libraries so far.

Collision callbacks will be a task unto themselves, but we only have a couple special ones. Definitely doable in a week.

Miscellaneous testing and quality control can be a task unto itself. Just adding some polish

# Are we behind?

Not really. Assuming sound doesn't give us any trouble, we just need to implement the last few pieces of logic and polish the project before turning it in.


