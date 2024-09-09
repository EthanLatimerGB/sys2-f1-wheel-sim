#!/usr/bin/env python3

import src.simulator
import src.terminal_curses as cur
import src.globals as g
import src.uart as comm

import threading
import tkinter as tk
import sys
from curses import wrapper

"""
Module Docstring
"""
__author__ = "Ethan"
__license__ = "MIT"


# Main entry point of the app
def main(stdscr):
    # Terminal and simulation threads starting
    carsim_thread = threading.Thread(target=lambda: src.simulator.simulateCar())
    carsim_thread_daemon = True

    curses_thread = threading.Thread(target=cur.cursesCommandLine, args=(stdscr,))
    curses_thread_daemon = True

    carsim_thread.start()
    curses_thread.start()

    # Optional listening to a pico microcontroller
    if len(sys.argv) < 2:
        cur.printToCurses("WARNING: No location provided in command-line arguments")
        cur.printToCurses("\t No UART communication will happen")
    else:
        SERIAL_LOCATION = sys.argv[1]
        pico_listener_thread = threading.Thread(
            target=comm.listen, args=(SERIAL_LOCATION,)
        )
        pico_listener_thread.start()

    # TkInter service init
    cur.buildGUI()


if __name__ == "__main__":
    wrapper(main)
