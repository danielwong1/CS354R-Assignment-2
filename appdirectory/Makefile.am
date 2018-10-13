ACLOCAL_AMFLAGS= -I m4

noinst_HEADERS= BallGame.h Physics.h GameObject.h Wall.h Ball.h OgreMotionState.h BaseApplication.h Paddle.h Score.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BallGame.cpp Physics.cpp GameObject.cpp Wall.cpp Ball.cpp OgreMotionState.cpp BaseApplication.cpp Paddle.cpp Score.cpp

OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(SDL2_CFLAGS) $(SDL_MIXER_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL2_LIBS) $(SDL_MIXER_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -lSDL -lSDL_mixer

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
