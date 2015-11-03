#include "Arduino.h"

#include "cube.h"
#include "draw.h"
#include "effect.h"

void fillCubeDrawing(void) {
    clearCube();

    for (int z = 0; z < 4; z++)
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++) {
                setVoxel(x, y, z);
                delay(20);
            }
}

void sweepPlanes(void) {
    int del = 50;

    //horizontal planes
    for (int i = 0; i < 4; i++) {
        fillPlane(i, 0);
        delay(del);
    }
    for (int i = 3; i >= 0; i--) {
        fillPlane(i, 0);
        delay(del);
    }

    for (int i = 0; i < 4; i++) {
        fillPlane(i, 1);
        delay(del);
    }
    for (int i = 3; i >= 0; i--) {
        fillPlane(i, 1);
        delay(del);
    }

    for (int i = 0; i < 4; i++) {
        fillPlane(i, 2);
        delay(del);
    }
    for (int i = 3; i >= 0; i--) {
        fillPlane(i, 2);
        delay(del);
    }
}

void rndFill(void) {

    clearCube();

    for (int i = 0; i < 200; i++) {
        setVoxel(rand() % 4, rand() % 4, rand() % 4);
        delay(10);
    }
    for (int i = 0; i < 200; i++) {
        clearVoxel(rand() % 4, rand() % 4, rand() % 4);
        delay(10);
    }

}

void box_grow(byte x1, byte y1, byte z1, byte grow, int del, int wait) {

    int dx, dy, dz;
    int x2, y2, z2;

    // grow from x1, y1, z1
    if (grow == 1) {
        if (x1 == 3) {
            dx = -1;
            x2 = 3;
        } else {
            dx = 1;
            x2 = 0;
        }

        if (y1 == 3) {
            dy = -1;
            y2 = 3;
        } else {
            dy = 1;
            y2 = 0;
        }

        if (z1 == 3) {
            dz = -1;
            z2 = 3;
        } else {
            dz = 1;
            z2 = 0;
        }
    } else
        // shrink to x1, y1, z1
    {
        if (x1 == 3) {
            dx = 1;
            x2 = 0;
        } else {
            dx = -1;
            x2 = 3;
        }

        if (y1 == 3) {
            dy = 1;
            y2 = 0;
        } else {
            dy = -1;
            y2 = 3;
        }

        if (z1 == 3) {
            dz = 1;
            z2 = 0;
        } else {
            dz = -1;
            z2 = 3;
        }
    }



    for (int i = 0; i < 4; i++) {
        clearCube();
        box_wireframe(x1, y1, z1, x2, y2, z2);

        x2 = x2 + dx;
        y2 = y2 + dy;
        z2 = z2 + dz;

        delay(del);
    }

    delay(wait);
}

void effect_box_shrink_grow(int del, int wait) {

    box_grow(0, 0, 0, 1, del, wait);
    box_grow(3, 3, 3, 0, del, wait);
    box_grow(3, 3, 3, 1, del, wait);
    box_grow(0, 0, 0, 0, del, wait);

    box_grow(3, 3, 0, 1, del, wait);
    box_grow(0, 0, 3, 0, del, wait);
    box_grow(0, 0, 3, 1, del, wait);
    box_grow(3, 3, 0, 0, del, wait);

    box_grow(3, 0, 3, 1, del, wait);
    box_grow(0, 3, 0, 0, del, wait);
    box_grow(0, 3, 0, 1, del, wait);
    box_grow(3, 0, 3, 0, del, wait);

    box_grow(3, 0, 0, 1, del, wait);
    box_grow(0, 3, 3, 0, del, wait);
    box_grow(0, 3, 3, 1, del, wait);
    box_grow(3, 0, 0, 0, del, wait);



}


// Send a voxel flying from one side of the cube to the other
// If its at the bottom, send it to the top..

void sendvoxel_z(unsigned char x, unsigned char y, unsigned char z, int del) {
    int i, ii;
    for (i = 1; i < 4; i++) {
        if (z == 3) {
            ii = 3 - i;
            clearVoxel(x, y, ii + 1);
        } else {
            ii = i;
            clearVoxel(x, y, ii - 1);
        }
        setVoxel(x, y, ii);
        delay(del);
    }
}

// Send all the voxels from one side of the cube to the other
// Start at z and send to the opposite side.
// Sends in random order.

void sendplane_rand_z(unsigned char z, int del, int wait) {
    unsigned char loop = 16;
    unsigned char x, y;

    clearCube();

    setplane_z(z);

    // Send voxels at random until all 16 have crossed the cube.
    while (loop) {
        x = rand() % 4;
        y = rand() % 4;
        if (getVoxel(x, y, z)) {
            // Send the voxel flying
            sendvoxel_z(x, y, z, del);
            delay(wait);
            loop--; // one down, loop-- to go. when this hits 0, the loop exits. 
        }
    }
}

// For each coordinate along X and Y, a voxel is set either at level 0 or at level 7
// for n iterations, a random voxel is sent to the opposite side of where it was.

void sendvoxels_rand_z(int iterations, int del, int wait) {
    unsigned char x, y, last_x = 0, last_y = 0, i;

    clearCube();

    // Loop through all the X and Y coordinates
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            // Then set a voxel either at the top or at the bottom
            // rand()%2 returns either 0 or 1. multiplying by 7 gives either 0 or 7.
            setVoxel(x, y, ((rand() % 2)*3));
        }
    }

    delay(1000);

    for (i = 0; i < iterations; i++) {
        // Pick a random x,y position
        x = rand() % 4;
        y = rand() % 4;

        // but not the sameone twice in a row

        if (y != last_y && x != last_x) {
            // If the voxel at this x,y is at the bottom
            if (getVoxel(x, y, 0)) {
                // send it to the top
                sendvoxel_z(x, y, 0, del);
            } else {
                // if its at the top, send it to the bottom
                sendvoxel_z(x, y, 3, del);
            }
            delay(wait);

            // Remember the last move
            last_y = y;
            last_x = x;
        }

    }
}

void effectRain(int iterations) {
    int i, ii;
    int rnd_x;
    int rnd_y;
    int rnd_num;

    clearCube();

    for (ii = 0; ii < iterations; ii++) {
        rnd_num = rand() % 2;

        for (i = 0; i < rnd_num; i++) {
            rnd_x = rand() % 4;
            rnd_y = rand() % 4;
            setVoxel(rnd_x, rnd_y, 3);
        }
        delay(30);
        shift(AXIS_Z, -1);
    }
}

void starBox(void) {
    int ledsave;
    ledsave = ledOnTime;

    ledOnTime = 1;
    setVoxel(0, 0, 0);
    setVoxel(0, 3, 0);
    setVoxel(3, 3, 0);
    setVoxel(3, 0, 0);

    setVoxel(0, 0, 3);
    setVoxel(0, 3, 3);
    setVoxel(3, 3, 3);
    setVoxel(3, 0, 3);

    setVoxel(1, 1, 1);
    setVoxel(1, 2, 1);
    setVoxel(2, 1, 1);
    setVoxel(2, 2, 1);

    setVoxel(1, 1, 2);
    setVoxel(1, 2, 2);
    setVoxel(2, 1, 2);
    setVoxel(2, 2, 2);



    for (int i = 1; i < ledsave / 2; i++) {
        ledOnTime = i;
        delay(3);
    }

    delay(1000);

    clearVoxel(0, 0, 0);
    clearVoxel(0, 3, 0);
    clearVoxel(3, 3, 0);
    clearVoxel(3, 0, 0);

    clearVoxel(0, 0, 3);
    clearVoxel(0, 3, 3);
    clearVoxel(3, 3, 3);
    clearVoxel(3, 0, 3);

    delay(1000);

    for (int i = ledsave / 2; i > 1; i--) {
        ledOnTime = i;
        delay(3);
    }
    clearCube();
    delay(2000);
    ledOnTime = ledsave;

}

void rotateWall(int iterations, int del) {
    clearCube();
    for (int i = 0; i < iterations; i++) {
        fillPlane(0, 1);
        delay(del);
        fillPlane(3, 2);
        delay(del);
        fillPlane(3, 1);
        delay(del);
        fillPlane(0, 2);
        delay(del);
    }
}

// Set n number of voxels at random positions

void effect_random_sparkle_flash(int iterations, int voxels, int del) {
    int i;
    int v;
    for (i = 0; i < iterations; i++) {
        for (v = 0; v <= voxels; v++)
            setVoxel(rand() % 4, rand() % 4, rand() % 4);

        delay(del);
        clearCube();
    }
}

// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.

void effect_random_sparkle(void) {
    int i;

    for (i = 1; i < 15; i++) {
        effect_random_sparkle_flash(5, i, 40);
    }

    for (i = 15; i >= 1; i--) {
        effect_random_sparkle_flash(5, i, 40);
    }

}

void draw_positions_axis(char axis, unsigned char positions[64], int invert) {
    int x, y, p;

    clearCube();

    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            if (invert) {
                p = (3 - positions[(x * 4) + y]);
            } else {
                p = positions[(x * 4) + y];
            }

            if (axis == AXIS_Z)
                setVoxel(x, y, p);

            if (axis == AXIS_Y)
                setVoxel(x, p, y);

            if (axis == AXIS_X)
                setVoxel(p, y, x);
        }
    }

}

void effect_axis_updown_randsuspend(char axis, int del, int sleep, int invert) {
    unsigned char positions[16];
    unsigned char destinations[16];

    int i, px;

    // Set 64 random positions
    for (i = 0; i < 16; i++) {
        positions[i] = 0; // Set all starting positions to 0
        destinations[i] = rand() % 4;
    }

    // Loop 8 times to allow destination 7 to reach all the way
    for (i = 0; i < 4; i++) {
        // For every iteration, move all position one step closer to their destination
        for (px = 0; px < 16; px++) {
            if (positions[px] < destinations[px]) {
                positions[px]++;
            }
        }
        // Draw the positions and take a nap
        draw_positions_axis(axis, positions, invert);
        delay(del);
    }

    // Set all destinations to 7 (opposite from the side they started out)
    for (i = 0; i < 16; i++) {
        destinations[i] = 3;
    }

    // Suspend the positions in mid-air for a while
    delay(sleep);

    // Then do the same thing one more time
    for (i = 0; i < 4; i++) {
        for (px = 0; px < 16; px++) {
            if (positions[px] < destinations[px]) {
                positions[px]++;
            }
            if (positions[px] > destinations[px]) {
                positions[px]--;
            }
        }
        draw_positions_axis(axis, positions, invert);
        delay(del);
    }
}

void effect_wormsqueeze(int size, int axis, int direction, int iterations, int del) {
    int x, y, i, j, k, dx, dy;
    int cube_size;
    int origin = 1;

    if (direction == -1)
        origin = 3;

    cube_size = 4 - (size - 1);

    x = rand() % cube_size;
    y = rand() % cube_size;

    for (i = 0; i < iterations; i++) {
        dx = ((rand() % 3) - 1);
        dy = ((rand() % 3) - 1);

        if ((x + dx) > 0 && (x + dx) < cube_size)
            x += dx;

        if ((y + dy) > 0 && (y + dy) < cube_size)
            y += dy;

        shift(axis, direction);


        for (j = 0; j < size; j++) {
            for (k = 0; k < size; k++) {
                if (axis == AXIS_Z)
                    setVoxel(x + j, y + k, origin);

                if (axis == AXIS_Y)
                    setVoxel(x + j, origin, y + k);

                if (axis == AXIS_X)
                    setVoxel(origin, y + j, x + k);
            }
        }

        delay(del);
    }
}

void effect_running_light_z(int z, int del) {
    for (int i = 0; i < 4; i++) {
        clearCube();
        setVoxel(i, 0, z);
        delay(del);
    }
    for (int i = 1; i < 4; i++) {
        clearCube();
        setVoxel(3, i, z);
        delay(del);
    }
    for (int i = 1; i < 4; i++) {
        clearCube();
        setVoxel(3 - i, 3, z);
        delay(del);
    }
    for (int i = 1; i < 3; i++) {
        clearCube();
        setVoxel(0, 3 - i, z);
        delay(del);
    }
}

void effect_running_up_down(int del) {
    for (int z = 0; z < 4; z++) {
        effect_running_light_z(z, 20);
    }
    for (int z = 1; z < 4; z++) {
        effect_running_light_z(3 - z, 20);
    }

}

// Shows an animation of a spinning spiral

void effect_spiral(int direction, int iterations, int del) {
    int i;
    int z; // cube level

    for (i = 0; i < iterations; i++) {
        // Loop cube levels
        for (z = 0; z < 4; z++) {
            // Read the animation from program memory and put it in the cube buffer.
            // y=0 and y=2 is stored in the upper nibble of the byte, and must be bitshifted into place.
            cube[z][0] = (pgm_read_byte(&spinning_line[(i + z) % 6][0]) >> 4);
            cube[z][1] = pgm_read_byte(&spinning_line[(i + z) % 6][0]);
            cube[z][2] = (pgm_read_byte(&spinning_line[(i + z) % 6][1]) >> 4);
            cube[z][3] = pgm_read_byte(&spinning_line[(i + z) % 6][1]);
            // the +z makes the animation iterate one step for each plane
            // making it into a spiral
        }
        delay(del);
    }
}

// Shows an animation of a spinning plane.

void effect_spinning_plane(int direction, int iterations, int del) {
    int i;
    int z; // cube level

    for (i = 0; i < iterations; i++) {
        // Loop cube levels.
        for (z = 0; z < 4; z++) {
            cube[z][0] = (pgm_read_byte(&spinning_line[(i) % 6][0]) >> 4);
            cube[z][1] = pgm_read_byte(&spinning_line[(i) % 6][0]);
            cube[z][2] = (pgm_read_byte(&spinning_line[(i) % 6][1]) >> 4);
            cube[z][3] = pgm_read_byte(&spinning_line[(i) % 6][1]);
        }
        delay(del);
    }
}


