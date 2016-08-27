import binaryClockAPI
import time


def main(port, mode, baudrate=250000, time_delay=0.25):
    modes = ["rainbow all", "rainbow each", "test all", "test each", "set time"]
    DELAY = time_delay
    BC = binaryClockAPI.BinaryClock(serial_port=port, baud=baudrate)
    if mode == modes[0]:
        # rainbow all of the LEDs
        BC.set_state(BC.STATE_RUN_MANUAL)
        red = 0
        green = 0
        blue = 0
        while True:
            BC.set_all_LEDs(red, green, blue)
            time.sleep(0.25)
            red = (red << 1) + 1
            if (red >= 16):
                red = 0
                green = (green << 1) + 1
                if (green >= 16):
                    green = 0
                    blue = (blue << 1) + 1
                    if (blue >= 16):
                        blue = 0
    elif mode == modes[1]:
        # rainbow each LED individually
        BC.set_state(BC.STATE_RUN_MANUAL)
        while True:
            for row in range(4):
                for col in range(6):
                    # make sure it's a valid row/col pair
                    if row >= 2:
                        if row >= 2 and col == 0:
                            continue
                        elif row == 3 and (col == 2 or col == 4):
                            continue
                    red = 0
                    green = 0
                    blue = 0
                    while blue < 16:
                        BC.set_LED(row, col, red, green, blue)
                        # time.sleep(0.25)
                        red = (red << 1) + 1
                        if (red >= 16):
                            red = 0
                            green = (green << 1) + 1
                            if (green >= 16):
                                green = 0
                                blue = (blue << 1) + 1
                    BC.clear_all_LEDs()
    elif mode == modes[2]:
        # test all LEDS (set to full red, full green, full blue, then full all)
        BC.set_state(BC.STATE_RUN_MANUAL)
        BC.clear_all_LEDs()
        BC.set_all_LEDs(0x0F, 0x00, 0x00)
        time.sleep(DELAY)
        BC.set_all_LEDs(0x00, 0x0F, 0x00)
        time.sleep(DELAY)
        BC.set_all_LEDs(0x00, 0x00, 0x0F)
        time.sleep(DELAY)
        BC.set_all_LEDs(0x0F, 0x0F, 0x0F)
        time.sleep(DELAY)
    elif mode == modes[3]:
        # test each LED individually
        BC.set_state(BC.STATE_RUN_MANUAL)
        while True:
            for row in range(4):
                for col in range(6):
                    # make sure it's a valid row/col pair
                    if row >= 2:
                        if row >= 2 and col == 0:
                            continue
                        elif row == 3 and (col == 2 or col == 4):
                            continue
                    BC.clear_all_LEDs()
                    BC.set_LED(row, col, 0x0F, 0x00, 0x00)
                    time.sleep(DELAY)
                    BC.set_LED(row, col, 0x00, 0x0F, 0x00)
                    time.sleep(DELAY)
                    BC.set_LED(row, col, 0x00, 0x00, 0x0F)
                    time.sleep(DELAY)
                    BC.set_LED(row, col, 0x0F, 0x0F, 0x0F)
                    time.sleep(DELAY)
    elif mode == modes[4]:
        BC.set_state(BC.STATE_RUN_TIME)
        BC.update_time()


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Test the Binary Clock.',
                                     formatter_class=argparse.RawTextHelpFormatter)
    help_str = """
mode to use the BC tester in. Valid modes are:
    'rainbow all': Goes through all color combinations on all LEDs at once
    'rainbow each': Goes through all color combinations on each LED
    'test all': sets full red, then full green, then full blue, then full all on all LEDs at once
    'test each': sets full red, then full green, then full blue, then full all on each LED
    'set time': sets the clock to the current local time
-> Make sure to use quotes!  """
    parser.add_argument('mode', type=str,
                        help=help_str)

    args = parser.parse_args()

    main(port="/dev/tty.usbserial-A5047KDG", mode=args.mode)
