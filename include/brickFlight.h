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


  brick flight manager


*/


#ifndef BRICK_FLIGHT
#define BRICK_FLIGHT

#include <list>
#include "playBoard.h"
#include "brick.h"

#define MAX_IN_FLIGHT	1

using namespace std;      

/*
 * A predicate that returns true if a brick is stationary
 */
class brickIsStationary {  
public:
	 bool operator()(gnoodBrick * brick) const {
	 	return (brick->getBrickState() == BRICK_STATE_IN_CELL);
	}
};

/*
 * A predicate that returns true if a brick is null
 */
class brickHasBeenCleared {  
public:
	 bool operator()(gnoodBrick * brick) const {	 	
		 return (brick->getBrickState() == BRICK_STATE_DEAD);
	}
};

class brickFlightManagerClass {

public:
	brickFlightManagerClass(playBoardClass * pb) :
		playBoard(pb)
	{

        maxInFlight = CL_Res_Integer::load("Game/Config/PlayBoard/maxInFlight", resources, MAX_IN_FLIGHT);

	}


	// accept a new brick for flight
	bool acceptNewBrick(gnoodBrick * newBrick);

	// timeslice
	void timeSlice();

private:

	// array of bricks that are currently in flight
	list<gnoodBrick *> brickList;

	// link to play board
	playBoardClass * playBoard;

    // max in air at a time
    unsigned int maxInFlight;


};      
      

#endif
