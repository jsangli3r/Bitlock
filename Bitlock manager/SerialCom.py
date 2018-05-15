from CommandInterpreter import *
import serial


class SerialCom:

    def __init__(self):
        default_transmission_rate = 9600  # baud.s^-1
        default_chip_location = '/dev/cu.usbmodem1421'

        ser = serial.Serial()
        ser.baudrate = default_transmission_rate
        ser.port = default_chip_location
        ser.timeout = 10
        if ser.isOpen():
            ser.close()
        ser.open()

        self.serial = ser

    def get_serial_instance(self):
        return self.serial

    def serial_not_init_check(self):
        if self.serial is None:
            raise Exception('Serial not init')

    def close(self):
        self.serial_not_init_check()
        self.serial.close()

    def wait_read(self):
        while True:
            try:
                self.serial_not_init_check()

                incoming_data = self.serial.readline().decode()
                incoming_data = incoming_data.replace('\n', '')
                incoming_data = incoming_data.replace('\r', '')

                command_router(incoming_data)
            except KeyboardInterrupt:
                print("wait_read while stopped")
                break

    def send(self, raw_command):
        if type(raw_command) is not str:
            raise Exception('Bad data handled, must be str')

        self.serial_not_init_check()
        self.serial.write(str.encode(raw_command))
