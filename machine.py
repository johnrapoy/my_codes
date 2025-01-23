from loguru import logger
from threading import Thread
from rpi_lcd import LCD
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
        self.arduino = serial.Serial(port, 9600, timeout=10)
        self.available_commands = list(range(15))

        self.ws_host = "ws://10.42.0.1:8000/ws"
        self.ws: websocket.WebSocketApp = None
        self.start_websocket_listener()

        self.lcd = LCD()

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
        if (command in self.available_commands):
            while True:
                self.arduino.write(bytes(str(command)+'\n','utf-8'))
                time.sleep(0.5)
                response = self.get_arduino_response()
                if response == 'ok':
                    break
        else:
            raise Exception('Unknown command')
        self.logger.info(f'Sent command to arduino: {command}')
            
    def get_arduino_response(self) -> str:
        """
        Get arduino serial response

        Returns:
        response (str) : Arduino response
        """
        try:
            response = self.arduino.readline().decode('utf-8').rstrip()
        except UnicodeDecodeError:
            self.logger.debug('UnicodeDecodeError occured')
            response = self.arduino.readline().decode('utf-8').rstrip()
        self.logger.debug(f'Got response from arduino: {response}')
        return response

    def move_conveyor_start(self):
        """
        Send `moveConveyorStart` command to arduino
        """
        self.send_command(0)
        response = self.get_arduino_response()
        while response != 'done':
            response = self.get_arduino_response()

    def move_conveyor_reject(self):
        """
        Send `moveConveyorReject` command to arduino
        """
        self.send_command(1)
        response = self.get_arduino_response()
        while response != 'done':
            response = self.get_arduino_response()

    def move_conveyor_mid(self):
        """
        Send `moveConveyorMid` command to arduino
        """
        self.send_command(2)
        response = self.get_arduino_response()
        while response != 'done':
            response = self.get_arduino_response()

    def move_conveyor_end(self):
        """
        Send `moveConveyorEnd` command to arduino
        """
        self.send_command(3)
        response = self.get_arduino_response()
        while response != 'done':
            response = self.get_arduino_response()

    def move_conveyor_final(self):
        """
        Send `moveConveyorFinal` command to arduino
        """
        self.send_command(4)
        response = self.get_arduino_response()
        while response != 'done':
            response = self.get_arduino_response()
        
    def detect_bottle_ir_sensor(self) -> bool:
        """
        Detect from bottle ir sensor
        """
        self.send_command(5)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return response == '1'

    def detect_ir_sensor(self) -> bool:
        """
        Detect from ir sensor on arduino
        """
        self.send_command(6)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def open_height_servo(self):
        """
        Open the height servo motor
        """
        self.send_command(7)

    def close_height_servo(self):
        """
        Close the height servo motor
        """
        self.send_command(8)

    def detect_proximity_sensor(self) -> bool:
        """
        Detect from proximity sensor
        """
        self.send_command(9)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return response == '1'

    def open_filling_servo(self):
        """
        Open the height servo motor
        """
        self.send_command(10)

    def close_filling_servo(self):
        """
        Close the height servo motor
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

    def get_weight(self) -> float:
        """
        Get weight from load cell on arduino
        """
        self.send_command(14)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return float(response)

    def create_transaction(self, quantity: int, weight: float):
        """
        Save transaction to database server

        Parameters:
        quantity (int) : Transaction quantity
        weight (float) : Transaction weight
        """
        data = {
            'type': 'add_transaction',
            'quantity': quantity,
            'weight': weight
        }
        self.ws.send(json.dumps(data))

    def create_notification(self, category: str, title: str, body: str):
        """
        Send notification to server

        Parameters:
        category (str) : Notification category
        title (str) : Notification title
        body (str) : Notification body
        """
        data = {
            'type': 'add_notification',
            'category': category,
            'title': title,
            'body': body,
        }
        self.ws.send(json.dumps(data))
