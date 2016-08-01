import serial
import time
import os


class BinaryClock:
    def __init__(self, serial_port, baud=115200):
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
        self.coms = serial.Serial(port=serial_port, baudrate=baud)

    def set_LED(self, row, column, red, green, blue):
        self.coms.write(bytes(self.UART_CMD_SET_LED))
        self.coms.write(bytes(row))
        self.coms.write(bytes(column))
        self.coms.write(bytes(red))
        self.coms.write(bytes(green))
        self.coms.write(bytes(blue))
        response = self.coms.read(size=1)
        return response

    def update_time(self):
        time_vals = time.localtime()
        hour = time_vals[3]
        minute = time_vals[4]
        second = time_vals[5]
        self.coms.write(bytes(self.UART_CMD_SET_TIME))
        self.coms.write(bytes(hour))
        self.coms.write(bytes(minute))
        self.coms.write(bytes(second))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response

    def clear_LED(self, row, column):
        self.coms.write(bytes(self.UART_CMD_SET_TIME))
        self.coms.write(bytes(row))
        self.coms.write(bytes(column))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response

    def clear_all_LEDs(self):
        self.coms.write(bytes(self.UART_CMD_CLEAR_ALL_LED))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response

    def set_all_LEDs(self, red, green, blue):
        self.coms.write(bytes(self.UART_CMD_SET_ALL_LED))
        self.coms.write(bytes(red))
        self.coms.write(bytes(green))
        self.coms.write(bytes(blue))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response

    def set_color(self, red, green, blue):
        self.coms.write(bytes(self.UART_CMD_SET_COLOR))
        self.coms.write(bytes(red))
        self.coms.write(bytes(green))
        self.coms.write(bytes(blue))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response

    def set_state(self, state):
        self.coms.write(bytes(self.UART_CMD_SET_COLOR))
        self.coms.write(bytes(state))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        self.coms.write(bytes(0))
        response = self.coms.read(size=1)
        return response


def run_as_main():
    os.system('cls' if os.name == 'nt' else 'clear')
    port = int(raw_input("Enter serial port number: "))
    serialport = "COM{:}".format(port)
    bc_obj = BinaryClock(serial_port=serialport, baudrate=115200)
    print "0. Set Led"
    print "1. Clear Led"
    print "2. Clear All Led"
    print "3. Set All Led"
    print "4. Set Time"
    print "5. Set Color"
    print "6. Change State"
    command_number = raw_input("Command Number: ")
    cmd_num = int(command_number)
    if cmd_num == 0:
        row = int(raw_input("   row: "))
        column = int(raw_input("   column: "))
        red_val = int(raw_input("   red val [0-255]: "))
        green_val = int(raw_input("   green val [0-255]: "))
        blue_val = int(raw_input("   blue val [0-255]: "))
        retval = bc_obj.set_LED(row, column, red_val, green_val, blue_val)
        print retval
    elif cmd_num == 1:
        row = int(raw_input("   row: "))
        column = int(raw_input("   column: "))
        retval = bc_obj.clear_LED(row, column)
        print retval
    elif cmd_num == 2:
        retval = bc_obj.clear_all_LED()
        print retval
    elif cmd_num == 3:
        red_val = int(raw_input("   red val [0-255]: "))
        green_val = int(raw_input("   green val [0-255]: "))
        blue_val = int(raw_input("   blue val [0-255]: "))
        retval = bc_obj.set_all_LEDs(red_val, green_val, blue_val)
        print retval
    elif cmd_num == 4:
        retval = bc_obj.update_time()
        print retval
    elif cmd_num == 5:
        red_val = int(raw_input("   red val [0-255]: "))
        green_val = int(raw_input("   green val [0-255]: "))
        blue_val = int(raw_input("   blue val [0-255]: "))
        retval = bc_obj.set_color(red_val, green_val, blue_val)
        print retval
    elif cmd_num == 6:
        print "  run by time state: enter 0"
        print "  run by manual state: enter 1"
        state = int(raw_input("   State: "))
        retval = bc_obj.set_state(state)
        print retval


if __name__ == '__main__':
    run_as_main()
