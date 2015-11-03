/* 
 * File:   cube.h
 * Author: mattzz
 *
 * Created on December 29, 2011, 8:58 PM
 */

#ifndef CUBE_H
#define	CUBE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CUBE_SIZE 4
#define CUBE_BYTES 16

volatile unsigned char cube[CUBE_SIZE][CUBE_SIZE];

#define AXIS_X 0x78
#define AXIS_Y 0x79
#define AXIS_Z 0x7a

volatile int ledOnTime; 

#ifdef	__cplusplus
}
#endif

#endif	/* CUBE_H */

