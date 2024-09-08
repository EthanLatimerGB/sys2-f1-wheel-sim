import tkinter as tk
import time

root = tk.Tk()

# Global Variables
ENGINE_REDLINE = 15000
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

exit_flag = False
