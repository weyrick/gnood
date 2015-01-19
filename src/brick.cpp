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

   gnood brick / brick manager stuff

*/
#include "brick.h"


// blit brick to screen
void gnoodBrick::showBrick() {

	// DEBUG


	/*
	// screen location points
	coords collPoint1, collPoint2, collPoint3;
	// slope cache
	bool goingLeft, goingRight, goingUp;

	// setup collision points based on bricks current location
	if (slope.x < 0) {

		// it's traveling left
		goingLeft = true;
		collPoint1 = screenLoc;
		collPoint2.x = collPoint1.x + (brickManager->getBrickSize().w / 3);
		collPoint2.y = collPoint1.y;
		collPoint3.x = collPoint1.x;
		collPoint3.y = collPoint1.y + (brickManager->getBrickSize().h / 3);

	}
	else if (slope.x > 0) {

		// it's travelling right
		goingRight = true;
		collPoint1 = screenLoc;
		collPoint1.x = (collPoint1.x + brickManager->getBrickSize().w);
		collPoint2.x = collPoint1.x - (brickManager->getBrickSize().w / 3);
		collPoint2.y = collPoint1.y;
		collPoint3.x = collPoint1.x;
		collPoint3.y = collPoint1.y + (brickManager->getBrickSize().h / 3);

	}
	else {

		// it's travelling strait up
		goingUp = true;
		collPoint1 = screenLoc;
		collPoint1.x = collPoint1.x + (brickManager->getBrickSize().w/2);
		collPoint2 = collPoint1;
		collPoint3 = collPoint1;

	}

	CL_Display::fill_rect(collPoint1.x, collPoint1.y, collPoint1.x+1, collPoint1.y+1, 1, 1, 1, 1);
	CL_Display::fill_rect(collPoint2.x, collPoint2.y, collPoint2.x+1, collPoint2.y+1, 1, 1, 1, 1);
	CL_Display::fill_rect(collPoint3.x, collPoint3.y, collPoint3.x+1, collPoint3.y+1, 1, 1, 1, 1);
	*/
	  
	  
	// blit onto the screen
	brickManager->getBrickSurface(brickType)->put_screen(screenLoc.x, screenLoc.y, animPos);

	// are we currently animating this brick?
	if (animating) {
		animPos++;
	}
	else {
		// should we start animating?
		if (random() % 20 == 1)
			animating = true;
	}

	// wrap to begining of sprite loop if we've hit the end
	if (animPos >= brickManager->getBrickSurface(brickType)->get_num_frames()) {
		animPos = 0;
		animating = false;
	}

}


// -----------------------------------------------------------------

brickManagerClass::brickManagerClass() {


     // load the brick height and width size we'll use
     brickSize.h = CL_Res_Integer::load("Game/Config/PlayBoard/brickHeight", resources);
     brickSize.w = CL_Res_Integer::load("Game/Config/PlayBoard/brickWidth", resources);

     // how many brick types do we have?
     numBrickTypes = CL_Res_Integer::load("Game/Config/PlayBoard/numBrickTypes", resources);

     if (numBrickTypes > MAX_BRICK_TYPES) {
         cout << "too many brick types specified!\n";
         exit(-1);
     }

     // load in different brick types
     char bName[100];
     for (unsigned int b=0; b < numBrickTypes; b++) {
 
         snprintf(bName, 100, "Game/Config/PlayBoard/Bricks/sur_brick_%d", b);
         brickTypes[b] = CL_Surface::load(bName, resources);
 
     }

 }



// free resources
brickManagerClass::~brickManagerClass() {

	
	// load in different brick types
	for (unsigned int b=0; b < numBrickTypes; b++) {

		delete brickTypes[b];

	}

}


// create a new brick at location x,y of brick type brickype
// if brickType is left off or -1, it will be random from all brick types
gnoodBrick * brickManagerClass::newBrick(const coords& screenLoc, const coords& cellLoc, int brickState, int brickType=-1) {

	// random brick type
	if ((brickType < 0) || ((unsigned)brickType >= numBrickTypes)) {
		brickType = random() % numBrickTypes;
	}

	return new gnoodBrick(screenLoc, cellLoc, brickType, brickState, this);

}


// create a new brick at location x,y of brick type brickype
// this version has a vector list of types to choose from
gnoodBrick * brickManagerClass::newBrick(const coords& screenLoc, const coords& cellLoc, int brickState, const vector<int>& typeList) {

	// random brick type
	gnoodBrick * newBrick;
	int brickType;
	if (typeList.size() > 0) {
		brickType = random() % (int)typeList.size();
		newBrick = new gnoodBrick(screenLoc, cellLoc, typeList[brickType], brickState, this);
	}
	else {
		brickType = random() % numBrickTypes;
		newBrick = new gnoodBrick(screenLoc, cellLoc, brickType, brickState, this);
	}

	return newBrick;

}

