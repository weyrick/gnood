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

   a single gnood

*/

#ifndef GNOOD_BRICK
#define GNOOD_BRICK
      
#include <ClanLib/core.h>
#include <stdio.h>
#include <vector>
#include "utils.h"

#define MAX_BRICK_TYPES	10
	
#define BRICK_STATE_ON_SHOOTER	0
#define BRICK_STATE_FIRED		1
#define BRICK_STATE_IN_CELL		2
#define BRICK_STATE_DEAD		3

extern CL_ResourceManager * resources;

class brickManagerClass;

// a singal brick 
class gnoodBrick {

public:
	// initialize a new brick
	gnoodBrick(const coords& sLoc, const coords& cLoc, unsigned int bNum, int bState, brickManagerClass * bManager) :
        screenLoc(sLoc),
        cellLoc(cLoc),
		brickType(bNum),
		animPos(0),
		brickState(bState),
        animating(false),
		brickManager(bManager)
	{

	}

	// blit brick to screen
	void showBrick();

	// set location of brick in screen coords
	void setScreenLoc(coords c) { screenLoc = c; }

	// get location of brick in screen coords
	const coords& getScreenLoc() { return screenLoc; }

    // get the screen location of collision points for this brick. store them in passed vector
    void getCollisionPoints(vector<coords>& cPoints);

	// set slope vector of brick
	void setSlope(coords c) { slope = c; }

	// get slope vector of brick
	const coords& getSlope() { return slope; }

	// set cell location in cell coords. save last one if it's changed
	void setCellLoc(coords c) { cellLoc = c; }

	// get cell location in cell coords
	const coords& getCellLoc() { return cellLoc; }

	// bounce the brick to the right (after hitting left wall)
	void bounceLeft() { slope.x = abs(slope.x); }

	// bounce the brick to the left (after hitting right wall)
	void bounceRight() { slope.x = -slope.x; }

	// return brick state
	int getBrickState() { return brickState; }

	// set brick state
	void setBrickState(int newState) { brickState = newState; }

	// move this brick according to our state and slope vector
	void moveBrick() {

		if ((brickState == BRICK_STATE_IN_CELL)||
			(brickState == BRICK_STATE_ON_SHOOTER))
			return;

		screenLoc.x += slope.x;
		screenLoc.y += slope.y;

	}

    // return coordinates of screen location we'll next be at if we
    // move based on our current slope
    coords nextLoc() {
        return coords(screenLoc.x + slope.x, screenLoc.y + slope.y);
    }

	// return the brick number
	unsigned int getBrickType() { return brickType; }

	// set brick surface image
	void setBrickType(unsigned int bNum) { brickType = bNum; }

private:

	// location on the screen (x,y)
	coords screenLoc;

	// slope for movement
	coords slope;

	// current cell location we're either stationary at, or flying over
	// -1,-1 if we're not either
	coords cellLoc;

	// brick tile we're using
	unsigned int brickType;

	// animation position we are in
	unsigned int animPos;

	// motion state
	int brickState;

    // animation state
    bool animating;

	// the brick manager
	brickManagerClass * brickManager;

};      
      

// the brick manager should be used to create new bricks. it maintains the list of total bricks
class brickManagerClass {

public:
	brickManagerClass();
	~brickManagerClass();

	// create a new brick at location x,y of brick type brickype
	// if brickType is left off or -1, it will be random from all brick types
	gnoodBrick * newBrick(const coords& screenLoc, const coords& cellLoc, int brickState, int brickType=-1);


	// create a new brick at location x,y of brick type brickype
	// this version has a vector list of types to choose from
	gnoodBrick * newBrick(const coords& screenLoc, const coords& cellLoc, int brickState, const vector<int>& typeList);

	// return a brick surface based
	CL_Surface * getBrickSurface(unsigned int bType) {
		if (bType >= numBrickTypes)
			bType = 0;
		return brickTypes[bType];
	}

	// retrieve the number of brick types we maintain
	int getNumBrickTypes() { return numBrickTypes; }

    // return brick dimensions
    const dimensions& getBrickSize() { return brickSize; }

private:

	// array of surfaces, one for each brick type
	CL_Surface * brickTypes[MAX_BRICK_TYPES];

	// number of brick types
	unsigned int numBrickTypes;

	// dimensions of our bricks
	dimensions brickSize;

};

#endif

