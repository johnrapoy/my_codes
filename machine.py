from loguru import logger
import serial
import time
import sys


log_base_format = '<green>{time}</green> | <level>{level}</level> | <cyan>{name}</cyan>:<cyan>{line}</cyan> | <level>{message}</level> {extra}\n'

def format_extra(extra):
    extra_str = ''
    if isinstance(extra, dict):
        extra_str = ' '.join(f'{key}={value}' for key, value in extra.items())
    elif isinstance(extra, str):
        extra_str = extra
    return extra_str

def log_format(record):
    record['extra'] = format_extra(record.get('extra', {}))
    return log_base_format.format(**record)


class Machine:
    def __init__(self, port: str = None) -> None:
        self.arduino = serial.Serial(port, 9600, timeout=1)
        self.available_commands = list(range(10))
        self.logger = logger.bind()
        self.logger.remove()
        self.logger.add(
            f'logs/machine.log', 
            level="INFO", 
            format=log_format
        )
        self.logger.add(
            sys.stdout, 
            format=log_format,
            colorize=True
        )

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

    def detect_laser_sensor(self) -> bool:
        """
        Detect from laser sensor
        """
        self.send_command(0)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def turn_on_led(self):
        """
        Turn on LED
        """
        self.send_command(1)

    def turn_off_led(self):
        """
        Turn off LED
        """
        self.send_command(2)

    def turn_on_buzzer(self):
        """
        Turn on buzzer
        """
        self.send_command(3)

    def turn_off_buzzer(self):
        """
        Turn off buzzer
        """
        self.send_command(4)

    def detect_proximity_sensor(self) -> bool:
        """
        Detect from proximity sensor
        """
        self.send_command(5)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)

    def turn_on_servo(self):
        """
        Turn on servo
        """
        self.send_command(6)

    def turn_off_servo(self):
        """
        Turn off servo
        """
        self.send_command(7)

    def detect_ir_sensor(self) -> bool:
        """
        Detect from IR sensor
        """
        self.send_command(8)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return bool(response)
    
    def move_conveyor(self):
        """
        Move conveyor
        """
        self.send_command(9)
        response = self.get_arduino_response()
        while not response:
            response = self.get_arduino_response()
        return