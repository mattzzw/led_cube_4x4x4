/* 
 * File:   draw.h
 * Author: mattzz
 *
 * Created on December 29, 2011, 8:11 PM
 */

#ifndef DRAW_H
#define	DRAW_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#include "draw.h"
#include "Arduino.h"
    
void setVoxel(byte x, byte y, byte z);
void clearVoxel(byte x, byte y, byte z);
unsigned char getVoxel(int x, int y, int z);
void alterVoxel(int x, int y, int z, int state);
void flpvoxel(int x, int y, int z);

void setplane_z(int z);
void fill(byte x);
void clrplane_z(int z);
void setplane_x(int x);
void clearCube(void);
void fillCube(void);
char flipbyte(char byte);
void argorder(int ix1, int ix2, int *ox1, int *ox2);
char byteline(int start, int end);
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2);
void mirror_y(void);
void mirror_x(void); 
void mirror_z(void);

void fillPlane(byte plane, byte mode);

void shift(char axis, int direction);
void rangecheck(byte x, byte y, byte z);



#ifdef	__cplusplus
}
#endif

#endif	/* DRAW_H */

