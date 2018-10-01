ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BallApplication.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BallApplication.cpp
OgreApp_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS)
OgreApp_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lpthread $(OIS_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
