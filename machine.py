from loguru import logger
from threading import Thread
import json
import serial
import time
import sys
import websocket

class Machine:
    def __init__(self, port: str = None) -> None:
        self.logger = logger.bind()
        self.logger.remove()
        self.logger.add(
            f'logs/machine.log', 
            level="INFO", 
        )
        self.logger.add(
            sys.stdout, 
            colorize=True
        )

        self.state = False
        self.arduino = serial.Serial(port, 9600, timeout=1)
        self.available_commands = list(range(10))

        self.ws_host = "ws://192.168.0.153:8000/ws"
        self.ws = None
        self.start_websocket_listener()

    def start_websocket_listener(self):
        """
        Start the websocket listener on another thread
        """
        def _start():
            self.ws = websocket.WebSocketApp(
            self.ws_host,
            on_open = lambda ws: print('Starting websocket listener...'),
            on_message=lambda ws, message: self._parse_websocket_message(ws, message),
            on_error=lambda ws, error: print(error),
            on_close=lambda ws, close_status_code, close_msg: print('Server closing')
        )
            self.ws.run_forever()
        Thread(target=_start).start()

    def _parse_websocket_message(self, ws, message):
        """
        Parse the websocket message
        Just mainly for listening to state updates
        """
        data = json.loads(message)
        self.logger.info(data)
        if data['type'] == 'parameter':
            self.state = data['status']

    def update_state(self, state: bool):
        """
        Update parameters and send to server
        """
        data = {
            'type': 'update_machine_status',
            'state': state,
            'origin': 'machine'
        }
        json_data = json.dumps(data)
        self.ws.send(json_data)
        self.state = state

    def send_command(self, command: int):
        """
        Send command to arduino. 
        Can be used to explicitly invoke Arduino operation without calling specific functions \n

        Parameters:
        command (int) : Command to send
        """
        self.logger.debug(f'Send Command to Arduino: {command}')
        if command in self.available_commands:
            self.arduino.write(bytes(str(command)+'\n','utf-8'))
            self.arduino.flush()
            while True:
                time.sleep(0.5)
                response = self.get_arduino_response()
                if response == 'ok':
                    break
        else:
            raise Exception('Unknown command')
            
    def get_arduino_response(self) -> str:
        """
        Get arduino serial response

        Returns:
        response (str) : Arduino response
        """
        try:
            response = self.arduino.readline().decode('utf-8').rstrip()
        except UnicodeDecodeError:
            response = self.arduino.readline().decode('utf-8').rstrip()
        self.logger.debug(f'Got response from arduino: {response}')
        return response

    def move_conveyor_start(self):
        """
        Send `moveConveyorStart` command to arduino
        """
        self.send_command(0)

    def move_conveyor_mid(self):
        """
        Send `moveConveyorMid` command to arduino
        """
        self.send_command(1)

    def move_conveyor_end(self):
        """
        Send `moveConveyorEnd` command to arduino
        """
        self.send_command(2)

    def detect_laser_sensor(self) -> bool:
        """
        Detect from laser sensor on arduino
        """
        self.send_command(3)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def open_height_servo(self):
        """
        Open the height servo motor
        """
        self.send_command(4)

    def close_height_servo(self):
        """
        Close the height servo motor
        """
        self.send_command(5)

    def detect_proximity_sensor(self) -> bool:
        """
        Detect from proximity sensor
        """
        self.send_command(6)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def open_reject_servo(self):
        """
        Open the reject servo motor
        """
        self.send_command(7)

    def close_reject_servo(self):
        """
        Close the reject servo motor
        """
        self.send_command(8)

    def detect_filling_ir_sensor(self) -> bool:
        """
        Detect from filling IR sensor
        """
        self.send_command(9)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)
    
    def turn_on_shredder(self):
        """
        Turn on shredder relay
        """
        self.send_command(10)

    def turn_off_shredder(self):
        """
        Turn off shredder relay
        """
        self.send_command(11)

    def turn_on_pneumatic_actuator(self):
        """
        Turn on pneumatic actuator
        """
        self.send_command(12)

    def turn_off_pneumatic_actuator(self):
        """
        Turn off pneumatic actuator
        """
        self.send_command(13)

    def get_weight(self) -> bool:
        """
        Get weight from load cell on arduino
        """
        self.send_command(14)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return float(response)

    def detect_capping_ir_sensor(self) -> bool:
        """
        Detect from capping IR sensor
        """
        self.send_command(15)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def turn_on_capping_relay(self):
        """
        Turn on capping relay
        """
        self.send_command(16)

    def turn_off_capping_relay(self):
        """
        Turn off capping relay
        """
        self.send_command(17)

    def detect_reject_ir_sensor(self) -> bool:
        """
        Detect from reject IR sensor
        """
        self.send_command(18)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)
