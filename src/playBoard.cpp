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

  playboard class

*/

#include "playBoard.h"
#include "stdio.h"
#include <vector>

// display a box where this cell is located
void playCell::showGrid(int showX, int showY) {

	CL_String sx(showX);
	CL_String sy(showY);
	CL_String dis = sx + "," + sy;

	CL_Display::fill_rect(screenLoc.x, 
						  screenLoc.y, 
						  screenLoc.x + cellSize.h-1, 
						  screenLoc.y+ + cellSize.w-1,
						  .3,.6,.3,.3);

	mainFont->print_center(screenLoc.x + (cellSize.w/2), 
						   screenLoc.y + (cellSize.h/2), 
						   dis.get_string() );

}

// display our assigned brick, if we currently have one
void playCell::showCell() {

	if (!brick)
		return;

	// blit onto the screen
	brick->showBrick();

}

// set the brick located in this play cell
void playCell::setBrick(gnoodBrick * newBrick) { 

	// if newBrick is null, oopsie!
	if (!newBrick) {
		cout << "woopsie! newBrick was NULL for playCell::setBrick!\n";
		return;
	}

    // if we already have a brick here, get rid of it
    if (brick)
        delete brick;

	brick = newBrick; 
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// constructor
cellManagerClass::cellManagerClass(int _startX, int _startY, int w, int h, brickFallManagerClass * fm) :
	showGrid(false),
	showBricks(true),
	showCLalpha(false),
	fallManager(fm),
	numBricksInCells(0)
{

	// start location
	startLoc.x = _startX;
	startLoc.y = _startY;

	// cell area in pixels
	cellAreaPixelSize.w = w;
	cellAreaPixelSize.h = h;

	// figure out how many cells we can fit in our play area
	cellAreaCellSize.w = (cellAreaPixelSize.w / brickManager->getBrickSize().w);
	cellAreaCellSize.h = (cellAreaPixelSize.h / brickManager->getBrickSize().h);

    // clear play cell array
    for (int cx = 0; cx < MAXCELLS_W; cx++)
        for (int cy = 0; cy < MAXCELLS_H; cy++)
            cellList[cx][cy] = 0;

	// setup a playCell for each cell we'll have
	for (int cY = 0; cY < cellAreaCellSize.h; cY++) {
		for (int cX = 0; cX < cellAreaCellSize.w; cX++) {

            // create a new play cell for this cell location
			cellList[cX][cY] = new playCell(calcScreenFromCell(coords(cX,cY)), brickManager->getBrickSize()); 

			// assign random brick type, if this isn't the last row (which we leave open)
			if (cY != cellAreaCellSize.h-1) {
				cellList[cX][cY]->setBrick(brickManager->newBrick(cellList[cX][cY]->getScreenLoc(), coords(cX,cY), BRICK_STATE_IN_CELL));
				numBricksInCells++;
			}

		}
	}

}

// timeslice, update/display cells
void cellManagerClass::timeSlice() {


	// loop through all cells in the play area
	for (int cX = 0; cX < cellAreaCellSize.w; cX++) {

		for (int cY = 0; cY < cellAreaCellSize.h; cY++) { 

			// if we're showing bricks, do that here
			if (showBricks)
				cellList[cX][cY]->showCell();

			// if the grid is on, show the grid
			if (showGrid)
				cellList[cX][cY]->showGrid(cX, cY);


		}

	} 

	// if we're showing board layout alpha, do it here
	if (showCLalpha) {


		CL_Display::fill_rect(startLoc.x, 
							  startLoc.y, 
							  startLoc.x + cellAreaPixelSize.w, 
							  startLoc.y + cellAreaPixelSize.h,
							  .3,.3,.6,.3);

	}

	// update scoreboard data
	scoreBoard->setBrickCount(numBricksInCells);

}

// calculate screen coordinates from cell coordinates. take into account 
// startLocation and brick pattern
const coords cellManagerClass::calcScreenFromCell(const coords cellLoc) {

    coords loc;

    // base location is a factor of (cell location * brick size) + start location offset
    loc.x = (cellLoc.x * brickManager->getBrickSize().w) + startLoc.x;
    loc.y = (cellLoc.y * brickManager->getBrickSize().h) + startLoc.y;

    // if this is an odd row, push brick x over by brickSize.w/2
    if (cellLoc.y % 2 != 0 )
        loc.x += (brickManager->getBrickSize().w/2);

    return loc;

}


// calculate cell location from screen coords
const coords cellManagerClass::calcCellFromScreen(const coords screenLoc) {

    // base our hotspot on the center of the brick
    coords loc;

    // if the brick is in our play area...
    if ((screenLoc.x >= startLoc.x) &&
        (screenLoc.x <= startLoc.x + cellAreaPixelSize.w) &&
        (screenLoc.y >= startLoc.y) &&
        (screenLoc.y <= startLoc.y + cellAreaPixelSize.h))
    {

        loc.y = ((screenLoc.y - startLoc.y) / brickManager->getBrickSize().h);
        
		// if it's an odd row, figure in brick pattern offset
        int offset = 0;
        if (loc.y % 2 != 0)
            offset = brickManager->getBrickSize().w / 2;
        loc.x = ((screenLoc.x - offset - startLoc.x) / brickManager->getBrickSize().w);

        // make sure this is a valid play cell
        if (!cellList[loc.x][loc.y]) {
            loc.x = -1;
            loc.y = -1;
        }

    }
    else {
        // outside cell pixel area
        loc.x = -1;
        loc.y = -1;
    }

    return loc;

}


// assign a brick to a particular cell
void cellManagerClass::assignBrick(const coords cellLoc, gnoodBrick * brick) {

    // if cell loc is -1,-1 we've got a problem
    if ((cellLoc.x == -1)||(cellLoc.y == -1)) {
        cout << "woops! tried assigning a brick to cell -1,-1!\n";
        exit(-1);
    }

	// make sure brick is alive
	if (!brick) {
		cout << "woops! tried assigning a NULL brick!\n";
		exit(-1);
	}

    // set it's new cell position
    brick->setCellLoc(cellLoc);

    // set it's new screen location
    brick->setScreenLoc(calcScreenFromCell(cellLoc));

    // set our cell list to point to this brick
    cellList[cellLoc.x][cellLoc.y]->setBrick(brick);

	// add to count
	numBricksInCells++;

    // since it's in our cell list, it shouldn't be moving...
    brick->setBrickState(BRICK_STATE_IN_CELL);

}


// get a match list for a list of cells
void cellManagerClass::getMatchList(gnoodBrick * brick, vector<coords> * passList) {

	// there are 5 spots we need to check, and they change based on whether this is an odd row or not:

	// picture the brick as the "o" in the middle, we check the ones around the outside in the following way...
	//   A  B
	// F  o  C
	//  E  D

	// odd row
	// A  0,-1
	// B  1,-1
	// C  1, 0
	// D  1, 1
	// E  0, 1
	// F -1, 0

	// even row
	// A -1,-1
	// B  0,-1
	// C  1, 0
	// D  0, 1
	// E -1, 1
	// F -1, 0

    vector<coords> checkList;
	if (brick->getCellLoc().y % 2 == 0) {
		// even row
		checkList.push_back(coords(-1,-1));
		checkList.push_back(coords( 0,-1));
		checkList.push_back(coords( 1, 0));
		checkList.push_back(coords( 0, 1));
		checkList.push_back(coords(-1, 1));
		checkList.push_back(coords(-1, 0));
	}
	else {
		// odd row
		checkList.push_back(coords( 0,-1));
		checkList.push_back(coords( 1,-1));
		checkList.push_back(coords( 1, 0));
		checkList.push_back(coords( 1, 1));
		checkList.push_back(coords( 0, 1));
		checkList.push_back(coords(-1, 0));
	}

	// our local match list
	vector<coords> matchList;

	// convienience...
    coords loc = brick->getCellLoc();    

	// get a match list
	int x,y;
    for (int i = 0; i < 6; i++) {

		x = loc.x + checkList[i].x;		
		y = loc.y + checkList[i].y;

		if ( (x < 0) || (y < 0) )
			continue;

		if ( (cellList[x][y] != NULL) &&	// this cell is defined and...
			 (cellList[x][y]->isOccupied()) &&  // this cell is occupied by a brick and...
			 (cellList[x][y]->getBrickType() == (signed)brick->getBrickType())  // the brick types match...
		    ) {
			
			// if it's not already in our list, add it
			if (find(passList->begin(), passList->end(), coords(x,y)) == passList->end()) {
				matchList.push_back(coords(x,y));
				passList->push_back(coords(x,y));
			}

		}


    }

	// so we have our local match list. now for each list, get call getMatchList again into another array
	for (vector<coords>::const_iterator pos=matchList.begin(); 
		 pos != matchList.end();
		 ++pos) {

		// get match list for each of our local matches
		getMatchList(cellList[(*pos).x][(*pos).y]->getBrick(), passList);

	}

}


// trace a path from a brick back to the ceiling
// function will return TRUE if it can trace a path to either another brick
// or the ceiling, and call itself recursively for surrounding bricks
bool cellManagerClass::tracePath(coords startCoords) {

    // nab these
    int x = startCoords.x;
    int y = startCoords.y;

    // if we're at the ceiling and there's a brick here, return true
    if ((y == 0)&&(x >= 0)&&(cellList[x][y] != NULL)&&(cellList[x][y]->isOccupied()))
        return true;

    // if there's no brick here, return false
    if ((cellList[x][y] == NULL)||(!cellList[x][y]->isOccupied()))
        return false;

    // see if we've cached this location yet...
    for (vector<cellState*>::const_iterator pos = hangerCache.begin();
         pos != hangerCache.end();
         ++pos) {

        if (startCoords == (*pos)->loc)
            return (*pos)->isValid;

    }

    // .. otherwise, check surrounding cells.
	// there are 5 spots we need to check, and they change based on whether this is an odd row or not:

    vector<coords> checkList;
	if (y % 2 == 0) {
		// even row
		checkList.push_back(coords(-1,-1));
		checkList.push_back(coords( 0,-1));
		checkList.push_back(coords( 1, 0));
		checkList.push_back(coords( 0, 1));
		checkList.push_back(coords(-1, 1));
		checkList.push_back(coords(-1, 0));
	}
	else {
		// odd row
		checkList.push_back(coords( 0,-1));
		checkList.push_back(coords( 1,-1));
		checkList.push_back(coords( 1, 0));
		checkList.push_back(coords( 1, 1));
		checkList.push_back(coords( 0, 1));
		checkList.push_back(coords(-1, 0));
	}

	// get a match list
    for (int i = 0; i < 6; i++) {

		x = startCoords.x + checkList[i].x;		
		y = startCoords.y + checkList[i].y;

		// if this location is locked, don't do it	(a recursion before us already is)

		bool locked = false;
		for (vector<coords*>::const_iterator pos2 = hangerLock.begin();
			 pos2 != hangerLock.end();
			 ++pos2) {

			if (coords(x,y) == *(*pos2)) {
				locked = true;
			}

		}
		if (locked)
			continue;

		// lock this location
		hangerLock.push_back(new coords(x,y));

		if ((x < 0)||(y < 0)) {
			// unlock location
			delete hangerLock.back();
			hangerLock.pop_back();
			continue;
		}

        // do we have this local cached?
		bool cached = false;
		bool pathOk;
		for (vector<cellState*>::const_iterator pos = hangerCache.begin();
			 pos != hangerCache.end();
			 ++pos) {
	
			if (startCoords == (*pos)->loc) {
				// yep!! 
				cached = true;
				pathOk = (*pos)->isValid;
			}
	
		}

		if (!cached) {
			pathOk = tracePath(coords(x,y));
			hangerCache.push_back(new cellState(coords(x,y),pathOk));
		}

        // if it was true, then we've found a link and can stop here
        if (pathOk) {
			// unlock location
			delete hangerLock.back();
			hangerLock.pop_back();
            return true;
		}
        
        // otherwise, we have to keep looking...

		// unlock location
		delete hangerLock.back();
		hangerLock.pop_back();


    }

    // fall through -- no valid brick path next to us!
    return false;

}


// clear hanging bricks
// also collect information of which types of bricks are available for shooter
void cellManagerClass::clearHangers() {


	// check the validity of every cell that has a brick. we must be able
    // to trace a path back to ceiling through touching bricks

    // clear the coord cache before checking this cell
    for (vector<cellState*>::const_iterator pos = hangerCache.begin();
         pos != hangerCache.end();
         ++pos) {
		delete (*pos);
	}
    hangerCache.clear();
	
	// 
	brickTypes.clear();

	// loop through all cells in the play area
	for (int cX = cellAreaCellSize.w-1; cX > -1; cX--) {

		for (int cY = cellAreaCellSize.h-1; cY > -1; cY--) { 

            if ((cellList[cX][cY] != NULL) && (cellList[cX][cY]->isOccupied()) && (!tracePath(coords(cX,cY)))) {
                // couldn't trace a valid path, drop brick
				fallManager->acceptNewBrick(cellList[cX][cY]->getBrick());
                cellList[cX][cY]->clearBrick();
				numBricksInCells--;

				// FIXME: score per brick cleared on hanger
				scoreBoard->addScore(200);
            }

			// collect brick type info		
			// add in this type if it's not already there
			if (find(brickTypes.begin(), brickTypes.end(), cellList[cX][cY]->getBrickType()) == brickTypes.end()) {
				brickTypes.push_back(cellList[cX][cY]->getBrickType());
			}

		}

	}

	return;

}

// zap local bricks and clear hangers
// return true if we cleared the main brick as well
void cellManagerClass::recurseZap(gnoodBrick * brick) {

    if (!brick) {
        cout << "woops! tried to zap a null brick\n";
        exit(-1);
    }

    coords loc = brick->getCellLoc();    

    if ((loc.x == -1)||(loc.y == -1)) {
        cout << "woops! tried to zap a -1,-1 cell location\n";
        exit(-1);
    }

    if (!cellList[loc.x][loc.y]) {
        cout << "woops! cell location of zap was null!\n";
        exit(-1);
    }

	// this list will contain a list of coordinates of adjacent cells with our brick type
	vector<coords> * matchList = new vector<coords>;
	getMatchList(brick, matchList);

	// there has to be n bricks together to clear them!!
	// FIXME make this a variable at runtime!!
	if (matchList->size() < 3)
		return;

	// FIXME: mult score for each in clump
	scoreBoard->multScore(matchList->size());

	// clear all cells that matched the match list
	for (vector<coords>::const_iterator pos=matchList->begin(); 

		 pos != matchList->end();
		 ++pos) {

		int x = (*pos).x;
		int y = (*pos).y;

		// take brick out of play board
		fallManager->acceptNewBrick(cellList[x][y]->getBrick());
		cellList[x][y]->clearBrick();
		numBricksInCells--;

		// FIXME: score per brick in match list
		scoreBoard->addScore(100);

	}

	// now clear "hangers"
	clearHangers();

	// check for win of game
	// FIXME
	if (numBricksInCells == 0) {
		cout << "YOU WIN!!!\n";
		exit(-1);
	}

}

// do boundry checking on a brick and move within play area
void cellManagerClass::moveInPlayArea(gnoodBrick * brick) {

	// collision detection points
	// points 1,2 and 3 will be at:

	// slope < 0
	// 1: x, y
	// 2: x+(w/3), y
	// 3: x, y+(h/3)

	// slope > 0
	// 1: (x+w), y
	// 2: (x+w)-(w/3), y
	// 3: (x+w), y+(h/3)

	// slope == 0
	// 1: x+(w/2), y
	// 1: x+(w/2), y
	// 1: x+(w/2), y

	// cellPoint1 and collPoint1 is the location of the direction we're going, and we'll use it to decide
	// things like whether we're in the cell area and stuff
	// 2 and 3 we just use for some more collision checking. 

	// screen location points
	coords collPoint1, collPoint2, collPoint3;
	// cell location of above points
	coords cellPoint1, cellPoint2, cellPoint3;
	// slope cache
	bool goingLeft, goingRight, goingUp;

	// setup collision points based on bricks current location
	if (brick->getSlope().x < 0) {

		// it's traveling left
		goingLeft = true;
		collPoint1 = brick->getScreenLoc();
		collPoint2.x = collPoint1.x + (brickManager->getBrickSize().w / 3);
		collPoint2.y = collPoint1.y;
		collPoint3.x = collPoint1.x;
		collPoint3.y = collPoint1.y + (brickManager->getBrickSize().h / 3);

	}
	else if (brick->getSlope().x > 0) {

		// it's travelling right
		goingRight = true;
		collPoint1 = brick->getScreenLoc();
		collPoint1.x = (collPoint1.x + brickManager->getBrickSize().w);
		collPoint2.x = collPoint1.x - (brickManager->getBrickSize().w / 3);
		collPoint2.y = collPoint1.y;
		collPoint3.x = collPoint1.x;
		collPoint3.y = collPoint1.y + (brickManager->getBrickSize().h / 3);

	}
	else {

		// it's travelling strait up
		goingUp = true;
		collPoint1 = brick->getScreenLoc();
		collPoint1.x = collPoint1.x + (brickManager->getBrickSize().w/2);
		collPoint2 = collPoint1;
		collPoint3 = collPoint1;

	}

	// setup cell points based on above screen points
	cellPoint1 = calcCellFromScreen(collPoint1);
	cellPoint2 = calcCellFromScreen(collPoint2);
	cellPoint3 = calcCellFromScreen(collPoint3);

	// if we're not in the playcell area, advance brick and return
	if ((cellPoint1.x == -1) || (cellPoint1.y == -1)) {
		brick->moveBrick();
		return;
	}

	// if y of brick has hit the top of the play board, stop it there
	if (collPoint1.y <= startLoc.y) {
		assignBrick(brick->getCellLoc(), brick);
		return;
	}

	// if x of brick is <= play area's left border, bounce it
	if ((goingLeft)&&(collPoint1.x <= startLoc.x)) {
		brick->bounceLeft();
		return;
	}

	// if x+w of brick is >= right border, bounce it
	if ((goingRight)&&(collPoint2.x >= (startLoc.x + cellAreaPixelSize.w))) {
		brick->bounceRight();
		return;
	}

	// now check the three points for collision with an occupied cell
	// check the main one first

	// if the cell we are next going into is occupied, plant ourselves in the current cell
	if (
		
		(cellList[cellPoint1.x][cellPoint1.y]->isOccupied()) ||

		((cellPoint2.x != -1)&&(cellPoint2.y != -1)&&
		(cellList[cellPoint2.x][cellPoint2.y])&&
		(cellList[cellPoint2.x][cellPoint2.y]->isOccupied())) ||

		((cellPoint3.x != -1)&&(cellPoint3.y != -1)&&
		(cellList[cellPoint3.x][cellPoint3.y])&&
		(cellList[cellPoint3.x][cellPoint3.y]->isOccupied()))

		)

	{

		// if the current cell location is -1,-1 the game is OVER because they've let
		// the bricks come down too far!
		if ((brick->getCellLoc().x == -1) ||
			(brick->getCellLoc().y == -1)) {

			// FIXME: GAME OVER
			cout << "Game Over!!\n";
			exit(1);

		}


		// assign this brick to it's current cell location, zap all related bricks
		assignBrick(brick->getCellLoc(), brick);

		// if we're zapping related bricks, do so here
		recurseZap(brick);

		// done
		return;
	}

    // if we get here, we're flying through the playboard, without touching walls,
    // through open cells with no bricks in them. update which cell we're flying over and
    // advance brick
	brick->setCellLoc(cellPoint1);
    brick->moveBrick();

	return;

}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// constructor
playBoardClass::playBoardClass(brickFallManagerClass * fm) :
	fallManager(fm),
	showBG(true),
	showBLalpha(false)
	{

	// load in the background tile
	sur_bgTile 	= CL_Surface::load("Game/Config/PlayBoard/sur_bgTile", resources);
	bgTileSize.h = sur_bgTile->get_height(); 
	bgTileSize.w = sur_bgTile->get_width();

	// load in various settings
	boardSize.h = CL_Res_Integer::load("Game/Config/PlayBoard/height", resources);
	boardSize.w = CL_Res_Integer::load("Game/Config/PlayBoard/width", resources);

	// load the position the playfield should start at
	startLoc.x = CL_Res_Integer::load("Game/Config/PlayBoard/startX", resources); 
	startLoc.y = CL_Res_Integer::load("Game/Config/PlayBoard/startY", resources);
	
	// create the cell manager
	cellManager = new cellManagerClass(startLoc.x, startLoc.y, boardSize.w, boardSize.h, fallManager);

}

// free resources
playBoardClass::~playBoardClass() {

	delete sur_bgTile;

}

// timeslice, do updates
void playBoardClass::timeSlice() { 
	

	// we're getting a time slice, update this frame
	int x,y;

	// start by laying down the background
	if (showBG) {

		for (x = 0; x <= boardSize.w + bgTileSize.w; x += bgTileSize.w) {
			for (y = 0; y <= boardSize.h + bgTileSize.h; y += bgTileSize.h) {
				sur_bgTile->put_screen(x,y);
			}
		}

	}

	// if we're showing board layout alpha, do it here
	if (showBLalpha) {


		CL_Display::fill_rect(startLoc.x, 
							  startLoc.y, 
							  startLoc.x + boardSize.w, 
							  startLoc.y + boardSize.h,
							  .6,.3,.3,.3);

	}

	// give slice to our cell manager
	cellManager->timeSlice(); 
		
}
