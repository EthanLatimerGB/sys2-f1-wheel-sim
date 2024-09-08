import src.globals as g

import curses
import threading
import tkinter as tk

history = [
    "This is the car simulation command line",
    "Enter commands to change the behaviour of the car, type HELP for the list of commands",
]
history_lock = threading.Lock()


# Main entrypoint for curses
def cursesCommandLine(stdscr):
    global history
    global current_command

    max_history = curses.LINES - 1

    stdscr.clear()
    curses.echo()

    stdscr.refresh()

    while not g.exit_flag:
        with history_lock:
            for num, line in enumerate(history[-max_history:]):
                stdscr.addstr(num, 0, line)

        stdscr.addstr(curses.LINES - 1, 0, "> ")
        current_command = stdscr.getstr(curses.LINES - 1, 3).decode("utf-8")
        stdscr.clrtoeol()
        stdscr.refresh()
        handle_commands(stdscr)


# ** Commands List **
# THROTTLE <INT>
# BRAKE <INT>
# POWER <Bool>
# HELP
def handle_commands(stdscr):
    if current_command == "":
        return

    tokens = current_command.split(" ")
    match tokens[0].upper():
        case "THROTTLE":
            value = int(tokens[1])
            if value >= 0 and value <= 100:
                g.ECU_CAR_THROTTLE.set(value / 100)
                printToCurses("Throttle open: {}".format(g.ECU_CAR_THROTTLE.get()))
                stdscr.refresh()
        case "GEAR":
            gearNumber = int(tokens[1])
            if gearNumber < 9 and gearNumber > 0:
                g.request_gear_change = gearNumber
                printToCurses("Changed gear to: {}".format(gearNumber))
        case "BRAKE":
            value = int(tokens[1])
            if value >= 0 and value <= 100:
                g.ECU_CAR_BRAKE.set(value / 100)
                printToCurses("Brake pressure: {}".format(g.ECU_CAR_BRAKE.get()))
        case "HELP":
            printToCurses("============= HELP ==============")
            printToCurses("- THROTTLE <1-100> : Change throttle from 0 to 100%")
            printToCurses("- GEAR <1-8> : Change gear")
            printToCurses(
                "- BRAKE <1-100> : Press on the brake with pressure from 0 to 100%"
            )
            printToCurses("==================================")
        case "QUIT":
            g.root.quit()
            exit()


def printToCurses(text):
    global history
    with history_lock:
        history.append(text)


def buildGUI():
    g.root.title("Live data")
    g.root.geometry("400x150")

    frame = tk.Frame(g.root)

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

    enginespeed_label = tk.Label(frame, textvariable=g.ECU_ENGINE_SPEED)
    carspeed_label = tk.Label(frame, textvariable=g.ECU_CAR_SPEED)
    clock_label = tk.Label(frame, textvariable=g.ECU_CAR_CLOCK)
    gear_label = tk.Label(frame, textvariable=g.ECU_CAR_GEAR)
    throttle_label = tk.Label(frame, textvariable=g.ECU_CAR_THROTTLE)

    clock_label.grid(row=0, column=1)
    enginespeed_label.grid(row=1, column=1)
    carspeed_label.grid(row=2, column=1)
    gear_label.grid(row=3, column=1)
    throttle_label.grid(row=4, column=1)

    frame.pack()
    g.root.mainloop()
