void follow() {
    uint16_t sensorReadings[5];
    //TODO: fix calibration by having it turn, go forward, take a reading, go back to start position
    lineSensors.setTimeout(3000);
    //Motors::setSpeeds(100,100); //Initial move????
    while (true) {

        /* THIS IS OLD CODE
        lineSensors.readCalibrated(sensorReadings);
        //TODO: fix the <= values after the sensorReadings[x] to better values that actually work
        if ((sensorReadings[1] >=850) && (sensorReadings[3] >=850)) {
            //make a sound and turn
            //TODO: make sound
            Motors::setSpeeds(-100, 100);
            delay(400);
        }
        if (sensorReadings[1] <=850) {
            Motors::setSpeeds(110,90);
        }
        //turn left
        else if (sensorReadings[3] <=850) {
            Motors::setSpeeds(90,110);
        }
        else {
            Motors::setSpeeds(100,100);
        }
        */

        lineSensors.readCalibrated(sensorReadings);
        int difference = (short)sensorReadings[3] - (short)sensorReadings[1];
        display.print(difference);
        if (difference >= 50) {
            Motors::setSpeeds(-100, 100); //TODO: fix values
        }
        else if (difference < 50) {
            Motors::setSpeeds(0, 100); //TODO: fix values
        }
        else {
            Motors::setSpeeds(100, 100);

        }

        /*
         *compare two value readings from left and right sensors, then
         *add/multiply either left or right motor to turn that direction by a
         *certain value based on the difference in reading. Otherwise, I want
         *to have the robot travel straight
         */





        //if off track, stop
        if (sensorReadings[2] <= 100) {
            //make a sound, stop, display message
            Buzzer::playNote(NOTE_C(1), 100, 5);
            Motors::setSpeeds(0, 0);
            display.clear();
            display.print("Help me.");
            break;
        }

    }

}//
// Created by s31197 on 2025-09-24.
//
