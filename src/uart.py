from sys import exception
import src.globals
import src.terminal_curses as term
import time
from serial import Serial
from queue import Empty, Queue

update_queue = Queue(maxsize=3)


def listen(SERIAL_LOCATION):
    ser = Serial()
    ser.baudrate = 115200
    ser.port = SERIAL_LOCATION
    ser.timeout = 5

    ser.open()
    term.printToCurses(f"Attempting to open connection on: {ser.port}")

    if ser.is_open:
        term.printToCurses("Opened successfully!")
        while True:
            try:
                message = update_queue.get_nowait()
                message = message + "\r"

                ser.write(message.encode("utf-8"))
                ser.flush()
            except Empty:
                pass

            try:
                if ser.in_waiting > 0:
                    data = ser.readline().decode("utf-8").strip()
                    term.printToCurses(f"uC => {data}")

            except Exception as e:
                term.printToCurses(f"[ERROR READING/WRITING SERIAL]: {e}")
    else:
        term.printToCurses("Failed to open connection to serial connection")


def send_update(message):
    if not update_queue.full():
        update_queue.put(message)
