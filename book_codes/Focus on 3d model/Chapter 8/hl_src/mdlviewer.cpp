/***
 *
 *	Copyright (c) 1998, Valve LLC. All rights reserved.
 *
 *  Last modified 11/26/2001, Michael Haller, haller@fhs-hagenberg.ac.at
 *
 **/

#include <stdio.h>
#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

#include "mathlib.h"
#include "studio.h"
#include "mdlviewer.h"


#pragma warning( disable : 4244 ) // conversion from 'double ' to 'float ', possible loss of data
#pragma warning( disable : 4305 ) // truncation from 'const double ' to 'float '

vec3_t		g_vright;		// needs to be set to viewer's right in order for chrome to work
float		g_lambert = 1.5;

float		gldepthmin = 0;
float		gldepthmax = 10.0;




//////////////////////////////////////////////////////////////////


static int pstyle;
static int translate = 1;
static int mesh = 1;
static float transx = 0, transy = 0, transz = -2, rotx=235, roty=-90;
static float scale = 0.01;static float amplitude = 0.03;
static float freq = 5.0f;
static float phase = .00003;
static int ox = -1, oy = -1;
static int show_t = 1;
static int mot;
#define PAN	1
#define ROT	2
#define ZOOM 3


