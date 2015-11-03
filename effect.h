/* 
 * File:   effect.h
 * Author: mattzz
 * 
 * Created on December 29, 2011, 10:29 PM
 */

#ifndef EFFECT_H
#define	EFFECT_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "cube.h"
#include "draw.h"
#include <avr/pgmspace.h>

    // workaround for a PROGMEM bug in gcc
#define PS __attribute__((section (".progmem.data")))     

    const unsigned char spinning_line[6][2] PS ={
        { 0x84, 0x21},
        { 0x0c, 0x30},
        { 0x03, 0xc0},
        { 0x12, 0x48},
        { 0x22, 0x44},
        { 0x44, 0x22},
    };

    void fillCubeDrawing(void);
    void sweepPlanes(void);
    void rndFill(void);
    void drawFrame(int del);

    void sendvoxel_z(unsigned char x, unsigned char y, unsigned char z, int del);
    void sendplane_rand_z(unsigned char z, int del, int wait);
    void sendvoxels_rand_z(int iterations, int del, int wait);
    void effectRain(int iterations);
    void starBox(void);
    void rotateWall(int iterations, int del);
    // Set n number of voxels at random positions

    void effect_random_sparkle_flash(int iterations, int voxels, int del);

    void effect_random_sparkle(void);
    void draw_positions_axis(char axis, unsigned char positions[64], int invert);
    void effect_axis_updown_randsuspend(char axis, int del, int sleep, int invert);

    void box_grow(byte x1, byte y1, byte z1, byte grow, int del, int wait);
    void effect_box_shrink_grow(int del, int wait);

    void effect_wormsqueeze(int size, int axis, int direction, int iterations, int del);

    void effect_running_light_z(int z, int del);
    void effect_running_up_down(int del);

    void effect_spiral(int direction, int iterations, int delay);
    void effect_spinning_plane(int direction, int iterations, int delay);


#ifdef	__cplusplus
}
#endif

#endif	/* EFFECT_H */

