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
            print("No Bottle Detected, please insert Bottle")
            continue

        machine.logger.info('Bottle detected')
        machine.move_conveyor_start()

        is_metal = machine.detect_proximity_sensor()
        height_passed = machine.detect_ir_sensor()
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

        # Filling Bottle
        weight = 0  

        while weight < 95.7:
            machine.turn_on_pneumatic_actuator()
            time.sleep(3) 
            
            machine.turn_off_pneumatic_actuator()
            time.sleep(3)
           
            weight = machine.get_weight()
            machine.logger.debug(f'Current weight: {weight}')
        
        machine.turn_off_pneumatic_actuator()
        machine.close_filling_servo()
        machine.move_conveyor_end()
        time.sleep(5)
        # Move conveyor after capping
        machine.move_conveyor_final()
        time.sleep(5)
        processed += 1
        total_weight += weight

        # Check if state is still on
        if not machine.state:
            # Save processed
            machine.save_transaction(processed, total_weight)
            body = f'You have processed {processed} bottles with total weight of {total_weight} grams'
            machine.notify('Processed finished', body)
            break
