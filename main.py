from machine import Machine
import time

machine = Machine(port='/dev/ttyACM0')

while True:
    print(machine.state)
    if not machine.state:
        continue
    
    processed = 0
    total_weight = 0
    while True:
       
        detected = machine.detect_bottle_ir_sensor()
        print(detected)
        if not detected:
            continue

        machine.logger.info('Bottle detected')
        machine.move_conveyor_start()

        is_metal = machine.detect_proximity_sensor()
        height_passed = machine.detect_laser_sensor()
        machine.logger.info(f'Bottle scan: is_metal={is_metal}, height_passed={height_passed}')

        # Reject bottle
        if is_metal or not height_passed:
            machine.logger.info('Rejecting bottle')
            machine.move_conveyor_reject()
            machine.close_height_servo()
            time.sleep(3)
            machine.open_height_servo()
            continue

        machine.move_conveyor_mid()
        machine.open_filling_servo()
        machine.turn_on_pneumatic_actuator()

        weight = 0
        while weight < 95.7:
            weight = machine.get_weight()
            machine.logger.debug(f'Current weight: {weight}')

        machine.turn_off_pneumatic_actuator()
        machine.close_filling_servo()
        machine.move_conveyor_end()

        recapping_done = False
        while not recapping_done:
            recapping_done = machine.detect_capping_ir_sensor()

        # Move conveyor after capping
        machine.move_conveyor_final()
        
        processed += 1
        total_weight += weight

        # Check if state is still on
        if not machine.state:
            # Save processed
            machine.save_transaction(processed, total_weight)
            body = f'You have processed {processed} bottles with total weight of {total_weight} grams'
            machine.notify('Processed finished', body)
            break
