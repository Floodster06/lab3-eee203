/*
 *Description: Make the robot perform the actions required of it as detailed in
 *lab 3 section 1
 *Author: OCdt Flood, OCdt Lee
 *Version: 17-09-2025
 */

#include <Pololu3piPlus32U4Buttons.h>
#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;
ButtonB buttonB;
OLED display;
LineSensors lineSensors;

/*
 *Print intro screen and wait until button B is pressed. Once pressed, take a
 *reading using the line sensors, then print the middle sensors' reading on the
 *screen.
 */
void see() {
    uint16_t sensorReadings[5];
    display.gotoXY(3, 0);
    display.print("Michael Flood");
    display.gotoXY(5, 1);
    display.print("Jeong Lee");
    display.gotoXY(3, 4);
    display.print("Lab 3-1: See");
    display.gotoXY(1, 7);
    display.print("To start, press B");
    if (buttonB.isPressed()) {
        lineSensors.read(sensorReadings);
        display.clear();
        display.print(sensorReadings[2]);
    }
}
