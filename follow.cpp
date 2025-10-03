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

