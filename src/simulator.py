import math
import time
import src.globals as g

##
## Car Simulation
##


def simulateCar():
    while not g.exit_flag:
        # Handle gear changes
        if g.request_gear_change > 0:
            changeGear()

        # Calculate time deltas
        oldTime = g.ECU_CLOCK
        g.ECU_CLOCK = time.perf_counter()
        timeDiff = g.ECU_CLOCK - oldTime
        g.ECU_CAR_CLOCK.set(g.ECU_CLOCK)

        # Calculate engine and car speed without drag
        if g.ECU_ENGINE_SPEED.get() < 15000:
            g.ECU_ENGINE_SPEED.set(
                round(
                    engine_speed_model(
                        g.ECU_ENGINE_SPEED.get(),
                        g.ECU_CAR_GEAR.get(),
                        g.ECU_CAR_THROTTLE.get(),
                        timeDiff,
                    ),
                    1,
                )
            )
            g.ECU_CAR_SPEED.set(
                round(getSpeedOfCar(g.ECU_CAR_GEAR.get(), g.ECU_ENGINE_SPEED.get()), 1)
            )

        # Factor in drag
        if g.ECU_ENGINE_SPEED.get() > 0:
            speedsFactoringInDrag(timeDiff)
        time.sleep(1 / 60)


# Does the math and updates the rev counter to its new speed, relative to the new gear change
def changeGear():
    oldGear = g.ECU_CAR_GEAR.get()
    newGear = g.request_gear_change

    speed = getSpeedOfCar(oldGear, g.ECU_ENGINE_SPEED.get())

    # inverse of getSpeedOfCar
    newEngineRPM = getRPMfromCarSpeed(speed, newGear)
    g.ECU_ENGINE_SPEED.set(newEngineRPM)
    g.ECU_CAR_GEAR.set(newGear)
    g.request_gear_change = 0


# Returns spedd in km/h
def getSpeedOfCar(currentGear, engineRPM):
    constant = 110 + ((currentGear - 1) * 35)
    return constant * (engineRPM / 15000)


def getRPMfromCarSpeed(speed, gear):
    return (15000 * speed) / (110 + ((gear - 1) * 35))


# Returns the new engine speed after a certain period of time.
# Puts on model of a time accellerating over engine speed
def engine_speed_model(currentSpeed, gear, throttle, timeDelta):
    constant = 0
    match gear:
        case 1:
            constant = 35
        case 2:
            constant = 25
        case 3:
            constant = 20
        case 4:
            constant = 14
        case 5:
            constant = 10
        case 6:
            constant = 6
        case 7:
            constant = 4
        case 8:
            constant = 3.5
        case _:
            constant = 0

    relativeTimeToModel = math.sqrt(currentSpeed) / constant
    newSpeed = (constant * (relativeTimeToModel + timeDelta)) ** 2

    speedGain = (newSpeed - currentSpeed) * throttle
    return currentSpeed + speedGain


def speedsFactoringInDrag(timeDelta):
    speedLostDueToResistance = timeDelta * (
        (g.sim_air_resistance * (g.ECU_CAR_SPEED.get() / 150))
        + (g.ECU_CAR_BRAKE.get() * g.sim_peak_brake_power)
    )

    if (g.ECU_CAR_SPEED.get() + speedLostDueToResistance) < 0:
        return

    g.ECU_CAR_SPEED.set(round((g.ECU_CAR_SPEED.get() + speedLostDueToResistance), 1))
    g.ECU_ENGINE_SPEED.set(
        round((getRPMfromCarSpeed(g.ECU_CAR_SPEED.get(), g.ECU_CAR_GEAR.get())), 1)
    )
