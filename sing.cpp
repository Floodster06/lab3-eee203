#include <Arduino.h>
#include <Pololu3piPlus32U4.h>


using namespace Pololu3piPlus32U4;

OLED display;
ButtonB buttonB;
LineSensors lineSensors;


bool start = false;

long int see() {

    uint16_t eee[5];

    lineSensors.read(eee);

    display.clear();
    display.gotoXY(7,3);
    display.print(eee[2]);

    return eee[2];


}

void introScreen() {

    display.gotoXY(3,0);
    display.print("Michael Flood");
    display.gotoXY(5,1);
    display.print("Jeong Lee");
    display.gotoXY(3,4);
    display.print("Lab 3-2: Sing");
    display.gotoXY(1,7);
    display.print("To start, press B");

    if (buttonB.isPressed()) {

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

        int playedFreq = 3500 - see();

        Buzzer::playFrequency(playedFreq * 2, 30, 11);

    } else {

        introScreen();
    }


}
