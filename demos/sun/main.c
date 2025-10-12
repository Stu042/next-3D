#pragma output REGISTER_SP = 0xbfff

#include <arch/zxn.h>
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <stdio.h>
#include <stdlib.h>
#include <z80.h>
#include <im2.h>
#include <intrinsic.h>

#include "../../src/core.h"
#include "../../src/main.h"
#include "../../src/kernel.h"
#include "../../src/render.h"
#include "../../src/clipping.h"
#include "../../src/maths.h"
#include "../../src/render_3D.h"
#include "../../src/sprites.h"
#include "../../src/experiments.h"


uint8_t	renderMode = 1;					// 0: Wireframe, 1: Filled
Point16_3D sun_pos = { 0, 0, 20000 };	// The sun position
Object_3D object[MAX_OBJECTS];			// List of objects to display


void init() {
//  BREAK;
    NextReg(0x57,2);          	// Page in kernel
    initKernel();
    initIRQs();
    NextReg(0x08,0x4A);        	// Disable RAM contention, enable DAC and turbosound
//  NextReg(0x05,0x04);			// 60Hz mode
	NextReg(0x15,0x21);			// Enable sprites and clipping, SLU priority
	setCPU(3);					// 28Mhz
    initL2();
	zx_border(INK_BLACK);
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



#define tableOfLineWidths_Length (128)
uint8_t tableOfLineWidths[tableOfLineWidths_Length] = {
	16, 23, 28, 32, 35, 39, 42, 45, 47, 50, 52, 54, 56, 58, 60, 62,
	64, 65, 67, 69, 70, 72, 73, 75, 76, 77, 79, 80, 81, 82, 84, 85,
	86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 97, 98, 99, 100,
	101, 101, 102, 103, 104, 104, 105, 106, 107, 107, 108, 108, 109, 110, 110, 111,
	111, 112, 113, 113, 114, 114, 115, 115, 116, 116, 117, 117, 117, 118, 118, 119,
	119, 119, 120, 120, 121, 121, 121, 122, 122, 122, 123, 123, 123, 123, 124, 124,
	124, 124, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127,
	127, 127, 127, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};


void RenderCircleF(const Point16 pos, const int16_t radius, const uint8_t colourIndex) {
	if (radius <= 0) {
		return;
	}
	int16_t line = (int16_t)(pos.y - radius);
	const int16_t lastLine = (int16_t)(pos.y + radius);
	uint8_t tableIndex = 0;
	uint16_t deltaTableIndex = 0;
	uint8_t count = 8;
	while (count-- > 0 && line <= lastLine && line < 192) {
		if (line >= 0) {
			const uint16_t halfWidth = (tableOfLineWidths[tableIndex] * radius) >> 7;
			int16_t left = (int16_t)(pos.x - (int16_t)halfWidth);
			if (left < 0) {
				left = 0;
			}
			int16_t right = (int16_t)(pos.x + (int16_t)halfWidth);
			if (right > 255) {
				right = 255;
			}
			const uint8_t width = right - left;
			uint8_t *de = getPixelAddress((uint8_t)line, (uint8_t)left);
			for(uint8_t i = 0; i < width; i++) {
				*de++ = colourIndex;
			}
			uint8_t banks = screen_banks;
		}
		line++;
		deltaTableIndex += (tableOfLineWidths_Length << 7) / radius;
		tableIndex += deltaTableIndex >> 7;
		if (tableIndex >= tableOfLineWidths_Length) {
			tableIndex = tableOfLineWidths_Length - 1;
		}
		deltaTableIndex &= 0x003f;
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
				RenderCircleF(t, r, 0xFC);
				// circleL2F(t,r,0xFC);
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


void TestPixelAddress() {
	uint8_t *de = getPixelAddress(1, 1);
	*de = 0xfc;
}


//////////////////////////////////////
// Code entry
void main(void) {
	init();

	while(1) {
		clearL2(0);
		readKeyboard();
		Run();
		TestPixelAddress();

		//waitVBlank();	// Wait for the vblank before switching
		swapL2(); 		// Do the double-buffering
	};
}
