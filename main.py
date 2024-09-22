from machine import Machine

machine = Machine(port=None)

while not machine.detect_ir_sensor():
    pass

while not machine.detect_proximity_sensor() and not machine.detect_laser_sensor():
    machine.move_conveyor()

machine.turn_on_servo()

while not machine.detect_ir_sensor_2():
     machine.move_conveyor()

machine.turn_on_pneumatic()

