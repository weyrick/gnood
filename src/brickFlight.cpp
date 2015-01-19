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

#include "brickFlight.h"

// accept a new brick for flight
bool brickFlightManagerClass::acceptNewBrick(gnoodBrick * newBrick) {

	// don't allow more than max at once
	if (brickList.size() == maxInFlight)
		return false;

	// set brick state
	newBrick->setBrickState(BRICK_STATE_FIRED);

	// okay add the new brick
	brickList.push_back(newBrick);

	return true;

}

// animate / move current bricks in flight
void brickFlightManagerClass::timeSlice() {

	// don't bother doing this if we have no bricks in flight
	if (brickList.empty())
		return;

	// for each brick in flight...
	for (list<gnoodBrick *>::const_iterator pos = brickList.begin(); pos != brickList.end(); ++pos) {

		// check the location of this brick for boundries
		// if this function return true, then our brick has been zapped or cleared
		// so we want to not show it and remove it
		playBoard->moveInPlayArea(*pos);

		if ((*pos)->getBrickState() == BRICK_STATE_FIRED)
			(*pos)->showBrick();

	}

	// remove all bricks from our flight list that are marked stationary
	brickList.remove_if(brickIsStationary());    
	brickList.remove_if(brickHasBeenCleared());

}

