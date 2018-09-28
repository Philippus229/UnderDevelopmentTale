#include <snes.h>
//#include <stdio.h>

extern char ruinsgfx, ruinsgfx_end;
extern char ruinspal;
extern char ruins;
extern char ruinscol;

extern char ruins2gfx, ruins2gfx_end;
extern char ruins2pal;
extern char ruins2;
extern char ruins2col;

extern char gfxfrisk, gfxfrisk_end;
extern char frisksprite, frisksprite_end;

short scrX=0, scrY=0;
int minScrollX=0, maxScrollX=0, minScrollY=0, maxScrollY=0;
int rn = 0;
int testTimer = 0;
bool testTested = false;

typedef struct {
    int x, y;
	int state;
	int frame;
	int underframe;
	bool move;
} Frisk;

Frisk frisk;

enum FriskState {W_UP = 0, W_DOWN = 4, W_RIGHT = 8,  W_LEFT = 12};

int lz[2][1][13] = {{{433,65,505,107,1,100,175,0,0,0,256,0,0}},{{90,190,110,224,0,100,100,0,0,0,276,0,0}}};

u16 getCollisionTile(u16 x, u16 y) {
    u16 *ptrMap;
    if(rn == 0) {
	    ptrMap = (u16 *) &ruinscol + (y>>3)*128 + (x>>3);
	} else if (rn == 1) {
	    ptrMap = (u16 *) &ruins2col + (y>>3)*128 + (x>>3);
	}
	return (*ptrMap);
}

/*char **split_char(char *in_string, char split_char) {
    unsigned char **tmp_out;
    int i2 = 0;
	int i3 = 0;
    int i = 0;
	for(i=0;i<strlen(in_string);i++) {
	    if(in_string[i] == split_char) {
		    i2++;
			i3 = 0;
		} else {
		    tmp_out[i2][i3] = in_string[i];
		}
		i3++;
	}
	return tmp_out;
}*/

void load_room(int rn2, int startX, int startY, int startScrX, int startScrY, int minSX, int maxSX, int minSY, int maxSY) {
	if (rn2 == 0) {
		bgInitTileSet(1, &ruinsgfx, &ruinspal, 0, (&ruinsgfx_end - &ruinsgfx), 16*2, BG_256COLORS, 0x6000);
	    int i;
	    for (i=0;i<27;i++) { //28 = (map_height/16)*2
		    u8 *ptrMap  = &ruins + 128*i*2; //128 = (map_width/16)*2
		    if (i>=28) ptrMap  = &ruins + 128*5*2; //init anything else with white lines
		    u16 ptrVRAM = 0x1000+i*32; //screen begin to 0x1000
		    dmaCopyVram(ptrMap, ptrVRAM, 32*2); //copy row to VRAM
		    dmaCopyVram((ptrMap+32*2), (ptrVRAM+32*32), 32*2); //copy row to VRAM
	    }
    } else if (rn2 == 1) {
	    bgInitTileSet(1, &ruins2gfx, &ruins2pal, 0, (&ruins2gfx_end - &ruins2gfx), 16*2, BG_256COLORS, 0x6000);
	    int i;
	    for (i=0;i<27;i++) { //28 = (map_height/16)*2
		    u8 *ptrMap  = &ruins2 + 128*i*2; //128 = (map_width/16)*2
		    if (i>=28) ptrMap  = &ruins2 + 128*5*2; //init anything else with white lines
		    u16 ptrVRAM = 0x1000+i*32; //screen begin to 0x1000
		    dmaCopyVram(ptrMap, ptrVRAM, 32*2); //copy row to VRAM
		    dmaCopyVram((ptrMap+32*2), (ptrVRAM+32*32), 32*2); //copy row to VRAM
		}
	}
	bgSetMapPtr(1, 0x1000, SC_64x32);
	oamInitGfxSet(&gfxfrisk, (&gfxfrisk_end-&gfxfrisk), &frisksprite, (&frisksprite_end-&frisksprite), 0, 0x0000, OBJ_SIZE32);
	oamSet(0,  0, 0, 3, 0, 0, 4, 0);
    oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
	setMode(BG_MODE1,0); bgSetDisable(2);
	setScreenOn();
	rn = rn2;
	scrX = startScrX;
	scrY = startScrY;
	minScrollX = minSX;
	maxScrollX = maxSX;
	minScrollY = minSY;
	maxScrollY = maxSY;
	frisk.x = startX;
	frisk.y = startY;
}

/*void lzc() {
    int i = 0;
	for(i=0;lz[rn][i] != NULL;i++) {
	    if (frisk.x+scrX > lz[rn][i][0] && frisk.x+scrX < lz[rn][i][2] && frisk.y+scrY > lz[rn][i][1] && frisk.y+scrY < lz[rn][i][3]) {//TODO: Make this less RAM-hungry
			load_room(lz[rn][i][4], lz[rn][i][5], lz[rn][i][6], lz[rn][i][7], lz[rn][i][8], lz[rn][i][9], lz[rn][i][10], lz[rn][i][11], lz[rn][i][12]);//TODO: Same here
		}
	}
}*/

int main(void) {
	unsigned short pad0;
	consoleInit();
	load_room(0, 100, 100, 0, 0, 0, 276, 0, 0);
	while (1) {
		if (rn == 0) {
		    if (frisk.x+scrX > 433 && frisk.x+scrX < 505 && frisk.y+scrY > 65 && frisk.y+scrY < 107 && (pad0 & KEY_UP)) {
			    load_room(1, 100, 175, 0, 0, 0, 256, 0, 0);
			}
		} else if (rn == 1) {
		    if (frisk.x+scrX > 80 && frisk.x+scrX < 130 && frisk.y+scrY > 190 && frisk.y+scrY < 224 && (pad0 & KEY_DOWN)) {
			    load_room(0, 175, 100, 276, 0, 0, 276, 0, 0);
			}
		}
		/*if (testTimer % 600 == 0 && testTimer >= 600) {
		    lzc();
		}*/
	    frisk.move = false;
	    pad0 = padsCurrent(0);
		if (pad0 & KEY_UP) {
		    frisk.state = W_UP;
		    if ((getCollisionTile(scrX+frisk.x+7, scrY+frisk.y+7) == 0) && (getCollisionTile(scrX+frisk.x+15, scrY+frisk.y+7) == 0) && (getCollisionTile(scrX+frisk.x+22, scrY+frisk.y+7) == 0)) {
			    if (scrY+(frisk.y-100) >= maxScrollY || scrY+(frisk.y-100) <= minScrollY) {
		            frisk.y -= 1;
				} else {
				    scrY -= 1;
				}
				frisk.move = true;
			}
		}
		if (pad0 & KEY_DOWN) {
		    frisk.state = W_DOWN;
		    if ((getCollisionTile(scrX+frisk.x+7, scrY+frisk.y+33) == 0) && (getCollisionTile(scrX+frisk.x+15, scrY+frisk.y+33) == 0) && (getCollisionTile(scrX+frisk.x+22, scrY+frisk.y+33) == 0)) {
                if (scrY+(frisk.y-100) >= maxScrollY || scrY+(frisk.y-100) <= minScrollY) {
		            frisk.y += 1;
				} else {
				    scrY += 1;
				}
				frisk.move = true;
			}
		}
		if (pad0 & KEY_LEFT) {
		    frisk.state = W_LEFT;
		    if ((getCollisionTile(scrX+frisk.x+6, scrY+frisk.y+8) == 0) && (getCollisionTile(scrX+frisk.x+6, scrY+frisk.y+20) == 0) && (getCollisionTile(scrX+frisk.x+6, scrY+frisk.y+32) == 0)) {
                if (scrX+(frisk.x-100) >= maxScrollX || scrX+(frisk.x-100) <= minScrollX) {
		            frisk.x -= 1;
				} else {
				    scrX -= 1;
				}
				frisk.move = true;
			}
		}
		if (pad0 & KEY_RIGHT) {
		    frisk.state = W_RIGHT;
		    if ((getCollisionTile(scrX+frisk.x+23, scrY+frisk.y+8) == 0) && (getCollisionTile(scrX+frisk.x+23, scrY+frisk.y+20) == 0) && (getCollisionTile(scrX+frisk.x+23, scrY+frisk.y+32) == 0)) {
                if (scrX+(frisk.x-100) >= maxScrollX || scrX+(frisk.x-100) <= minScrollX) {
		            frisk.x += 1;
				} else {
				    scrX += 1;
				}
				frisk.move = true;
			}
		}
		oamSetXY(0, frisk.x, frisk.y);
		if (frisk.move == false) {
		    frisk.frame = 0;
			frisk.underframe = 0;
		} else {
			frisk.underframe++;
			if (frisk.underframe >= 60) {
			    frisk.underframe = 0;
			} else {
			    frisk.underframe++;
			}
			if ((frisk.underframe/15) > frisk.frame) {
		        if (frisk.frame == 3) {
			        frisk.frame = 0;
			    } else {
			        frisk.frame++;
			    }
			}
		}
		if (frisk.state == 0) {
		    oamSet(0, frisk.x, frisk.y, 3, 0, 0, (frisk.state+frisk.frame)*4, 0);
		} else if (frisk.state == 4) {
		    oamSet(0, frisk.x, frisk.y, 3, 0, 0, (frisk.state*16)+(frisk.frame*4), 0);
		} else if (frisk.state == 8) {
		    oamSet(0, frisk.x, frisk.y, 3, 0, 0, (frisk.state*16)+(frisk.frame*4), 0);
		} else {
		    oamSet(0, frisk.x, frisk.y, 3, 1, 0, (8*16)+(frisk.frame*4), 0);
		}
		bgSetScroll(1,scrX,scrY);
		testTimer++;
		WaitForVBlank();
	}
	return 0;
}