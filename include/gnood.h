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
	
	gnood include file.

*/

#ifndef GNOOD_MAIN
#define GNOOD_MAIN

#include <iostream.h>
#include <ClanLib/core.h>

#include "playBoard.h"
#include "shooterBoard.h"
#include "scoreBoard.h"
#include "brick.h"
#include "brickFlight.h"
#include "brickFall.h"

class GnoodApp : public CL_ClanApplication
{
public:	
	virtual char *get_title() { return "the game of Gnood"; }
	virtual int main(int argc, char** argv);
    void virtual init_modules();
    void virtual deinit_modules();

protected:

	void initDisplay();
	void gameLoop();

	// toggle FPS display
	void toggleFPS() { (showFPS) ? showFPS = false : showFPS = true; }

	// toggle skin display
	void toggleSkin() { (showSkin) ? showSkin = false : showSkin = true; }

private:

	// our top level classes
	playBoardClass * playBoard;
	shooterBoardClass * shooterBoard;
	brickFlightManagerClass * flightManager;
	brickFallManagerClass * fallManager;

	// are we showing FPS now?
	bool showFPS;
	bool showSkin;

	// skin
	CL_Surface * skin;

	// dimensions of our main window
	int appHeight;
	int appWidth;
	int appBPP;


};

#endif
