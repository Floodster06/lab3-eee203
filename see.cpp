/*
 * A program that where each time the user presses button B, a single raw
 * reading from the centre reflectance sensor is displayed on screen.
 *
 * Author: OCdt Flood, OCdt Lee
 * Version: 2025-09-24
 */

#include <Pololu3piPlus32U4Buttons.h>
#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;

ButtonB buttonB;
OLED display;
LineSensors lineSensors;

bool start = false; // flag to determine whether program is started or not

/*
 * Displays the required start up screen to the display of the bot.
 */
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
        // if button B is pressed set start to true
        start = true;
    }
}

void setup() {
    display.init();
    display.setLayout21x8();
}

void loop() {
    if (start == true) {
        display.clear();

        uint16_t eee[5]; // establishes array of reflectance values

        // reads sensors and populates array of reflectance values
        lineSensors.read(eee);

        display.clear();
        display.gotoXY(7, 3); // centers display
        display.print(eee[2]); // prints the middle sensor value to screen
    } else {
        // until start is true, keep displaying introScreen()

        introScreen();
    }
}
