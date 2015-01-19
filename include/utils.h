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

  utility classes

*/


#ifndef GNOOD_UTILS
#define GNOOD_UTILS


class coords {
public:
	coords() {
		x = 0;
		y = 0;
	}

	coords (int _x, int _y) {
		x = _x;
		y = _y;
	}

    coords (const coords& loc) {
        x = loc.x;
        y = loc.y;
    }

	friend bool operator == (const coords& coord1, const coords& coord2) {
		return ((coord1.x == coord2.x)&&(coord1.y == coord2.y));
	}

	friend bool operator != (const coords& coord1, const coords& coord2) {
		return ((coord1.x != coord2.x)||(coord1.y != coord2.y));
	}

   int x;
   int y;
};


class dimensions {
public:
	dimensions() {
		w = 0;
		h = 0;
	}
	dimensions(int _w, int _h) {
		w = _w;
		h = _h;
	}
    dimensions(const dimensions& size) {
        w = size.w;
        h = size.h;
    }

   int w;
   int h;
};      
      
      
#endif
