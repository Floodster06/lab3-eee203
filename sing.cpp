/*
 * A program that causes the 3pi+ bot to emit an increasingly high-pitched
 * frequency based on an increasingly lighter measured grey level.
 *
 * Author : OCdt Flood, OCdt Lee
 * Version : 2025-09-24
 */

#include <Arduino.h>
#include <Pololu3piPlus32U4.h>


using namespace Pololu3piPlus32U4;

OLED display;
ButtonB buttonB;
LineSensors lineSensors;
PololuBuzzer buzzer;


bool start = false; // flag to determine whether program is started or not

/*
 * Reads the greyscale level from the middle light sensor on the robot, and
 * displays it to the center of the screen.
 *
 * Returns: the reflected grey level from the middle sensor
 */
long int see() {
    uint16_t eee[5]; // establishes array of reflectance values

    // reads sensors and populates array of reflectance values
    lineSensors.read(eee);

    display.clear();
    display.gotoXY(7, 3); // centers display
    display.print(eee[2]); // prints the middle sensor value to screen

    return eee[2]; // returns the middle sensor value as a long int
}

/*
 * Displays the required start up screen to the display of the bot.
 */
void introScreen() {
    display.gotoXY(3, 0);
    display.print("Michael Flood");
    display.gotoXY(5, 1);
    display.print("Jeong Lee");
    display.gotoXY(3, 4);
    display.print("Lab 3-2: Sing");
    display.gotoXY(1, 7);
    display.print("To start, press B");

    if (buttonB.isPressed()) { // if button B is pressed set start to true
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

        // determines the played frequency by subtracting the current reading
        // from a set max value (3500) in order to correlate a brighter reading
        // with a higher pitched noise
        int playedFreq = 3500 - see();

        // double the determined frequency and play it for 30ms
        buzzer.playFrequency(playedFreq * 2, 30, 11);
    } else { // until start is true, keep displaying introScreen()
        introScreen();
    }
}
