//
// Created by s31197 on 2025-10-03.
//
float reckonCalibration(uint16_t sensorReadings[5]) {
    display.clear();
    display.gotoXY(3, 3);
    display.print("Ready to measure");
    display.gotoXY(7, 3);
    display.print("Press B");

    //initial calibration
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
            //started calibration, reset counts
            if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 && !
                calCheck) {
                Encoders::getCountsAndResetLeft();
                Encoders::getCountsAndResetRight();
                ++calCheck;
                //finished calibration, break from loop
            } else if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 &&
                       calCheck == 1) {
                ++calCheck;
            }
        }
        //stop, save counts
        Motors::setSpeeds(0, 0);
        int leftCounts = Encoders::getCountsAndResetLeft();
        int rightCounts = Encoders::getCountsAndResetRight();

        float oneCM = ((float) (leftCounts + rightCounts) / 2) / 120;
        return oneCM;
    }
    return 0;
}

void reckon(uint16_t sensorReadings[5]) {
    display.clear();
    display.gotoXY(3, 3);
    display.print("Ready to measure");
    display.gotoXY(7, 4);
    display.print("Press B");
    buttonB.waitForButton();
    //initial calibration
    if (buttonB.isPressed()) {
        //flavour text
        display.clear();
        display.gotoXY(5, 3);
        display.print("Calibrating");
        display.print("\0");


        //0 = hasn't started calibration, approaching calibration start
        //1 = started calibration, still calibrating
        //2 = finished calibration
        int calCheck = 0;

        while (calCheck != 2) {
            follow(sensorReadings);
            //started calibration, reset counts
            if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 && !
                calCheck) {
                Encoders::getCountsAndResetLeft();
                Encoders::getCountsAndResetRight();
                ++calCheck;
                //finished calibration, break from loop
                } else if (sensorReadings[0] == 1000 && sensorReadings[4] == 1000 &&
                           calCheck == 1) {
                    ++calCheck;
                           }
        }
        //stop, save counts
        Motors::setSpeeds(0, 0);
        int leftCounts = Encoders::getCountsAndResetLeft();
        int rightCounts = Encoders::getCountsAndResetRight();

        float oneCM = ((float) (leftCounts + rightCounts) / 2) / 120;


        while (true){
            //set move distance portion of code
            int count = 0;
            while (!buttonB.isPressed()) {
                //common display settings for all distances
                display.clear();
                display.gotoXY(8, 0);
                display.print("Ready");
                display.gotoXY(9, 7);
                display.print("Go");
                display.print("\0");

                //reset count to correct value if value flows outside of intended
                //range
                if (count == 5) {
                    count = 0;
                } else if (count == -1) {
                    count = 4;
                }

                //show different distances depending on count value
                switch (count) {
                    case 0:
                        display.gotoXY(9, 3);
                        display.print("10");
                        break;
                    case 1:
                        display.gotoXY(9, 3);
                        display.print("30");
                        break;
                    case 2:
                        display.gotoXY(9, 3);
                        display.print("60");
                        break;
                    case 3:
                        display.gotoXY(9, 3);
                        display.print("100");
                    case 4:
                        display.gotoXY(9, 3);
                        display.print("200");
                    default:
                        display.print("Something went wrong.");
                }
                //in/decrement count based on if A or B is pressed
                if (buttonC.getSingleDebouncedRelease()) {
                    ++count;
                } else if (buttonA.getSingleDebouncedRelease()) {
                    --count;
                }
            }
            //might need to be a different button b check

            //Actually move now
            display.clear();
            display.gotoXY(7, 3);
            display.print("Driving");
            switch (count) {
                case 0:
                    while (Encoders::getCountsLeft() != (oneCM * 10)) {
                        Motors::setSpeeds(50, 50);
                    }
                    break;
                case 1:
                    while (Encoders::getCountsLeft() != (oneCM * 30)) {
                        Motors::setSpeeds(50, 50);
                    }
                    break;
                case 2:
                    while (Encoders::getCountsLeft() != (oneCM * 60)) {
                        Motors::setSpeeds(50, 50);
                    }
                    break;
                case 3:
                    while (Encoders::getCountsLeft() != (oneCM * 100)) {
                        Motors::setSpeeds(50, 50);
                    }
                    break;
                case 4:
                    while (Encoders::getCountsLeft() != (oneCM * 200)) {
                        Motors::setSpeeds(50, 50);
                    }
                    break;
            }
            //Done
            Motors::setSpeeds(0, 0);
        }
    }
}