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

#ifndef GNOOD_FALLMANAGER
#define GNOOD_FALLMANAGER	

#include "brick.h"

/*
 * A predicate that returns true if a brick is off screen. will DELETE brick at this point
 */
class brickIsOffScreen {  
public:
	 bool operator()(gnoodBrick * brick) const {
		if (brick->getScreenLoc().y > CL_Display::get_height()) {
			delete brick;
			return true;
		}
		else {
			return false;
		}
	}
};
	
class brickFallManagerClass {

public:

	// time slice
	void timeSlice();

	// accept a new brick for flight
	bool acceptNewBrick(gnoodBrick * newBrick);

private:
	// array of bricks that are currently in flight
	list<gnoodBrick *> brickList;


};

#endif        
