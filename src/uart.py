import src.globals
import serial
import src.uart as comm


def listen():
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = "/dev/ttyACM0"
    ser.timeout = 1

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode("utf-8").strip()
            comm.history.append(data)
