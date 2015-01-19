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

#include <stdio.h>
#include "gnood.h"
#include "config.h"

// global reference to the application				 
GnoodApp gameRef;		 

// global resource manager
CL_ResourceManager * resources;

// our main font
CL_Font * mainFont;

// our brick manager
brickManagerClass * brickManager;

// our score board class
scoreBoardClass * scoreBoard;

// main program entry point
int GnoodApp::main(int /*argc*/, char** /*argv*/)
{

	showSkin = true;
	showFPS = false;
	cout << "Game of Gnood\nwritten by shannon weyrick (weyrick@roadsend.com)" << endl;

	try
	{
		// create resource manager
        resources = CL_ResourceManager::create("gnood.dat",true);

        // randomize seed
        srandom(time(0));

		// initialize the display
		initDisplay();

		// initialize the score board
		scoreBoard = new scoreBoardClass();

		// initialize the brick manager
		brickManager = new brickManagerClass();

		// initialize the flight manager
		fallManager = new brickFallManagerClass();

		// initialize the play board
		playBoard = new playBoardClass(fallManager);

		// initialize the flight manager
		flightManager = new brickFlightManagerClass(playBoard);

		// initialize shooter board
		shooterBoard = new shooterBoardClass(playBoard, flightManager);

		// DEBUG
		playBoard->toggleBG();
		toggleSkin();

		// run game loop..
		gameLoop();

		// free up 
		delete resources;

	}
	catch (CL_Error err)
	{
		cout << "Exeception caught: " << err.message << endl;
	}

	return 0;
}


// core init modules
void GnoodApp::init_modules() {

	CL_SetupCore::init();
	CL_SetupCore::init_display();

}

// core deinit modules
void GnoodApp::deinit_modules() {


	// free resources
	delete brickManager;
	delete scoreBoard;
	delete fallManager;
	delete playBoard;
	delete flightManager;
	delete shooterBoard;

	delete mainFont;
	delete skin;

	CL_SetupCore::deinit_display();
	CL_SetupCore::deinit();

}


// initialize display
void GnoodApp::initDisplay()
{
	
	// retrieve main window settings	
	appWidth	= CL_Res_Integer::load("Game/Config/Main/screenWidth", resources, DEFAULT_APP_WIDTH);
	appHeight	= CL_Res_Integer::load("Game/Config/Main/screenHeight", resources, DEFAULT_APP_HEIGHT);
	appBPP		= CL_Res_Integer::load("Game/Config/Main/screenBPP", resources, DEFAULT_APP_BPP);
	
	// Set videomode resolution and bpp (bits per pixel)
	CL_Display::set_videomode(appWidth, appHeight, appBPP, false);

	// load fonts
	mainFont = CL_Font::load("Game/Fonts/fntAvantgardeWhiteBold10", resources);

	// load skin
	skin = CL_Surface::load("Game/Config/Main/skin", resources);

}


// run the game. will loop until user wishes to exit
void GnoodApp::gameLoop()
{

	// mouse tracking
	int mX;

	// hide the mouse cursor
	CL_MouseCursor::hide();

	// loop
	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) {
	
		// clear the buffer
		CL_Display::clear_display();


		// catch mouse position
		if (mX != CL_Mouse::get_x()) {
			// mouse has moved, move shooter ..
			if (mX > CL_Mouse::get_x())
				// to the left
				shooterBoard->increaseShooterAngle(mX-CL_Mouse::get_x());
			else 
				// to the right
				shooterBoard->decreaseShooterAngle(CL_Mouse::get_x()-mX);
		}
		mX = CL_Mouse::get_x(); 

		// toggle grid
		if (CL_Keyboard::get_keycode(CL_KEY_G))
			playBoard->toggleGrid();

		// toggle background
		if (CL_Keyboard::get_keycode(CL_KEY_B))
			playBoard->toggleBG();

		// toggle bricks
		if (CL_Keyboard::get_keycode(CL_KEY_S))
			playBoard->toggleBricks();

		// toggle playboard layout alphas
		if (CL_Keyboard::get_keycode(CL_KEY_L))
			playBoard->togglePBalpha();

		// toggle cell manager layout alphas
		if (CL_Keyboard::get_keycode(CL_KEY_C))
			playBoard->toggleCLalpha();

		// toggle FPS
		if (CL_Keyboard::get_keycode(CL_KEY_F))
			toggleFPS();

		// toggle show skin
		if (CL_Keyboard::get_keycode(CL_KEY_K))
			toggleSkin();

		// decrease shooter angle (fine)
		if ((CL_Keyboard::get_keycode(CL_KEY_RIGHT))&&(!CL_Keyboard::get_keycode(CL_KEY_LSHIFT)))
			shooterBoard->decreaseShooterAngle();
		// decrease shooter angle (fast)
		if ((CL_Keyboard::get_keycode(CL_KEY_RIGHT))&&(CL_Keyboard::get_keycode(CL_KEY_LSHIFT)))
			shooterBoard->decreaseShooterAngle(5);


		// increase shooter angle (fine)
		if ((CL_Keyboard::get_keycode(CL_KEY_LEFT))&&(!CL_Keyboard::get_keycode(CL_KEY_LSHIFT)))
			shooterBoard->increaseShooterAngle();
		// increase shooter angle (fast)
		if ((CL_Keyboard::get_keycode(CL_KEY_LEFT))&&(CL_Keyboard::get_keycode(CL_KEY_LSHIFT)))
			shooterBoard->increaseShooterAngle(5);

		// launch brick
		if ((CL_Keyboard::get_keycode(CL_KEY_SPACE))|| (CL_Mouse::left_pressed()) )
			shooterBoard->launchBrick();

		// hand out time slices
		
		// .. play board
		playBoard->timeSlice();

		// lay down the skin
		if (showSkin)
			skin->put_screen(0,0);

		// .. shooter board
		shooterBoard->timeSlice();

		// .. brick flight
		flightManager->timeSlice();

		// .. falling bricks
		fallManager->timeSlice();

		// .. score board
		scoreBoard->timeSlice();

		// are we showing FPS?
		// FIXME

		// flip from backbuffer
		CL_Display::flip_display();

		// system keepalive
		CL_System::keep_alive();

	}
	

}

