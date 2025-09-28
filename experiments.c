/*
 * Title:			Experimental Code
 * Author:			Dean Belfield
 * Created:			20/08/2025
 * Last Updated:	24/09/2025
 *
 * Modinfo:
 */

#include <arch/zxn.h>
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <stdio.h>
#include <stdlib.h>
#include <z80.h>

#include "core.h"
#include "render.h"
#include "clipping.h"
#include "maths.h"
#include "render_3D.h"
#include "experiments.h"

extern int pd;
extern uint8_t renderMode;
extern Point16 point_t[64];
extern Angle_3D cam_theta;
extern Point16_3D cam_pos;

#define starCount 32

Point16_3D stars[starCount];

// ***************************************************************************************************************************************
//  Sample routines
// ***************************************************************************************************************************************

int16_t dotp(Point16_3D p1, Point16_3D p2) {
	return (p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z);
}

int16_t crossp(Point16_3D p1, Point16_3D p2) {
	return (p1.x * p2.x) - (p1.y * p2.y) - (p1.z * p2.z);
}

Point8_3D calculateNormal(Point8_3D p1, Point8_3D p2, Point8_3D p3) {
	Point8_3D a = {
		p2.x - p1.x,
		p2.y - p1.y,
		p2.z - p1.z,
	};
	Point8_3D b = {
		p3.x - p1.x,
		p3.y - p1.y,
		p3.z - p1.z,
	};
	Point8_3D n = {
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x),
	};
	return n;
}

void initStar(Point16_3D * star) {
	star->x = 256 - rand()%512;
	star->y = 256 - rand()%512;
	star->z = 1024 + rand()%1024;
}

void initStars(void) {
	int	i;
	for(i=0; i<starCount; i++) {
		initStar(&stars[i]);
	}
}

void drawStars(int speed) {
	int	i;
	Point16_3D * star;

	for(i=0; i<starCount; i++) {
		star = &stars[i];
		int x = fastMulDiv(star->x, 256, star->z) + 128;
		int y = fastMulDiv(star->y, 256, star->z) + 96;
		if(x >=0 && x <=255 && y >=0 && y <= 191) {
			if(star->z > 512) {
				plotL2(x,y,0xFF);
			}
			else {
				plotL2(x,y,0xFF);
				plotL2(x+1,y,0xFF);
				plotL2(x,y+1,0xFF);
				plotL2(x+1,y+1,0xFF);

			}
		}
		star->z -= speed;
		if(star->z  < 0) {
			initStar(star);
		}
	}
}
