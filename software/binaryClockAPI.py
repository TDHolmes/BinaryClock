#!/usr/bin/env python
'''
API functions for interfacing with the Binary Clock
'''
import serial
import time
import os
import sys
import binascii
import struct
import glob

# just for my python3 linter to shut up...
if sys.version_info.major == 3:
    raw_input = input


class BinaryClockAPI:
    """
    Main class that handles communication and exposes functions to be called.

    :param str serial_port: Serial port to connect to. (e.g. "/dev/tty.usbserial-12345")
    :param int baud: baudrate to communicate at (default: 250000)
    """
    def __init__(self, serial_port, baud=250000):
        # Start byte
        self.UART_START_BYTE = 0xAA
        # Command bytes
        self.UART_CMD_SET_LED = 0x10
        self.UART_CMD_CLEAR_LED = 0x11
        self.UART_CMD_CLEAR_ALL_LED = 0x12
        self.UART_CMD_SET_ALL_LED = 0x13
        self.UART_CMD_SET_TIME = 0x14
        self.UART_CMD_SET_COLOR = 0x15
        self.UART_CMD_CHANGE_STATE = 0x16
        # error codes
        self.UART_CMD_INVALID = 0xFF
        self.UART_CMD_WRONG_STATE = 0xFE
        # valid command
        self.UART_CMD_VALID = 0x42
        self.STATE_RUN_TIME = 0
        self.STATE_RUN_MANUAL = 1
        self.coms = serial.Serial(port=serial_port, baudrate=baud, timeout=3)

    def close(self):
        """ Closes the com port to the device"""
        self.coms.close()

    def set_LED(self, row, column, red, green, blue):
        """
        Set an LED to the given color.

        :param int row: Row of the LED to set
        :param int column: Column of the LED to set
        :param int red: Red value (0 - 255)
        :param int green: Green value (0 - 255)
        :param int blue: Blue value (0 - 255)
        """
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_SET_LED))
        self.coms.write(struct.pack("B", row))
        self.coms.write(struct.pack("B", column))
        self.coms.write(struct.pack("B", red))
        self.coms.write(struct.pack("B", green))
        self.coms.write(struct.pack("B", blue))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def update_time(self, time_override_arr=None):
        """
        Function for updating the time displayed on the clock. By default, uses
        time.localtime() function to grab the current time from your computer.

        :param list time_override_arr: List (in the format [hour, min, sec]) if you want to
            override the local time on your machine.
        """
        if time_override_arr is None:
            time_vals = time.localtime()
            hour = time_vals[3]
            minute = time_vals[4]
            second = time_vals[5]
        else:
            hour = time_override_arr[0]
            minute = time_override_arr[1]
            second = time_override_arr[2]
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_SET_TIME))
        self.coms.write(struct.pack("B", hour))
        self.coms.write(struct.pack("B", minute))
        self.coms.write(struct.pack("B", second))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def clear_LED(self, row, column):
        """Clear an LED at the given co-ordinates.

        :param int row: Row of the LED to clear
        :param int column: Column of the LED to clear
        """
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_CLEAR_LED))
        self.coms.write(struct.pack("B", row))
        self.coms.write(struct.pack("B", column))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def clear_all_LEDs(self):
        """Clears all LEDs."""
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_CLEAR_ALL_LED))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def set_all_LEDs(self, red, green, blue):
        """
        Sets all LEDs to the given color.

        :param int red: Red value (0 - 255)
        :param int green: Green value (0 - 255)
        :param int blue: Blue value (0 - 255)
        """
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_SET_ALL_LED))
        self.coms.write(struct.pack("B", red))
        self.coms.write(struct.pack("B", green))
        self.coms.write(struct.pack("B", blue))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def set_color(self, red, green, blue):
        """
        Sets the active colors when running in time mode.

        :param int red: Red value (0 - 255)
        :param int green: Green value (0 - 255)
        :param int blue: Blue value (0 - 255)
        """
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_SET_COLOR))
        self.coms.write(struct.pack("B", red))
        self.coms.write(struct.pack("B", green))
        self.coms.write(struct.pack("B", blue))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)

    def set_state(self, state):
        """
        Sets the current state of the device. (either time control or manual control)

        :param int state: Can either be 0 (RUN TIME) or 1 (RUN MANUAL)
        """
        if state != 0 and state != 1:
            raise ValueError("State must either be a 0 or a 1!")
        self.coms.write(struct.pack("B", self.UART_START_BYTE))
        self.coms.write(struct.pack("B", self.UART_CMD_CHANGE_STATE))
        self.coms.write(struct.pack("B", state))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        self.coms.write(struct.pack("B", 0))
        response = self.coms.read(size=1)
        return binascii.hexlify(response)


def run_as_main():
    """
    If you wish to use these functions via the command line, this function
    gives you a bare-bones way to access the functions. It will ask you which function
    you wish to issue and ask you for the requisite data fields.
    """
    options = {"Set Led": 0, "Clear Led": 1, "Clear All Led": 2, "Set All Led": 3,
               "Set Time": 4, "Set Color": 5, "Change State": 6}
    os.system('cls' if os.name == 'nt' else 'clear')
    print("finding ports")
    available_ports = get_available_serial_ports()
    print("Available ports:")
    for ind, port in enumerate(available_ports):
        print("  {:}: {:}".format(ind, port))
    port_ind = int(raw_input("Select port: "))
    serialport = available_ports[port_ind]
    bc_obj = BinaryClockAPI(serial_port=serialport)
    while True:
        print("0. Set Led")
        print("1. Clear Led")
        print("2. Clear All Led")
        print("3. Set All Led")
        print("4. Set Time")
        print("5. Set Color")
        print("6. Change State")
        print("")
        command_number = raw_input("Command Number: ")
        cmd_num = int(command_number)
        if cmd_num == options["Set Led"]:
            # Set an individual LED to a color
            row = int(raw_input("   row: "))
            column = int(raw_input("   column: "))
            red_val = int(raw_input("   red val [0-255]: "))
            green_val = int(raw_input("   green val [0-255]: "))
            blue_val = int(raw_input("   blue val [0-255]: "))
            retval = bc_obj.set_LED(row, column, red_val, green_val, blue_val)
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Clear Led"]:
            # Clear an LED to 0
            row = int(raw_input("   row: "))
            column = int(raw_input("   column: "))
            retval = bc_obj.clear_LED(row, column)
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Clear All Led"]:
            # Clear ALL LEDs
            retval = bc_obj.clear_all_LEDs()
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Set All Led"]:
            # Set ALL LEDs to a color
            red_val = int(raw_input("   red val [0-255]: "))
            green_val = int(raw_input("   green val [0-255]: "))
            blue_val = int(raw_input("   blue val [0-255]: "))
            retval = bc_obj.set_all_LEDs(red_val, green_val, blue_val)
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Set Time"]:
            # Set the time to the current
            override = raw_input("   Manual time? (Y/n): ").lower()
            if override == "y":
                override_arr = []
                override_arr[0] = int(raw_input("   hour: "))
                override_arr[1] = int(raw_input("   minute: "))
                override_arr[2] = int(raw_input("   second: "))
                retval = bc_obj.update_time(time_override_arr=override_arr)
            else:
                retval = bc_obj.update_time()
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Set Color"]:
            # Set the color if the mode is time
            red_val = int(raw_input("   red val [0-255]: "))
            green_val = int(raw_input("   green val [0-255]: "))
            blue_val = int(raw_input("   blue val [0-255]: "))
            retval = bc_obj.set_color(red_val, green_val, blue_val)
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))
        elif cmd_num == options["Change State"]:
            # change the state
            print("  run by time state: enter 0")
            print("  run by manual state: enter 1")
            state = int(raw_input("   State: "))
            retval = bc_obj.set_state(state)
            print("retval = 0x{:} (len = {:})".format(binascii.hexlify(retval), len(retval)))


# adapted from http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
def get_available_serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/cu.usbserial*')
        return ports

    result = []
    for ind, port in enumerate(ports):
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


if __name__ == '__main__':
    run_as_main()
