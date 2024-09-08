#!/usr/bin/env python3

import src.simulator
import src.terminal_curses as cur
import src.globals as g

import threading
import tkinter as tk
import serial
from curses import wrapper

"""
Module Docstring
"""
__author__ = "Ethan"
__license__ = "MIT"


def main(stdscr):
    # Main entry point of the app

    carsim_thread = threading.Thread(target=lambda: src.simulator.simulateCar())
    carsim_thread_daemon = True

    curses_thread = threading.Thread(target=cur.cursesCommandLine, args=(stdscr,))
    curses_thread_daemon = True

    # pico_listener_thread = threading.Thread()

    carsim_thread.start()
    curses_thread.start()

    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = "/dev/ttyACM0"

    cur.buildGUI()


if __name__ == "__main__":
    wrapper(main)
