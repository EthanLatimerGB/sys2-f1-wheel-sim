#!/usr/bin/env python3

import threading
import time
import matplotlib.pyplot as plt
import numpy as np
import math

plt.style.use('_mpl-gallery')

"""
Module Docstring
"""
ENGINE_REDLINE = 15,000

ECU_CAR_CLOCK = time.perf_counter()
ECU_ENGINE_SPEED = 3500 
ECU_CAR_SPEED = 0
ECU_CAR_GEAR = 8
ECU_CAR_THROTTLE = 1
ECU_CAR_BRAKE = 0

x = []
y = []

__author__ = "Ethan"
__license__ = "MIT"

def updateECU():
    while True:
        print ("Clock: " , ECU_CAR_CLOCK)
        print ("Engine Speed: " , ECU_ENGINE_SPEED)
        print ("Car Speed: " , ECU_CAR_SPEED)
        print ("Gear: " , ECU_CAR_GEAR)
        print ("Throttle: " , ECU_CAR_THROTTLE)
        print ("Brake: " , ECU_CAR_BRAKE)
        time.sleep(0.03)

def simulateCar():
    while True:
        global ECU_CAR_CLOCK
        global ECU_ENGINE_SPEED
        global ECU_CAR_GEAR
        global ECU_CAR_SPEED
        
        oldTime = ECU_CAR_CLOCK 
        ECU_CAR_CLOCK = time.perf_counter()

        timeDiff = ECU_CAR_CLOCK - oldTime
        if (ECU_ENGINE_SPEED > 15000):
            break;


        ECU_ENGINE_SPEED = engine_speed_model(ECU_ENGINE_SPEED, ECU_CAR_GEAR, 1, timeDiff)
        ECU_CAR_SPEED = getSpeedOfCar(ECU_CAR_GEAR, ECU_ENGINE_SPEED)
        
        x.append(ECU_CAR_CLOCK)
        y.append(ECU_ENGINE_SPEED)

        time.sleep(0.03)

# Returns spedd in km/h
def getSpeedOfCar(currentGear, engineRPM):
    constant = 110 + ((currentGear - 1) * 35)

    return constant * (engineRPM / 15000)



# Returns the new engine speed after a certain period of time.
# Puts on model of a time accellerating over engine speed
def engine_speed_model(currentSpeed, gear, throttle, timeelapsed):
    constant = 0
    match gear:
        case 1:
            constant=35
        case 2:
            constant=25
        case 3:
            constant=20
        case 4:
            constant=14
        case 5:
            constant=10
        case 6:
            constant=6
        case 7:
            constant=4
        case 8:
            constant=2
        case _:
            constant=0
    currentSpeedRelTime = math.sqrt(currentSpeed) / constant
    newTimeAcc = currentSpeedRelTime + timeelapsed

    return (constant * newTimeAcc * throttle) ** 2

def main():
    """ Main entry point of the app """
    print_thread = threading.Thread(target=updateECU)
    print_thread_daemon = True

    carsim_thread = threading.Thread(target=simulateCar)
    print_thread_daemon = True

    carsim_thread.start()

    print_thread.start()

    time.sleep (5)
    fig, ax = plt.subplots()
    ax.plot(x, y, linewidth=2.0)
    plt.show()



if __name__ == "__main__":
    """ This is executed when run from the command line """
    main()


