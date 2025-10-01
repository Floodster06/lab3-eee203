/*
 *Description: Make the robot move as described in lab 2 IOT prepare us for
 *future projects
 *Author: OCdt Flood, OCdt Lee
 *Version: 10-09-2025
*/

#include <Pololu3piPlus32U4Buttons.h>
#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;
ButtonB buttonB;
OLED display;
LineSensors lineSensors;
bool start = false;
bool lost = false;

//TODO: COMMENT SHIT OUT
//TODO: FORMAT CODE PROPERLY
/*
 *Initialize the robot, resetting the display back to its default state
 */
void setup() {
    display.init();
    display.setLayout21x8();
}

/*
 *Calibrate the robots' sensors
 */
void calibration() {
    //Delay is so that it starts after we stop touching the robot, making sure
    //that us touching it doesn't fuck up its positioning
    delay(1000);
    display.clear();
    //Prints required message
    display.gotoXY(1,3);
    display.print("Calibrating Sensors");
    //Turn left to calibrate on white floor
    Motors::setSpeeds(-100, 100);
    delay(185);
    Motors::setSpeeds(100, 100);
    delay(250);
    Motors::setSpeeds(0, 0);
    lineSensors.calibrate();
    //Go back to starting position
    Motors::setSpeeds(-100, -100);
    delay(250);
    Motors::setSpeeds(0,0);
    //Turn in place to calibrate side sensors to dark part of line
    Motors::setSpeeds(-100, 100);
    for (int x = 0; x<15; x++) {
        lineSensors.calibrate();
        delay(46);
    }
    //Extra delay to ensure robot is properly lined up
    delay(26);
    Motors::setSpeeds(0, 0);
    display.clear();
}

void printSensor(uint16_t sensorReadings[5]) {
    display.clear();
    for (int x=0; x<5; x++) {
        display.print(sensorReadings[x]);
        display.gotoXY(0,x+1);
    }
}

bool checkLost(uint16_t sensorReadings[5]) {
    int sensorLost = 0;
    for (int x=0; x<5; x++) {
        if (!(sensorReadings[x])) {
            sensorLost++;
        }
    }
    if (sensorLost == 5) {
        return true;
    }
    else {
        return false;
    }

}

bool checkT(uint16_t sensorReadings[5]) {
    int tcount =0;
    for (int x = 0; x<5; x++) {
        if (sensorReadings[x] == 1000) {
            tcount++;
        }
    }
    if (tcount == 5) {
        return true;
    }
    return false;
}

long int see() {

    uint16_t eee[5];
    lineSensors.read(eee);
    display.clear();
    display.gotoXY(7,3);
    display.print(eee[2]);

    return eee[2];

}

void sing() {
    display.clear();

    int playedFreq = 3500 - see();

    Buzzer::playFrequency(playedFreq * 2, 30, 11);
}

void follow() {
    //TODO: still needs some fine tuning on some of the harder turns
    uint16_t sensorReadings[5];

    lineSensors.readCalibrated(sensorReadings);

    if (sensorReadings[1] > sensorReadings[3]) {
        Motors::setSpeeds(25, 70);
    }
    else if (sensorReadings[1] < sensorReadings[3]) {
        Motors::setSpeeds(70, 25);
    }
    else {
        Motors::setSpeeds(40, 40);
    }

//TODO: Below is an attempt to implement proportional steering. It doesn't work
/*
    int error = abs(sensorReadings[1] - sensorReadings[3])/5000;
    if (sensorReadings[1] > sensorReadings[3]) {
        Motors::setSpeeds(40*error, 40);
    }
    else if (sensorReadings[1] < sensorReadings[3]) {
        Motors::setSpeeds(40, 40*error);
    }
    else {
        Motors::setSpeeds(40, 40);
    }
*/


    if (checkT(sensorReadings)) {
        Buzzer::playNote(NOTE_A(4), 1000, 10);
        Motors::setSpeeds(-100, 100);
        delay(370);
        Motors::setSpeeds(0, 0);
    }



    if (checkLost(sensorReadings)) {
        lost = true;
        Motors::setSpeeds(0, 0);
        display.clear();
        display.gotoXY(7,3);
        display.print("I'm Lost!");
        Buzzer::playNote(NOTE_F(4),1000, 10);
        delay(5000);
        display.clear();
    }




}

void introScreen() {
    display.gotoXY(3, 0);
    display.print("Michael Flood");
    display.gotoXY(5, 1);
    display.print("Jeong Lee");
    display.gotoXY(3, 4);
    display.print("Lab 3-1: See");
    display.gotoXY(1, 7);
    display.print("To start, press B");

    if (buttonB.isPressed()) {
        start = true;
        lost = false;
        calibration();
    }
}

void loop() {
    if (start == true && !lost){
        follow();

    } else {
        introScreen();
    }

}