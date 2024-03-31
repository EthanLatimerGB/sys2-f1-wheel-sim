#!/usr/bin/env python3

import threading
import time
import math
import curses
import tkinter as tk


"""
Module Docstring
"""
ENGINE_REDLINE = 15,000

root = tk.Tk()

ECU_CLOCK = time.perf_counter()

ECU_CAR_CLOCK = tk.IntVar(root, ECU_CLOCK)
ECU_ENGINE_SPEED = tk.DoubleVar(root, 3500.00)
ECU_CAR_SPEED = tk.DoubleVar(root, 0.00)
ECU_CAR_GEAR = tk.IntVar(root, 8)
ECU_CAR_THROTTLE = tk.DoubleVar(root, 1.00)
ECU_CAR_BRAKE = tk.DoubleVar(root, 0.00)

current_command = ""
trigger_input = False

__author__ = "Ethan"
__license__ = "MIT"

##
## Handle Commands
##

def cursesCommandLine(stdscr):
    stdscr.clear()

    stdscr.addstr(0, 0, "This is the car simulation command line")
    stdscr.addstr(1, 0, "Enter commands to change the behaviour of the car, type HELP for the list of commands")
    stdscr.refresh()

    while (current_command == "EXIT"):
        

    exit()

    



# ** Commands List ** 
# THROTTLE <INT>
# BRAKE <INT>
# POWER <Bool>
def handle_commands():
    global current_command
    if current_command == "":
        return

    tokens = current_command.split(" ")
    match tokens[0]:
        case "THROTTLE":
            if(tokens[1] > 0 and tokens[1] <= 100):
                ECU_CAR_THROTTLE.set(tokens[1] / 100)


##
## Car Simulation
##

def simulateCar():
    while True:
        global ECU_CAR_CLOCK
        global ECU_ENGINE_SPEED
        global ECU_CAR_GEAR
        global ECU_CAR_SPEED
        global ECU_CLOCK

        # handle_commands()
        oldTime = ECU_CLOCK
        ECU_CLOCK = time.perf_counter()

        timeDiff = ECU_CLOCK - oldTime

        ECU_CAR_CLOCK.set(ECU_CLOCK)


        if (ECU_ENGINE_SPEED.get() < 15000):
            ECU_ENGINE_SPEED.set( round( engine_speed_model(ECU_ENGINE_SPEED.get(), ECU_CAR_GEAR.get(), 1, timeDiff), 1))
            ECU_CAR_SPEED.set( round( getSpeedOfCar(ECU_CAR_GEAR.get(), ECU_ENGINE_SPEED.get()) , 1) )

        time.sleep( 1 / 15 )

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


##
## Handle GUI Components
##

def buildGUI():
    root.title("Live data")
    root.geometry("400x150")

    frame = tk.Frame(root)

    # All labels
    clock_header_label = tk.Label(frame, text="CLOCK: ")
    enginespeed_header_label = tk.Label(frame, text="ENGINE SPEED: ")
    carspeed_header_label = tk.Label(frame, text="CAR_SPEED: ")
    gear_header_label = tk.Label(frame, text="CAR_GEAR: ")
    throttle_header_label = tk.Label(frame, text="CAR_THROTTLE_PERCENTAGE: ")
    brake_header_label = tk.Label(frame, text="CAR_BRAKE_PERCENTAGE")

    clock_header_label.grid(row=0, column=0) 
    enginespeed_header_label.grid(row=1, column=0)
    carspeed_header_label.grid(row=2, column=0)
    gear_header_label.grid(row=3, column=0)
    throttle_header_label.grid(row=4, column=0)

    enginespeed_label   = tk.Label(frame, textvariable = ECU_ENGINE_SPEED)
    carspeed_label      = tk.Label(frame, textvariable = ECU_CAR_SPEED)
    clock_label         = tk.Label(frame, textvariable = ECU_CAR_CLOCK)
    gear_label          = tk.Label(frame, textvariable = ECU_CAR_GEAR)
    throttle_label      = tk.Label(frame, textvariable = ECU_CAR_THROTTLE)
        
    clock_label.grid(row=0, column=1) 
    enginespeed_label.grid(row=1, column=1)
    carspeed_label.grid(row=2, column=1)
    gear_label.grid(row=3, column=1)
    throttle_label.grid(row=4, column=1)

    frame.pack()
    root.mainloop()

def main(s):
    """ Main entry point of the app """

    carsim_thread = threading.Thread(target=simulateCar)
    carsim_thread_daemon = True

    curses_thread = threading.Thread(target=cursesCommandLine)
    curses_thread_daemon = True

    carsim_thread.start()
    curses_thread.start()

    buildGUI()

if __name__ == "__main__":
    wrapper(main)

