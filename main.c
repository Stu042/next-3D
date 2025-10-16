// /*
//  * Title:			Spectrum Next 3D Engine
//  * Author:			Dean Belfield
//  * Contributors:	Henrique Olifiers, Michael "Flash" Ware
//  * Created:			20/08/2025
//  * Last Updated:	22/09/2025
//  *
//  * Modinfo:
//  * 04/09/2025:		Moved models to includes in models folder
//  * 22/09/2025:		Beta version 0.5
//  */

// #pragma output REGISTER_SP = 0xbfff

// //#define test_triangles

// #include <arch/zxn.h>
// #include <stdint.h>             // standard names for ints with no ambiguity 
// #include <stdio.h>
// #include <stdlib.h>
// #include <z80.h>
// #include <im2.h>
// #include <intrinsic.h>

// #include "core.h"
// #include "main.h"
// #include "kernel.h"
// #include "render.h"
// #include "clipping.h"
// #include "maths.h"
// #include "render_3D.h"
// #include "sprites.h"
// #include "experiments.h"		// Work-in-progress stuff

// // ***************************************************************************************************************************************
// // Sprites data
// // ***************************************************************************************************************************************

// #include "sprites/cursors.h"

// // ***************************************************************************************************************************************
// // Model data
// // ***************************************************************************************************************************************

// // Elite model data courtesy of Mark Moxon
// // https://elite.bbcelite.com/6502sp/main/variable/ship_cobra_mk_3.html
// //
// #include "models/cube.h"
// #include "models/cobra_mk3.h"
// #include "models/coriolis_station.h"

// // ***************************************************************************************************************************************
// // Global data
// // ***************************************************************************************************************************************

// uint8_t	renderMode = 1;					// 0: Wireframe, 1: Filled
// Point16_3D sun_pos = { 0, 0, 20000 };	// The sun position
// Object_3D object[MAX_OBJECTS];			// List of objects to display

// // ***************************************************************************************************************************************
// //  Main startup and loop
// // ***************************************************************************************************************************************

// void rotate(int i) {
// 	Object_3D * self = &object[i];
// 	self->theta.x+=1;
// 	self->theta.y+=2;
// 	self->theta.z-=1;
// }

// void drawSun() {
// 	BREAK
// 	Point16_3D p = {
// 		sun_pos.x - cam_pos.x,
// 		sun_pos.y - cam_pos.y,
// 		sun_pos.z - cam_pos.z,
// 	};
// 	p = rotate16_3D(p, cam_theta);
// 	if(p.z >= 200 && abs(p.x) < p.z && abs(p.y) < p.z ) {
// 		Point16 t = {
// 		    fastMulDiv(p.x, pd, p.z) + 128,
// 		    fastMulDiv(p.y, pd, p.z) + 96,
// 		};
// 		int16_t r = (32768-p.z)/256;
// 		if(r > 0) {
// 			if(renderMode) {
// 				circleL2F(t,r,0xFC);
// 			}
// 			else {
// 				circleL2(t,r,0xFF);
// 			}
// 		}
// 	}
// }


// void init() {
// //  BREAK;
//     NextReg(0x57,2);          	// Page in kernel
//     initKernel();
//     initIRQs();
//     NextReg(0x08,0x4A);        	// Disable RAM contention, enable DAC and turbosound
// //  NextReg(0x05,0x04);			// 60Hz mode
// 	NextReg(0x15,0x21);			// Enable sprites and clipping, SLU priority
// 	setCPU(3);					// 28Mhz
//     initL2();
// 	zx_border(INK_BLACK);
// 	spriteInit(0x00, &sprite_circle[0]);
// 	spriteInit(0x01, &sprite_cross[0]);
// }

// #ifdef test_triangles
// //////////////////////////////////////
// // Triangle

// Point16 p1, p2, p3;
// Point16 *p;
// void InitObjects() {
// 	p1.x = 10;
// 	p1.y = 10;
// 	p2.x = 250;
// 	p2.y = 50;
// 	p3.x = 60;
// 	p3.y = 170;
// 	p = &p1;
// }

// void Run() {
// 	if(renderMode) {
// 		triangleL2CF(p1, p2, p3, 0xFC);
// 	}
// 	else {
// 		triangleL2C(p1, p2, p3, 0xFF);
// 	}
// 	p->y += Keys[VK_A] - Keys[VK_Q];
// 	p->x += Keys[VK_P] - Keys[VK_O];
// 	if(Keys[VK_ENTER]) {
// 		renderMode = 1-renderMode;
// 		while (Keys[VK_ENTER]) {
// 			readKeyboard();
// 		}
// 	}
// 	if(Keys[VK_SPACE]) {
// 		if(p == &p1) {
// 			p = &p2;
// 		} else if(p == &p2) {
// 			p = &p3;
// 		} else {
// 			p = &p1;
// 		}
// 		while (Keys[VK_SPACE]) {
// 			readKeyboard();
// 		}
// 	}
// }

// #else
// //////////////////////////////////////
// // Cobra, Sun and Stars

// int v = 0;
// int i = 0;

// void InitObjects() {
// 	Point16_3D pos = { 1000, 0, pd*15 };
// 	Angle_3D theta = { 0,0,0 };
// 	object[i].flags = 1;
// 	object[i].move = NULL;
// 	object[i].model = &cube_m;
// 	object[i].theta = theta;
// 	object[i++].pos = pos;

// 	pos.x = -1000;
// 	object[i].flags = 1;
// 	object[i].move = NULL;
// 	object[i].model = &cube_m;
// 	object[i].theta = theta;
// 	object[i++].pos = pos;

// 	pos.x = 0;
// 	pos.z = pd * 1.5;
// 	object[i].flags = 1;
// 	object[i].move = &rotate;
// 	object[i].model = &cobra_m;
// 	object[i].theta = theta;
// 	object[i++].pos = pos;

// 	initStars();
// }

// void chkSystemKeys() {
// 	if(Keys[VK_1]) {
// 		setCPU(0);
// 	}
// 	if(Keys[VK_2]) {
// 		setCPU(1);
// 	}
// 	if(Keys[VK_3]) {
// 		setCPU(2);
// 	}
// 	if(Keys[VK_4]) {
// 		setCPU(3);
// 	}

// 	if(Keys[VK_SPACE]) {
// 		renderMode = 1-renderMode;
// 		while (Keys[VK_SPACE]) {
// 			readKeyboard();
// 		}
// 	}
// }

// void Run() {
// 	cam_theta.z += Keys[VK_X] - Keys[VK_Z];
// 	cam_theta.x += Keys[VK_Q] - Keys[VK_A];
// 	cam_theta.y += Keys[VK_P] - Keys[VK_O];
	
// 	if(Keys[VK_S]) {
// 		v = 0;
// 	}
// 	if(Keys[VK_W])	{					
// 		if(v < 80) {
// 			v+=8;				
// 		}
// 	} else {
// 		if (v > 0) {
// 			v--;
// 		}
// 	}

// 	Point16_3D sp = { 0, 0, v/4 };
// 	Angle_3D dr = {
// 		-cam_theta.x,
// 		-cam_theta.y,
// 		-cam_theta.z,
// 	};
// 	Point16_3D zv = rotate16_3D(sp, dr);
// 	cam_pos.x += zv.x;					
// 	cam_pos.y += zv.y;					
// 	cam_pos.z += zv.z;	

// 	chkSystemKeys();
	
// 	drawStars(v/2);
// 	drawSun();

// 	for(int i=0; i<MAX_OBJECTS; i++) {
// 		if(object[i].flags) {
// 			drawObject(&object[i], renderMode);
// 			if(object[i].move) {
// 				object[i].move(i);
// 			}
// 		}
// 	}
// }
// #endif



// //////////////////////////////////////
// // Code entry
// void main(void) {
// 	init();

// 	InitObjects();

// 	while(1) {
// 		clearL2(0);
// 		readKeyboard();

// 		Run();

// 		waitVBlank();	// Wait for the vblank before switching
// 		swapL2(); 		// Do the double-buffering
// 	};
// }


#pragma output REGISTER_SP = 0xbfff

#include <arch/zxn.h>
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <stdio.h>
#include <stdlib.h>
#include <z80.h>
#include <im2.h>
#include <intrinsic.h>

#include "core.h"
#include "main.h"
#include "kernel.h"
#include "render.h"
#include "clipping.h"
#include "maths.h"
#include "render_3D.h"
#include "sprites.h"
#include "experiments.h"


uint8_t	renderMode = 1;					// 0: Wireframe, 1: Filled
Point16_3D sun_pos = { 0, 0, 20000 };	// The sun position
Object_3D object[MAX_OBJECTS];			// List of objects to display


void init() {
//	BREAK;
	NextReg(0x57,2);          	// Page in kernel
	initKernel();
	initIRQs();
	NextReg(0x08,0x4A);        	// Disable RAM contention, enable DAC and turbosound
//	NextReg(0x05,0x04);			// 60Hz mode
	NextReg(0x15,0x21);			// Enable sprites and clipping, SLU priority
	setCPU(3);					// 28Mhz
	initL2();
	zx_border(INK_RED);
}


void chkSystemKeys() {
	if(Keys[VK_1]) {
		setCPU(0);
	}
	if(Keys[VK_2]) {
		setCPU(1);
	}
	if(Keys[VK_3]) {
		setCPU(2);
	}
	if(Keys[VK_4]) {
		setCPU(3);
	}

	if(Keys[VK_SPACE]) {
		renderMode = 1-renderMode;
		while (Keys[VK_SPACE]) {
			readKeyboard();
		}
	}
}


void rotate(int i) {
	Object_3D * self = &object[i];
	self->theta.x+=1;
	self->theta.y+=2;
	self->theta.z-=1;
}




#define tableOfLineWidths_Length (256)
uint8_t tableOfLineWidths[tableOfLineWidths_Length] = {
	23, 32, 39, 45, 50, 55, 59, 63, 67, 71, 74, 77, 80, 83, 86, 89, 92, 94, 97, 99, 101, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 125, 127, 129, 131, 132, 134, 136, 137, 139, 140, 142, 143, 145, 146, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 165, 167, 168, 169, 170, 171, 172, 173, 174, 175, 177, 178, 179, 180, 181, 182, 183, 184, 185, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 194, 195, 196, 197, 198, 199, 199, 200, 201, 202, 202, 203, 204, 205, 205, 206, 207, 208, 208, 209, 210, 210, 211, 212, 212, 213, 214, 214, 215, 216, 216, 217, 218, 218, 219, 219, 220, 221, 221, 222, 222, 223, 223, 224, 224, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 231, 232, 232, 233, 233, 234, 234, 235, 235, 235, 236, 236, 237, 237, 237, 238, 238, 239, 239, 239, 240, 240, 240, 241, 241, 241, 242, 242, 242, 243, 243, 243, 244, 244, 244, 245, 245, 245, 245, 246, 246, 246, 247, 247, 247, 247, 248, 248, 248, 248, 249, 249, 249, 249, 249, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

void RenderCircleF(const Point16 pos, const int16_t radius, const uint8_t colourIndex) {
	if (radius <= 0) {
		return;
	}
	int16 line = pos.y - radius;
	int16 lastLine = pos.y;
	uint8 tableIndex = 0;
	int positiveThroughTable = 1;
	uint16 deltaTableIndex = 0;
	uint16 deltaDeltaTableIndex = ((uint16)65535) / radius;
	for (uint8 vertical=0; vertical < 2; vertical++) {
		while (line <= lastLine && line < SCREEN_HEIGHT) {
			if (positiveThroughTable) {
				deltaTableIndex += deltaDeltaTableIndex;
			} else {
				deltaTableIndex -= deltaDeltaTableIndex;
			}
			tableIndex += deltaTableIndex >> 8;
			deltaTableIndex &= 0x00ff;
			if (line >= 0) {
				const int16 halfWidth = (tableOfLineWidths[tableIndex] * radius) >> 8;

				int16 left = pos.x - halfWidth;
				if (left < SCREEN_WIDTH) {
					if (left < 0) {
						left = 0;
					}
					int16 right = pos.x + halfWidth;
					if (right < 0) {
						right = 0;
					}
					if (right >= SCREEN_WIDTH) {
						right = SCREEN_WIDTH - 1;
					}

					if (!positiveThroughTable) {
						uint8 bankOffset = line >> 5;
						//uint8 bank = offScreenBank + bankOffset;
						//setPage(bank);
						uint8 *de = (uint8 *)((uint16)left | ((uint16)line & 0x1f));

						const uint8 width = (uint8)(right - left);
						for (uint8 i = 0; i < width; i++) {
							//plotL2(left++, line, colourIndex);
							*de++ = colourIndex;
						}
					}
				}
			}
			line++;
		}
		positiveThroughTable = 0;
		lastLine += radius;
	}
}


void drawSun() {
	Point16_3D p = {
		sun_pos.x - cam_pos.x,
		sun_pos.y - cam_pos.y,
		sun_pos.z - cam_pos.z,
	};
	p = rotate16_3D(p, cam_theta);
	if(p.z >= 200 && abs(p.x) < p.z && abs(p.y) < p.z ) {
		Point16 t = {
		    fastMulDiv(p.x, pd, p.z) + 128,
		    fastMulDiv(p.y, pd, p.z) + 96,
		};
		int16_t r = (32768-p.z)/256;
		if(r > 0) {
			if(renderMode) {
				circleL2F(t, r, 0xFC);
				//RenderCircleF(t, r, 0xFC);
				//circleL2F(t,r,0xFC);
			}
			else {
				circleL2(t,r,0xFF);
			}
		}
	}
}


int velocity = 0;

void Run() {
	cam_theta.z += Keys[VK_X] - Keys[VK_Z];
	cam_theta.x += Keys[VK_Q] - Keys[VK_A];
	cam_theta.y += Keys[VK_P] - Keys[VK_O];
	
	if(Keys[VK_S]) {
		velocity = 0;
	}
	if(Keys[VK_W])	{					
		if(velocity < 80) {
			velocity+=8;				
		}
	} else {
		if (velocity > 0) {
			velocity--;
		}
	}

	Point16_3D deltaPos = { 0, 0, velocity/4 };
	Angle_3D deltaRot = {
		-cam_theta.x,
		-cam_theta.y,
		-cam_theta.z,
	};
	Point16_3D deltaMove = rotate16_3D(deltaPos, deltaRot);
	cam_pos.x += deltaMove.x;					
	cam_pos.y += deltaMove.y;					
	cam_pos.z += deltaMove.z;	

	chkSystemKeys();
	
	drawSun();
}


void TestMyCircle() {
	Point16 t = {
		100,
		100,
	};
	circleL2F(t, 50, 0xFC);
}

//////////////////////////////////////
// Code entry
void main(void) {
	init();
	Point16 t = {
		128,
		96,
	};
	while(1) {
		clearL2(0);

		zx_border(INK_YELLOW);
		newCircleL2F(t, 50, 0xFC);
		zx_border(INK_GREEN);

		waitVBlank();	// Wait for the vblank before switching
		zx_border(INK_RED);
		swapL2(); 		// Do the double-buffering
	};

	while(1) {
		clearL2(0);

		zx_border(INK_YELLOW);
		circleL2F(t, 50, 0xFC);
		zx_border(INK_GREEN);

		waitVBlank();	// Wait for the vblank before switching
		zx_border(INK_RED);
		swapL2(); 		// Do the double-buffering
	};
}
