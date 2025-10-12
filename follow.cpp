/*
 *Makes the robot follow a black line on the floor.
 *
 *Authors: OCdt Lee, OCdt Flood
 *Version: 5-10-2025
 */

#include <Pololu3piPlus32U4.h>
#include <Pololu3piPlus32U4Buttons.h>

using namespace Pololu3piPlus32U4;
OLED display;
LineSensors lineSensors;
bool lost = false;

/*
 *Calibrates the robot sensors so that the readCalibrated() function will work
 *properly.
 */
void calibration() {
    //Delay is so that it starts after we stop touching the robot, making sure
    //that us touching it doesn't fuck up its positioning
    delay(1000);
    display.clear();
    //Prints required message
    display.gotoXY(1, 3);
    display.print("Calibrating Sensors");
    //Turn left to calibrate on white floor
    Motors::setSpeeds(-100, 100);
    delay(185);
    //Move onto white tiles to calibrate lightest color detected
    Motors::setSpeeds(100, 100);
    delay(250);
    Motors::setSpeeds(0, 0);
    lineSensors.calibrate();
    //Go back to starting position
    Motors::setSpeeds(-100, -100);
    delay(250);
    Motors::setSpeeds(0, 0);
    //Turn in place a bunch to calibrate side sensors to dark part of line
    Motors::setSpeeds(-100, 100);
    for (int x = 0; x < 15; x++) {
        lineSensors.calibrate();
        //Delay value here is arbitrary
        delay(46);
    }
    //Extra delay to ensure robot is properly lined up. Number is specifically
    //tuned.
    delay(26);
    Motors::setSpeeds(0, 0);
    display.clear();
}

/*
 *Checks if the robot is lost. Being lost means that the black line isn't sensed
 *
 *sensorReadings: integer array of current readings of the robots' sensors
 *returns: boolean stating if the robot is lost (true) or not (false)
 */
bool checkLost(uint16_t sensorReadings[5]) {
    int sensorLost = 0;
    for (int x = 0; x < 5; x++) {
        if (!(sensorReadings[x])) {
            sensorLost++;
        }
    }
    if (sensorLost == 5) {
        return true;
    }
    return false;
    
}

/*
 *Checks if the robot has hit a T-intersection.
 *
 *sensorReadings: integer array of current readings of the robots' sensors
 *returns: boolean stating if the robot has hit a T-intersection (true) or not
 *(false)
 */
bool checkT(uint16_t sensorReadings[5]) {
    int tcount = 0;
    //Check all sensor readings to see if they've detected black
    for (int x = 0; x < 5; x++) {
        if (sensorReadings[x] == 1000) {
            tcount++;
        }
    }
    //If all sensors detect black, return true
    if (tcount == 5) {
        return true;
    }
    return false;
}

/*
 *Makes the robot follow a black line on the floor.
 */
void follow() {
    uint16_t sensorReadings[5];

    lineSensors.readCalibrated(sensorReadings);

    //Turn left if too far right
    if (sensorReadings[1] > sensorReadings[3]) {
        Motors::setSpeeds(25, 70);
    //Turn right if too far left
    } else if (sensorReadings[1] < sensorReadings[3]) {
        Motors::setSpeeds(70, 25);
    //Go straight if no issues
    } else {
        Motors::setSpeeds(50, 50);
    }

    //Turn around and play a note if hit a T-intersection
    if (checkT(sensorReadings)) {
        Buzzer::playNote(NOTE_A(4), 1000, 10);
        Motors::setSpeeds(-100, 100);
        delay(370);
        Motors::setSpeeds(0, 0);
    }

    //Stop in place and play a note if lose
    if (checkLost(sensorReadings)) {
        lost = true;
        Motors::setSpeeds(0, 0);
        display.clear();
        display.gotoXY(7, 3);
        display.print("I'm Lost!");
        Buzzer::playNote(NOTE_F(4), 1000, 10);
        delay(5000);
        display.clear();
    }
}
