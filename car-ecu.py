#!/usr/bin/env python3

import threading
import time
import math
import curses
import tkinter as tk
import curses
from curses import wrapper

"""
Module Docstring
"""
root = tk.Tk()

# Global Variables 
ENGINE_REDLINE = 15,000
ECU_CLOCK = time.perf_counter()

ECU_CAR_CLOCK = tk.IntVar(root, ECU_CLOCK)
ECU_ENGINE_SPEED = tk.DoubleVar(root, 3500.00)
ECU_CAR_SPEED = tk.DoubleVar(root, 0.00)
ECU_CAR_GEAR = tk.IntVar(root, 1)
ECU_CAR_THROTTLE = tk.DoubleVar(root, 0.0)
ECU_CAR_BRAKE = tk.DoubleVar(root, 0.00)

# This is speed lost per second (linear)
sim_air_resistance = -5
sim_peak_brake_power = -60

request_gear_change = 0

__author__ = "Ethan"
__license__ = "MIT"
exit_flag = False

##
## Handle Commands
##

def cursesCommandLine(stdscr):
    global current_command

    stdscr.clear()
    stdscr.addstr(0, 0, "This is the car simulation command line")
    stdscr.addstr(1, 0, "Enter commands to change the behaviour of the car, type HELP for the list of commands")
    curses.echo()
    
    stdscr.refresh()

    while (not exit_flag):
        stdscr.addstr(curses.LINES - 1, 0, "> ")
        current_command = stdscr.getstr(curses.LINES - 1, 3).decode("utf-8")
        stdscr.clrtoeol()
        stdscr.refresh()
        handle_commands(stdscr)

# ** Commands List ** 
# THROTTLE <INT>
# BRAKE <INT>
# POWER <Bool>
def handle_commands(stdscr):
    global root
    global exit_flag
    global request_gear_change

    if current_command == "":
        return

    tokens = current_command.split(" ")
    match tokens[0].upper():
        case "THROTTLE":
            value = int(tokens[1])
            if(value >= 0 and value <= 100):
                ECU_CAR_THROTTLE.set(value / 100)
                stdscr.addstr(curses.LINES - 2, 0, "Throttle open: {}".format(ECU_CAR_THROTTLE.get()))
                stdscr.refresh()
        case "GEAR":
            gearNumber = int(tokens[1])
            if (gearNumber < 9 and gearNumber > 0):
                request_gear_change = gearNumber
                stdscr.addstr(curses.LINES - 2, 0, "Changed gear to: {}".format(gearNumber))
        case "BRAKE":
            value = int(tokens[1])
            if(value >= 0 and value <= 100):
                ECU_CAR_BRAKE.set(value / 100)
                stdscr.addstr(curses.LINES - 2, 0, "Brake pressure: {}".format(ECU_CAR_BRAKE.get()))
        case "QUIT":
            root.quit()
            exit()


##
## Car Simulation
##

def simulateCar():
    while not exit_flag:
        global ECU_CAR_CLOCK
        global ECU_ENGINE_SPEED
        global ECU_CAR_GEAR
        global ECU_CAR_SPEED
        global ECU_CLOCK
        global sim_air_resistance
        global sim_peak_brake_power
        
        # Handle gear changes
        if (request_gear_change > 0):
            changeGear()

        # Calculate time deltas 
        oldTime = ECU_CLOCK
        ECU_CLOCK = time.perf_counter()
        timeDiff = ECU_CLOCK - oldTime
        ECU_CAR_CLOCK.set(ECU_CLOCK)

        # Calculate engine and car speed without drag
        if (ECU_ENGINE_SPEED.get() < 15000):
            ECU_ENGINE_SPEED.set( round( engine_speed_model(ECU_ENGINE_SPEED.get(), ECU_CAR_GEAR.get(), ECU_CAR_THROTTLE.get(), timeDiff), 1))
            ECU_CAR_SPEED.set( round( getSpeedOfCar(ECU_CAR_GEAR.get(), ECU_ENGINE_SPEED.get()) , 1) )

        # Factor in drag
        if (ECU_ENGINE_SPEED.get() > 0):
            speedsFactoringInDrag(timeDiff)
        time.sleep( 1 / 60 )

# Does the math and updates the rev counter to its new speed, relative to the new gear change
def changeGear():
    global request_gear_change, ECU_CAR_GEAR, ECU_ENGINE_SPEED

    oldGear = ECU_CAR_GEAR.get()
    newGear = request_gear_change

    speed = getSpeedOfCar(oldGear, ECU_ENGINE_SPEED.get())
    
    # inverse of getSpeedOfCar
    newEngineRPM = getRPMfromCarSpeed(speed, newGear)
    ECU_ENGINE_SPEED.set(newEngineRPM)
    ECU_CAR_GEAR.set(newGear)
    request_gear_change = 0

# Returns spedd in km/h
def getSpeedOfCar(currentGear, engineRPM):
    constant = 110 + ((currentGear - 1) * 35)
    return constant * (engineRPM / 15000)

def getRPMfromCarSpeed(speed, gear):
    return ( 15000 * speed ) / (110 + ( (gear - 1) * 35 ) )


# Returns the new engine speed after a certain period of time.
# Puts on model of a time accellerating over engine speed
def engine_speed_model(currentSpeed, gear, throttle, timeDelta):
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
    
    relativeTimeToModel = math.sqrt( currentSpeed ) / constant
    newSpeed = (constant * (relativeTimeToModel + timeDelta)) ** 2

    speedGain = (newSpeed - currentSpeed) * throttle
    return currentSpeed + speedGain

def speedsFactoringInDrag(timeDelta):
    global ECU_CAR_BRAKE, ECU_CAR_GEAR, ECU_CAR_SPEED, ECU_ENGINE_SPEED, sim_peak_brake_power

    speedLostDueToResistance = timeDelta * ((sim_air_resistance * (ECU_CAR_SPEED.get() / 150)) + (ECU_CAR_BRAKE.get() * sim_peak_brake_power))

    if((ECU_CAR_SPEED.get() + speedLostDueToResistance) < 0):
        return

    ECU_CAR_SPEED.set( round ( (ECU_CAR_SPEED.get() + speedLostDueToResistance), 1) )
    ECU_ENGINE_SPEED.set( round( (getRPMfromCarSpeed( ECU_CAR_SPEED.get(), ECU_CAR_GEAR.get())), 1) ) 

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

def main(stdscr):
    """ Main entry point of the app """

    carsim_thread = threading.Thread(target=simulateCar)
    carsim_thread_daemon = True

    curses_thread = threading.Thread(target=cursesCommandLine, args=(stdscr,))
    curses_thread_daemon = True

    carsim_thread.start()
    curses_thread.start()

    buildGUI()

if __name__ == "__main__":
    wrapper(main)

