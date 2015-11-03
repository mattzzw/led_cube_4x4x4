#include "draw.h"
#include "cube.h"
#include <string.h>


void rangecheck(byte x, byte y, byte z){
    
    if (x>CUBE_SIZE || y > CUBE_SIZE || z > CUBE_SIZE || 
            x<0 || y<0 || z<0){
        // cheap version of BSOD...
        fill(0x0a);
        // wait for reset...
        for (;;){}
    }
}

void setVoxel(byte x, byte y, byte z) {
    rangecheck(x, y, z);
    cube[z][y] |= (1 << x);
}

void clearVoxel(byte x, byte y, byte z) {
    rangecheck(x, y, z);
    cube[z][y] &= ~(1 << x);
}

// Get the current status of a voxel

unsigned char getVoxel(int x, int y, int z) {
    rangecheck(x, y, z);

    if (cube[z][y] & (1 << x)) {
        return 1;
    } else {
        return 0;
    }
}

// In some effect we want to just take bool and write it to a voxel
// this function calls the apropriate voxel manipulation function.

void alterVoxel(int x, int y, int z, int state) {
    rangecheck(x, y, z);
    if (state == 1) {
        setVoxel(x, y, z);
    } else {
        clearVoxel(x, y, z);
    }
}

// Flip the state of a voxel.
// If the voxel is 1, its turned into a 0, and vice versa.
void flpvoxel(int x, int y, int z)
{
	rangecheck(x, y, z);
		cube[z][y] ^= (1 << x);
}


void fill(byte x){
    for (int y =0; y<CUBE_SIZE; y++)
        for (int z=0; z< CUBE_SIZE; z++)
        cube[y][z]=x;
}

// Sets all voxels along a X/Y plane at a given point
// on axis Z

void setplane_z(int z) {
    int i;
    if (z >= 0 && z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
            cube[z][i] = 0xff;
    }
}

// Clears voxels in the same manner as above

void clrplane_z(int z) {
    int i;
    if (z >= 0 && z < CUBE_SIZE) {
        for (i = 0; i < CUBE_SIZE; i++)
            cube[z][i] = 0x00;
    }
}

void setplane_x(int x) {
    int z;
    int y;
    if (x >= 0 && x < CUBE_SIZE) {
        for (z = 0; z < CUBE_SIZE; z++) {
            for (y = 0; y < CUBE_SIZE; y++) {
                cube[z][y] |= (1 << x);
            }
        }
    }
}

void clearCube(void) {
    for (int z = 0; z < 4; z++) {
        for (int y = 0; y < 4; y++) {
            cube[z][y] = 0;
        }
    }
}

void fillCube(void) {
    for (int z = 0; z < 4; z++) {
        for (int y = 0; y < 4; y++) {
            cube[z][y] = 0x0f;
        }
    }
}

// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.

char flipbyte(char byte) {
    char flop = 0x00;

    flop = (flop & 0b11111110) | (0b00000001 & (byte >> 7));
    flop = (flop & 0b11111101) | (0b00000010 & (byte >> 5));
    flop = (flop & 0b11111011) | (0b00000100 & (byte >> 3));
    flop = (flop & 0b11110111) | (0b00001000 & (byte >> 1));
    flop = (flop & 0b11101111) | (0b00010000 & (byte << 1));
    flop = (flop & 0b11011111) | (0b00100000 & (byte << 3));
    flop = (flop & 0b10111111) | (0b01000000 & (byte << 5));
    flop = (flop & 0b01111111) | (0b10000000 & (byte << 7));
    return flop;
}


// Makes sure x1 is alwas smaller than x2
// This is usefull for functions that uses for loops,
// to avoid infinite loops

void argorder(int ix1, int ix2, int *ox1, int *ox2) {
    if (ix1 > ix2) {
        int tmp;
        tmp = ix1;
        ix1 = ix2;
        ix2 = tmp;
    }
    *ox1 = ix1;
    *ox2 = ix2;
}

// Returns a byte with a row of 1's drawn in it.
// byteline(2,5) gives 0b00111100

char byteline(int start, int end) {
    return ((0xff << start) & ~(0xff << (end + 1)));
}

// Draw a wireframe box. This only draws the corners and edges,
// no walls.

void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2) {
    int iy;
    int iz;

    argorder(x1, x2, &x1, &x2);
    argorder(y1, y2, &y1, &y2);
    argorder(z1, z2, &z1, &z2);

    // Lines along X axis
    cube[z1][y1] = byteline(x1, x2);
    cube[z1][y2] = byteline(x1, x2);
    cube[z2][y1] = byteline(x1, x2);
    cube[z2][y2] = byteline(x1, x2);

    // Lines along Y axis
    for (iy = y1; iy <= y2; iy++) {
        setVoxel(x1, iy, z1);
        setVoxel(x1, iy, z2);
        setVoxel(x2, iy, z1);
        setVoxel(x2, iy, z2);
    }

    // Lines along Z axis
    for (iz = z1; iz <= z2; iz++) {
        setVoxel(x1, y1, iz);
        setVoxel(x1, y2, iz);
        setVoxel(x2, y1, iz);
        setVoxel(x2, y2, iz);
    }
}


// Flip the cube 180 degrees along the y axis.

void mirror_y(void) {
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char x, y, z;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    clearCube();
    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            for (x = 0; x < CUBE_SIZE; x++) {
                if (buffer[z][y] & (0x01 << x))
                    setVoxel(x, CUBE_SIZE - 1 - y, z);
            }
        }
    }

}

// Flip the cube 180 degrees along the x axis

void mirror_x(void) {
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char y, z;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    clearCube();

    for (z = 0; z < CUBE_SIZE; z++) {
        for (y = 0; y < CUBE_SIZE; y++) {
            // This will break with different buffer sizes..
            cube[z][y] = flipbyte(buffer[z][y]);
        }
    }
}

// flip the cube 180 degrees along the z axis

void mirror_z(void) {
    unsigned char buffer[CUBE_SIZE][CUBE_SIZE];
    unsigned char z, y;

    memcpy(buffer, cube, CUBE_BYTES); // copy the current cube into a buffer.

    for (y = 0; y < CUBE_SIZE; y++) {
        for (z = 0; z < CUBE_SIZE; z++) {
            cube[CUBE_SIZE - 1 - z][y] = buffer[z][y];
        }
    }
}

void fillPlane(byte plane, byte mode) {
    clearCube();
    switch (mode) { // x, y, z
        case 0:
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    setVoxel(x, y, plane);
                }
            }
            break;
        case 1:
            for (int y = 0; y < 4; y++) {
                for (int z = 0; z < 4; z++) {
                    setVoxel(plane, y, z);
                }
            }
            break;
        case 2:
            for (int x = 0; x < 4; x++) {
                for (int z = 0; z < 4; z++) {
                    setVoxel(x, plane, z);
                }
            }
            break;
        default:
            break;
    }
}

void shift(char axis, int direction) {
    int i, x, y;
    int ii, iii;
    int state;

    for (i = 0; i < 4; i++) {
        if (direction == -1) {
            ii = i;
        } else {
            ii = (3 - i);
        }


        for (x = 0; x < 4; x++) {
            for (y = 0; y < 4; y++) {
                if (direction == -1) {
                    iii = ii + 1;
                } else {
                    iii = ii - 1;
                }

                if (axis == AXIS_Z) {
                    state = getVoxel(x, y, iii);
                    alterVoxel(x, y, ii, state);
                }

                if (axis == AXIS_Y) {
                    state = getVoxel(x, iii, y);
                    alterVoxel(x, ii, y, state);
                }

                if (axis == AXIS_X) {
                    state = getVoxel(iii, y, x);
                    alterVoxel(ii, y, x, state);
                }
            }
        }
    }

    if (direction == -1) {
        i = 3;
    } else {
        i = 0;
    }

    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            if (axis == AXIS_Z)
                clearVoxel(x, y, i);

            if (axis == AXIS_Y)
                clearVoxel(x, i, y);

            if (axis == AXIS_X)
                clearVoxel(i, y, x);
        }
    }
}

