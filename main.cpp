/* 
 * Software to control a 4x4x4 LED cube with Arduino
 * 
 * 
 * I used lots of code from Christian Moen (christian@lynet.no) 
 * and Ståle Kristoffersen (staalekb@ifi.uio.no)
 * 
 * However, I connected the LEDs differently (shift regs, SPI etc)
 * so at least the IRQ routine and most of the stuff in main.cpp
 * was done by me.
 * 
 * License: http://creativecommons.org/licenses/by-nc-sa/3.0/
 * 
 * 
 * 2011, Matthias Wientapper
 *  
 */


#include <Arduino.h>
#include <SPI.h>
#include <TimerOne.h>

#include "string.h"

#include "cube.h"
#include "draw.h"
#include "effect.h"


// SPI interface on Arduino:
// 10 (SS), 11 (MOSI), 12 (MISO), and 13 (SCK).

//Pin connected to ST_CP of 74HC594
int latchPin = 8;
//Pin connected to SH_CP of 74HC594
int clockPin = 13;
////Pin connected to DS of 74HC594
int dataPin = 11;

// layer switching pins, driving npn transistors
// to switch common cathodes to ground
// (grounded emitter circuit)
int z3Pin = 5;
int z2Pin = 4;
int z1Pin = 3;
int z0Pin = 2;

byte cubeactive = 1;

// --------- Ctrl stuff -----------

void display(void) {
    byte z, plow, phigh;

    PORTD &= B11000011;

    // go through all layers
    for (z = 0; z < 4; z++) {

        // fill layer with voxels
        plow = cube[z][1] | (cube[z][0] << 4);
        phigh = cube[z][3] | (cube[z][2] << 4);

        SPI.transfer(plow);
        SPI.transfer(phigh);
        // If not using SPI:        
        //        shiftOut(dataPin, clockPin, MSBFIRST, plow);
        //        shiftOut(dataPin, clockPin, MSBFIRST, phigh);

        digitalWrite(latchPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(latchPin, LOW);

        // switch on layer for specific time
        PORTD |= (1 << (z + z0Pin));
        delayMicroseconds(ledOnTime);
        PORTD &= B11000011;
    }
}

void procSerial(void) {

    char c;
    while (Serial.available()) {
        delay(300);
        c = Serial.read();
        switch (c) {
            case '0':
                clearCube();
                cubeactive = 0;
                Serial.println("Cube stopped.");
                break;
            case '1':
                cubeactive = 1;
                Serial.println("Cube resumed.");
                break;
            default:
                break;
        }
    }
}

void setup() {

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(10, OUTPUT); //  for libSPI to work properly

    pinMode(z3Pin, OUTPUT);
    pinMode(z2Pin, OUTPUT);
    pinMode(z1Pin, OUTPUT);
    pinMode(z0Pin, OUTPUT);

    digitalWrite(z3Pin, LOW);
    digitalWrite(z2Pin, LOW);
    digitalWrite(z1Pin, LOW);
    digitalWrite(z0Pin, LOW);

    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    // clear shift regs
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
    shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);

    Serial.begin(9600);
    Serial.print("Init...");

    ledOnTime = 2000;
    Timer1.initialize(15000);
    Timer1.attachInterrupt(display);

    clearCube();
    Serial.println("Done.");
}

void play_effect(int effect) {
    // the show
    int ledsave;

    switch (effect) {
        case 0:
            clearCube();
            delay(1000);
            break;

        case 1:
            starBox();
            clearCube();
            delay(500);
            break;

        case 2:
            rndFill();
            clearCube();
            delay(500);

        case 3:
            effectRain(100);
            clearCube();
            delay(500);
            break;

        case 4:
            sendplane_rand_z(0, 20, 80);
            sendvoxels_rand_z(50, 20, 100);
            clearCube();
            delay(500);
            break;

        case 5:
            effect_axis_updown_randsuspend(AXIS_X, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_X, 50, 500, 1);

            effect_axis_updown_randsuspend(AXIS_X, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_X, 50, 500, 1);

            effect_axis_updown_randsuspend(AXIS_Z, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_Z, 50, 500, 1);

            effect_axis_updown_randsuspend(AXIS_Z, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_Z, 50, 500, 1);


            effect_axis_updown_randsuspend(AXIS_Y, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_Y, 50, 500, 1);

            effect_axis_updown_randsuspend(AXIS_Y, 50, 500, 0);
            effect_axis_updown_randsuspend(AXIS_Y, 50, 500, 1);

            clearCube();
            delay(500);
            break;

        case 6:
            effect_random_sparkle();
            clearCube();
            delay(500);
            break;
        case 7:
            effect_wormsqueeze(1, AXIS_Z, -1, 100, 50);
            clearCube();
            delay(500);
            break;

        case 8:
            effect_spinning_plane(1, 50, 50);
            break;

        case 9:
            effect_spiral(1, 75, 50);
            break;

        case 10:
            sweepPlanes();
            sweepPlanes();
            break;

        case 11:
            effect_running_up_down(20);
            break;

        case 12:
            effect_box_shrink_grow(30, 200);
            break;

        case 13:
            rotateWall(10, 60);
            break;

        case 14:
            fillCube();
            delay(2000);
            ledsave = ledOnTime;
            for (int i = ledsave; i > 1; i--) {
                ledOnTime = i;
                delay(2);
            }
            clearCube();
            ledOnTime = ledsave;
            break;
    }
}

void loop(void) {

    for (int i = 0; i < 15; i++) {
        procSerial();
        if (cubeactive) {
            play_effect(i);
        }
    }
    procSerial();
}





