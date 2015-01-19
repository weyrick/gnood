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

  brick fall manager class

*/

#include "brickFall.h"             


// accept a new brick for fall
bool brickFallManagerClass::acceptNewBrick(gnoodBrick * newBrick) {

	// setup slope to move down
	newBrick->setSlope(coords(0,2));

	// okay add the new brick
	brickList.push_back(newBrick);

	return true;

}
	 
// time slice
void brickFallManagerClass::timeSlice() 
{
	// don't bother doing this if we have no bricks in flight
	if (brickList.empty())
		return;

	// for each brick in flight...
	for (list<gnoodBrick *>::const_iterator pos = brickList.begin(); pos != brickList.end(); ++pos) {

		// move brick
		(*pos)->moveBrick();

		// show brick
		(*pos)->showBrick();

		// increase slope to simulate gravity
		(*pos)->setSlope(coords(0,(*pos)->getSlope().y*2));

	}

	// remove all bricks from our flight list that are marked stationary
	// this will DELETE the brick and free the memory
	brickList.remove_if(brickIsOffScreen());    

}
