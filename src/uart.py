import src.globals
import src.terminal_curses as term
import serial


def listen(SERIAL_LOCATION):
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = SERIAL_LOCATION
    ser.timeout = 1

    ser.open()
    term.printToCurses(f"Attempting to open connection on: {ser.port}")

    if ser.is_open:
        term.printToCurses("Opened successfully!")
        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode("utf-8").strip()
                term.printToCurses(f"uC => {data}")
    else:
        term.printToCurses("Failed to open connection to serial connection")
