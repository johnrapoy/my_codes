from machine import Machine
import time


machine = Machine(port='/dev/ttyACM0')

while True:
    print(machine.state)
    if not machine.state:
        continue
    
    processed = 0
    total_weight = 0
    machine.lcd.text('EcoBricks Maker', line=1)
    time.sleep(3)
    machine.lcd.text('Starting Process', line=1)
    time.sleep(3)

    # Processing starts here
    while True:
        detected = machine.detect_bottle_ir_sensor()
        print(detected)
        if not detected:
            machine.lcd.text('No Bottle Detected', line=1) 
            machine.lcd.txt ('Please insert Bottle', line=2)
            print("No Bottle Detected, please insert Bottle")
            continue

        machine.logger.info('Bottle detected')
        machine.move_conveyor_start()

        is_metal = machine.detect_proximity_sensor()
        height_passed = machine.detect_ir_sensor()
        machine.lcd.txt ('Scan Results:', line=1)
        machine.lcd.txt (f'Bottle scan: is_metal={is_metal}, height_passed={height_passed}', line=2)
        machine.logger.info(f'Bottle scan: is_metal={is_metal}, height_passed={height_passed}')

        # Reject bottle
        if is_metal or not height_passed:
            machine.lcd.text('Rejecting Bottle', line=1)
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
            machine.lcd.text(f'Current weight:', line=1)
            machine.lcd.txt(f'{weight} grams', line=2)
            machine.logger.debug(f'Current weight: {weight}')
        
        machine.turn_off_pneumatic_actuator()
        machine.close_filling_servo()
        machine.move_conveyor_end()
        machine.lcd.text('Capping Process')
        time.sleep(5)
        # Move conveyor after capping
        machine.move_conveyor_final()
        machine.lcd.text('Process Done', line=1)
        time.sleep(5)
        processed += 1
        total_weight += weight

        machine.create_notification(
            'progress', 
            'Single bottle notification', 
            'Finished processing single bottle'
        )

        # Check if state is still on
        if not machine.state:
            # Save processed
            machine.create_transaction(processed, total_weight)
            body = f'You have processed {processed} bottles with total weight of {total_weight} grams'
            machine.create_notification(
                'transaction', 
                'Processed finished', 
                f'You have processed {processed} bottles with total weight of {total_weight} grams'
            )
            break
