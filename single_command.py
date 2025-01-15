from machine import Machine
import time

machine = Machine(port='/dev/ttyACM0')
machine.detect_laser_sensor()