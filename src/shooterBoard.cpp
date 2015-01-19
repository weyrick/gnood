/*
													 
	Gnood

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

	
	Copyright (c) 2000 Shannon Weyrick
	weyrick@roadsend.com


*/
/*

  shooter class

*/

#include "shooterBoard.h"
#include <math.h>

// constructor
shooterBoardClass::shooterBoardClass(playBoardClass * b, brickFlightManagerClass * fm) :
	playBoard(b),
	flightManager(fm)
{

	// set location of shooter board on screen
	baseLoc.x = CL_Res_Integer::load("Game/Config/ShooterBoard/baseX", resources, 0);
	baseLoc.y = CL_Res_Integer::load("Game/Config/ShooterBoard/baseY", resources);

	// set width/height of shooter base
	baseSize.w = CL_Res_Integer::load("Game/Config/ShooterBoard/shooterBaseW", resources);
	baseSize.h = CL_Res_Integer::load("Game/Config/ShooterBoard/shooterBaseH", resources);

    // if baseX wasn't given, center it based on play area. add in brickSize becuase of brick pattern
    if (baseLoc.x == 0)
        baseLoc.x = (playBoard->getBoardSize().w / 2) - (baseSize.w / 2) + playBoard->getStartLoc().x - (brickManager->getBrickSize().w/2);

	// centerdistance
	centerDistance = CL_Res_Integer::load("Game/Config/ShooterBoard/centerDistance", resources);

	// default brick speed scaler
	brickSpeed = CL_Res_Integer::load("Game/Config/ShooterBoard/brickSpeed", resources, DEFAULT_BRICK_SPEED);

	// angles to allow shooter to fire from
	minMaxAngle = CL_Res_Integer::load("Game/Config/ShooterBoard/minMaxAngle", resources);

	// FIXME: this is a quick hack
	if ( (minMaxAngle < 5) || (minMaxAngle > ABSOLUTE_MINMAX_ANGLE) ) {
		cout << "minMaxAngle invalid!!\n";
		exit(-1);
	}

	// precompute the sin/cos tables
	double rads=0;
	int ag = -minMaxAngle;
	for (int i=0; i <= minMaxAngle*2; i++, ag++) {
		rads = ag*(M_PI/180);
		sinTable[i] = sin(rads);
		cosTable[i] = cos(rads);
	}

	// shooter base surface
	sur_shooterBase = CL_Surface::load("Game/Config/ShooterBoard/sur_shooterBase", resources);

	// setup the shooter base offset for calculations
	baseOriginLoc.x = (baseLoc.x + (baseSize.w / 2));
	baseOriginLoc.y = (baseLoc.y + (baseSize.h / 2));

	// setup the brick offset for same reason
	brickOriginOffsetLoc.x = brickManager->getBrickSize().w / 2;
	brickOriginOffsetLoc.y = brickManager->getBrickSize().h / 2;

	// setup angel to strait up
	fireBrick = 0;
	setShooterAngle(0);

}              

// free resources
shooterBoardClass::~shooterBoardClass() {

	delete sur_shooterBase;

}

// setup shooter angle
void shooterBoardClass::setShooterAngle(int a) {

	// is angle within range?
	if ((a < -minMaxAngle)||(a > minMaxAngle))
		return;

	// set new angle
	angle = a;

	// calculate new x,y for firebrick based on distance from origin of shooter base at current angle
	int angleIdx = angle+minMaxAngle;
	double x = ((baseOriginLoc.x-(sinTable[angleIdx]*centerDistance)))-brickOriginOffsetLoc.x;
	double y = ((baseOriginLoc.y-(cosTable[angleIdx]*centerDistance)))-brickOriginOffsetLoc.y;

	// FIXME: new mechanizm for loaded bricks...
	if (!fireBrick)
		fireBrick = brickManager->newBrick(coords(x,y),coords(-1,-1), BRICK_STATE_ON_SHOOTER, playBoard->getCurrentBrickTypes());
	else
		fireBrick->setScreenLoc(coords(x, y));

}

// fire the brick being aimed
void shooterBoardClass::launchBrick() {

	// if we have no brick to fire, don't let them do this
	if (!fireBrick)
		return;

	// set slope on brick

	// slope will be calculated from center of base to center of fireBrick at current angle
	// slope is y/x
	int slopeX = (fireBrick->getScreenLoc().x + brickOriginOffsetLoc.x) - baseOriginLoc.x;
	int slopeY = (fireBrick->getScreenLoc().y + brickOriginOffsetLoc.y) - baseOriginLoc.y;

	fireBrick->setSlope(coords(slopeX/brickSpeed, slopeY/brickSpeed));

	if (flightManager->acceptNewBrick(fireBrick)) {
		// successfully fired
		// FIXME: load new brick!!
		fireBrick = 0;
		setShooterAngle(angle);
	}
	

}

// time slice
void shooterBoardClass::timeSlice() 
{

	// show the base
	sur_shooterBase->put_screen(baseLoc.x, baseLoc.y);

	// show the firing brick for aiming
	if (fireBrick)
		fireBrick->showBrick();

}

