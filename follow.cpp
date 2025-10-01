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
