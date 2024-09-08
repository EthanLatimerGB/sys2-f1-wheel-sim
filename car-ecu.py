#!/usr/bin/env python3

import src.simulator
import src.terminal_curses as cur
import src.globals as g
import src.uart as comm

import threading
import tkinter as tk
import serial
from curses import wrapper

"""
Module Docstring
"""
__author__ = "Ethan"
__license__ = "MIT"


# Main entry point of the app
def main(stdscr):
    carsim_thread = threading.Thread(target=lambda: src.simulator.simulateCar())
    carsim_thread_daemon = True

    curses_thread = threading.Thread(target=cur.cursesCommandLine, args=(stdscr,))
    curses_thread_daemon = True

    # pico_listener_thread = threading.Thread(target=comm.listen)

    carsim_thread.start()
    curses_thread.start()
    # pico_listener_thread.start()

    cur.buildGUI()

    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = "/dev/ttyACM1"
    ser.timeout = 1

    cur.printToCurses("deez")

    # while True:
    #     cur.history.append("bum")
    #     if ser.in_waiting > 0:
    #         data = ser.readline().decode("utf-8").strip()
    #         with cur.history_lock:
    #             cur.history.append(data)


if __name__ == "__main__":
    wrapper(main)
