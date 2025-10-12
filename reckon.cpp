/*
 *Drive the robot forward a user-selected distance, using information gathered
 *in a distance calibration step to determine when to stop.
 *
 *Authors: OCdt Lee, OCdt Flood
 *Version: 12-10-2025
 */

#include <Pololu3piPlus32U4Buttons.h>
#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;
ButtonA buttonA;
ButtonB buttonB;
ButtonC buttonC;
OLED display;
LineSensors lineSensors;

/*
 *Pause all further code, until any initalized button has been pressed. Once
 *pressed, return an int corresponding to the button pressed. 1 = button A,
 *2 = button B, 3 = button C, and so on.
 *
 *returns: int from 1-3 corresponding to button pressed
 */
int waitAnyPress() {
    int pressed = 0;
    do {
        if (buttonA.getSingleDebouncedRelease()) {
            pressed = 1;
        } else if (buttonB.getSingleDebouncedRelease()) {
            pressed = 2;
        } else if (buttonC.getSingleDebouncedRelease()) {
            pressed = 3;
        }
        delay(10);
    } while (pressed == 0);
    return pressed;
}

/*
 *Drives forward a user-selected distance, using information gathered in a
 *distance calibration step to determine when to stop
 */
void reckon(uint16_t sensorReadings[5]) {
    //Initial display setup
    display.clear();
    display.gotoXY(3, 3);
    display.print("Ready to measure");
    display.gotoXY(7, 4);
    display.print("Press B");

    buttonB.waitForPress();

    //Initial distance calibration step
    if (buttonB.isPressed()) {
        //flavour text
        display.clear();
        display.gotoXY(5, 3);
        display.print("Calibrating");

        //0 = hasn't started calibration, approaching calibration start
        //1 = started calibration, still calibrating
        //2 = finished calibration
        int calCheck = 0;
        while (calCheck != 2) {
            follow(sensorReadings);
            //started calibration, reset encoder values for accuracy
            if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 &&
                calCheck != 1) {
                Encoders::getCountsAndResetLeft();
                Encoders::getCountsAndResetRight();
                calCheck = 1;
                //Delay so that robot passes the first black lines and doesn't
                //get stuck. Value is arbitrary and thus doesn't need
                //declaration as a constant
                delay(500);
                //Finished calibration, break from loop
            } else if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 &&
                       calCheck == 1) {
                calCheck = 2;
            }
        }
    }
    //Stop movement, save encoder counts
    Motors::setSpeeds(0, 0);
    int leftCounts = Encoders::getCountsAndResetLeft();
    int rightCounts = Encoders::getCountsAndResetRight();
    //get the encoder value equivalent of 1 centimeter
    int oneCM = ((leftCounts + rightCounts) / 2) / 30;

    //Distance screen & movement part of code. While true is necessary as per
    //lab instructions.
    while (true) {
        //Setting the move distance portion of code
        //count is used as an index to represent which distance is selected.
        int count = 0;
        int distances[] = {10, 30, 60, 100, 200};
        //press represents which button has been pressed. See waitAnyPress
        //function for more details
        int press = 0;
        //Changing distances displayed on the screen while buttonB hasn't been
        //pressed.
        while (press != 2) {
            //common display settings for all distances
            display.clear();
            display.gotoXY(8, 0);
            display.print("Ready");
            display.gotoXY(9, 7);
            display.print("Go");
            display.print("\0");

            //reset count to correct value if value flows outside of
            //intended range
            if (count > 4) {
                count = 0;
            } else if (count < 0) {
                count = 4;
            }

            //show different distances depending on count value
            display.gotoXY(9, 3);
            display.print(distances[count]);

            press = waitAnyPress();
            //in or decrement count based on if A or B is pressed
            if (press == 3) {
                ++count;
            } else if (press == 1) {
                --count;
            }
        }

        //Actually move now
        //Flavour text
        display.clear();
        display.gotoXY(7, 3);
        display.print("Driving");
        display.gotoXY(9, 7);
        display.print(oneCM);
        //Reset encoders
        Encoders::getCountsAndResetLeft();
        Encoders::getCountsAndResetRight();
        //int representing how far robot has moved
        int moveDistance = 0;
        //I LOVE SWITCH STATEMENTS RAHHHHHHHHH
        //each case represents a different distance, no base case needed.
        switch (count) {
            case 0:
                while (moveDistance != oneCM * distances[count]) {
                    moveDistance = Encoders::getCountsLeft();
                    Motors::setSpeeds(50, 50);
                }
                break;
            case 1:
                while (moveDistance != oneCM * distances[count]) {
                    moveDistance = Encoders::getCountsLeft();
                    Motors::setSpeeds(50, 50);
                }
                break;
            case 2:
                while (moveDistance != oneCM * distances[count]) {
                    moveDistance = Encoders::getCountsLeft();
                    Motors::setSpeeds(50, 50);
                }
                break;
            case 3:
                while (moveDistance != oneCM * distances[count]) {
                    moveDistance = Encoders::getCountsLeft();
                    Motors::setSpeeds(50, 50);
                }
                break;
            case 4:
                while (moveDistance != oneCM * distances[count]) {
                    moveDistance = Encoders::getCountsLeft();
                    Motors::setSpeeds(50, 50);
                }
                break;
        }
        //Done moving
        Motors::setSpeeds(0, 0);
    }
}
