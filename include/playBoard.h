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

#ifndef GNOOD_PLAYBOARD
#define GNOOD_PLAYBOARD

#include <iostream>
#include <ClanLib/core.h>
#include "config.h"
#include "scoreBoard.h"
#include "brick.h"
#include "brickFall.h"

#define MAXCELLS_H		50
#define MAXCELLS_W		50

using namespace std;				 
 
extern CL_Font * mainFont;
extern CL_ResourceManager * resources;
extern brickManagerClass * brickManager;
extern scoreBoardClass * scoreBoard;

// a cell in the playboard	
class playCell 
{

public:
	playCell(const coords& sLoc, const dimensions& cSize) :
		brick(NULL),
        screenLoc(sLoc),
        cellSize(cSize)
	{


	}

	// display a box where this cell is located
	void showGrid(int showX, int showY);

	// display our assigned brick, if we currently have one
	void showCell();

	// set the surface of this brick
	void setBrick(gnoodBrick * newBrick);

	// get the brick type of the brick in this cell, or -1 if no brick
	int getBrickType() {
		if (brick)
			return brick->getBrickType();
		else
			return -1;
	}

	// clear the brick
	void clearBrick() {
		if (brick)
			brick->setBrickState(BRICK_STATE_DEAD);
		brick = 0;
	}

	// get brick
	gnoodBrick * getBrick() {
		return brick;
	}

    // get the screen location of this cell
    const coords& getScreenLoc() { return screenLoc; }

    // return true if this cell is occupied by a brick
    bool isOccupied() { 
        if (brick)
            return true;
        else
            return false;
    }

private:	

	// our current brick, NULL if empty cell
	gnoodBrick * brick;

	// location on the playboard (pixels)
	coords screenLoc;

	// dimensions (pixels)
	dimensions cellSize;

};
				  

// state of a cell regarding hang status
class cellState {
public:
    cellState(coords l, bool iv) :
    loc(l),
    isValid(iv)
    {

    }
    coords loc;
    bool isValid;
};

// manages list of cells
class cellManagerClass {

public:
	// constructor
	cellManagerClass(int _startX, int _startY, int w, int h, brickFallManagerClass * fm);
	 
	// time slice
	void timeSlice();

	// set show grid
	void toggleGrid() { (showGrid) ? showGrid = false : showGrid = true; }

	// set show bricks
	void toggleBricks() { (showBricks) ? showBricks = false : showBricks = true; }

	// set show cell layout alpha
	void toggleCLalpha() { (showCLalpha) ? showCLalpha = false : showCLalpha = true; }

	// accept a gnoodBrick and do boundary checking on it 
	void moveInPlayArea(gnoodBrick * brick);

    // assign a brick to a particular cell
    void assignBrick(const coords cellLoc, gnoodBrick * brick);

    // calculate the x,y screen location of a particular cell location
    const coords calcScreenFromCell(const coords cellLoc);

    // calculate cell coords from screen x,y
    const coords calcCellFromScreen(const coords screenLoc);

    // recursively zap all bricks starting with this brick which are related to it
    void recurseZap(gnoodBrick * brick);

	// get a list of matching coordinates at a location
	void getMatchList(gnoodBrick * brick, vector<coords> * passList);

	// clear hanging bricks
	void clearHangers();

    // trace a path from a start location to valid bricks back to the ceiling
    bool tracePath(coords startCoords);

	// get list of brick types currently on the board
	const vector<int>& getCurrentBrickTypes(void) { return brickTypes; }

private:
    // our cell list
	playCell * cellList[MAXCELLS_W][MAXCELLS_H];

	// dimensions of cell area (pixels)
	dimensions cellAreaPixelSize;

	// how many cells we will be able to fit in our play space
	dimensions cellAreaCellSize;

	// where we start the cell area coords (pixels)
	coords startLoc;

	// should we draw a grid outlining cellspace?
	bool showGrid;

	// should we draw the bricks?
	bool showBricks;

	// show we show cell board layout alpha?
	bool showCLalpha;

    // vector of coords for doing some calculations
    vector<cellState*> hangerCache;
	vector<coords*> hangerLock;

	// fall manager, for falling bricks
	brickFallManagerClass * fallManager;

	// number of bricks left in cell area
	int numBricksInCells;

	// current list of brick types on the board
	vector<int> brickTypes;

};


class playBoardClass
{


public:
	playBoardClass(brickFallManagerClass * fm);

	// cleanup
	~playBoardClass();

    // return start location coords
    const coords& getStartLoc() { return startLoc; }

	// return board size
	const dimensions& getBoardSize() { return boardSize; }

	// timeslice, do updates
	void timeSlice();

	// set show grid
	void toggleGrid() { cellManager->toggleGrid(); }

	// set show bricks
	void toggleBricks() { cellManager->toggleBricks(); }

	// set show playboard layout alpha
	void togglePBalpha() { 
		(showBLalpha) ? showBLalpha = false : showBLalpha = true; 
	}

	// set show cell manager layout alpha
	void toggleCLalpha() { cellManager->toggleCLalpha(); }

	// set show bg
	void toggleBG() { (showBG) ? showBG = false : showBG = true; }

	// boundary check. pass on to cell manager
	void moveInPlayArea(gnoodBrick * brick) { cellManager->moveInPlayArea(brick); }

	// get list of brick types currently on the board
	const vector<int>& getCurrentBrickTypes(void) { return cellManager->getCurrentBrickTypes(); }

private:

	// height and width in pixels that we have available for the play board
	dimensions boardSize;

	// dimensions of our board background tile
	dimensions bgTileSize;

	// where we'll start the playboard at
	coords startLoc;

	// our cell manager
	cellManagerClass * cellManager;

	// the fall managaer
	brickFallManagerClass * fallManager;

	// the surface image of the background tile
	CL_Surface * sur_bgTile;

	// display background tiles?
	bool showBG;

	// display board layout alpha?
	bool showBLalpha;

};

#endif