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

#ifndef GNOOD_SHOOTER
#define GNOOD_SHOOTER

#include "playBoard.h"
#include "brick.h"
#include "brickFlight.h"

// absolute min/max angle we'll allow the shooter to shoot at							 
#define ABSOLUTE_MINMAX_ANGLE  80

// default brick speed scaling factor
#define DEFAULT_BRICK_SPEED		5
	
extern brickManagerClass * brickManager;
	
class shooterBoardClass {

public:
	// constructor
    shooterBoardClass(playBoardClass * b, brickFlightManagerClass * fm);


	// destructor
    ~shooterBoardClass();

	// set the shooter to a certain angle
	void setShooterAngle(int a);

	// increase shooter angle
	void increaseShooterAngle() {
		setShooterAngle(angle+1);
	}
	void increaseShooterAngle(unsigned int mult) {
		setShooterAngle(angle+(mult));
	}

	// decrease shooter angle
	void decreaseShooterAngle() {
		setShooterAngle(angle-1);
	}
	void decreaseShooterAngle(unsigned int mult) {
		setShooterAngle(angle-(mult));
	}

	// time slice
	void timeSlice();

	// fire brick
	void launchBrick();

	// increase brick speed
	void increaseBrickSpeed(int mult=1) {
		brickSpeed-=mult;
		if (brickSpeed < 1)
			brickSpeed = 1;
	}

	// decrease brick speed
	void decreaseBrickSpeed(int mult=1) {
		brickSpeed+=mult;
	}

private:

	// start coodinates of the shooter base
	coords baseLoc;

	// shooter base height and width
	dimensions baseSize;

	// distance from center of shooterBase the brick will be launched from
	int centerDistance;

	// angle of shooter
	int angle;
	int minMaxAngle;

	// offsets for calcs
	coords baseOriginLoc;
	coords brickOriginOffsetLoc;

	// speed of brick
	int brickSpeed;

	// precompute sin/cos values
	double sinTable[ABSOLUTE_MINMAX_ANGLE*2];
	double cosTable[ABSOLUTE_MINMAX_ANGLE*2];

	// the play board
	playBoardClass * playBoard;

	// flight manager
	brickFlightManagerClass * flightManager;

	// the surface image of the shooter base
	CL_Surface * sur_shooterBase;

	// the current brick to fire
	gnoodBrick * fireBrick;

};

#endif        
