#!/usr/bin/env python
'''
GUI for the Binary Clock API.
'''
import binaryClockAPI as BC
import CustomGUIWrappers as Gui
try:
    import Tkinter as tk
    import tkMessageBox as messagebox
    import ttk
except:
    import tkinter as tk
    from tkinter import messagebox, ttk

import serial.serialutil


class BinaryClockGUI:
    '''
    Binary Clock GUI Object
    '''
    def __init__(self):
        # get the root object
        self.root = tk.Tk()
        self.root.configure(background="white")
        self.root.title("Binary Clock API")
        # themes: winnative, clam, alt, default, classic, vista, xpnative
        self.style = ttk.Style().theme_use('clam')
        # make the window fixed in size
        self.root.resizable(width=False, height=False)
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

        # setup the buttons, input boxes, etc.
        # comport label & dropdown
        ports = BC.get_available_serial_ports()
        Gui.Labels(self.root, 0, 0, "Com Port:")
        self.serial_port_dropdown = Gui.Dropdown(self.root, 0, 1, *ports, columnspan=3, sticky="we",
                                                 bind_callback=self.update_coms_list,
                                                 bind_key="<Button-1>")
        bauds = [250000, 9600, 9600 / 2, 9600 / 4, 9600 / 8, 9600 / 16]
        Gui.Labels(self.root, 1, 0, "Baudrate:")
        self.serial_baud_dropdown = Gui.Dropdown(self.root, 1, 1, *bauds, sticky="we")
        self.serial_connect_button = Gui.Button(self.root, 1, 2, self.connect, "Connect", columnspan=1)
        self.serial_disconnect_button = Gui.Button(self.root, 1, 3, self.disconnect, "Disconnect", columnspan=1)
        # time override settings
        self.time_override_cb = Gui.CheckBox(self.root, 0, 4, cb_text="Override Time",
                                             callback_funct=self.time_override)
        self.time_entry = Gui.TimeEntry(self.root, 0, 5)
        self.time_update_button = Gui.Button(self.root, 1, 4, self.set_time, "Update Time", columnspan=1)
        self.color_update_button = Gui.Button(self.root, 1, 5, self.update_color, "Update Color", columnspan=1)
        # color settings
        self.red_label = Gui.Labels(self.root, 2, 0, "Red:", sticky="e")
        self.red_entry = Gui.ScrollBox(self.root, 2, 1, min_val=0, max_val=4, start_val=3, box_width=10, sticky="w")
        self.green_label = Gui.Labels(self.root, 3, 0, "Green:", sticky="e")
        self.green_entry = Gui.ScrollBox(self.root, 3, 1, min_val=0, max_val=4, start_val=3,
                                         box_width=10, sticky="w")
        self.blue_label = Gui.Labels(self.root, 4, 0, "Blue:", sticky="e")
        self.blue_entry = Gui.ScrollBox(self.root, 4, 1, min_val=0, max_val=4, start_val=3,
                                        box_width=10, sticky="w")
        # set color buttons
        self.set_all_button = Gui.Button(self.root, 2, 4, self.set_all_LEDs, "Set All", columnspan=2)
        self.clear_all_button = Gui.Button(self.root, 3, 4, self.clear_all_LEDs, "Clear All", columnspan=2)
        self.set_led_button = Gui.Button(self.root, 4, 2, self.set_LED, "Set LED", columnspan=1)
        self.clear_led_button = Gui.Button(self.root, 4, 3, self.clear_LED, "Clear LED", columnspan=1)
        # row & column value entry
        self.row_label = Gui.Labels(self.root, 2, 2, "Row:", sticky="e")
        self.row_entry = Gui.ScrollBox(self.root, 2, 3, min_val=0, max_val=3, start_val=0, box_width=10, sticky="w")
        self.column_label = Gui.Labels(self.root, 3, 2, "Column:", sticky="e")
        self.column_entry = Gui.ScrollBox(self.root, 3, 3, min_val=0, max_val=5, start_val=0,
                                          box_width=10, sticky="w")
        # deactivate buttons that shouldn't be accessable yet
        self.disconnect(raise_error_dialog=False)

    def time_override(self, *args):
        '''
        activates and deactivates the time override boxes (hour, minute, and second) when the
        time override checkbox is toggled.
        '''
        if self.time_override_cb.get_val():
            self.time_entry.activate()
        else:
            self.time_entry.deactivate()

    def connect(self):
        '''Connect with the binary clock. (basically just opens serial communication)'''
        try:
            self.binary_clock = BC.BinaryClockAPI(self.serial_port_dropdown.get_val(),
                                                  int(self.serial_baud_dropdown.get_val()))
        except OSError:
            messagebox.showerror(title="Invalid Port", message="The com port selected does not seem to exist.")
            return
        # make sure this doesn't get called again
        self.serial_connect_button.deactivate()
        self.serial_disconnect_button.activate()
        # allow any update functions to be called
        self.time_update_button.activate()
        self.set_all_button.activate()
        self.clear_all_button.activate()
        self.set_led_button.activate()
        self.clear_led_button.activate()
        self.color_update_button.activate()
        self.row_entry.activate()
        self.column_entry.activate()
        self.red_entry.activate()
        self.green_entry.activate()
        self.blue_entry.activate()
        self.time_entry.activate()
        self.time_override_cb.activate()

    def update_coms_list(self, *args):
        '''Update the list of serial ports when the coms list dropdown is clicked.'''
        ports = BC.get_available_serial_ports()
        if len(ports) == 0:
            ports = [""]
        self.serial_port_dropdown.update_list(ports)

    def disconnect(self, raise_error_dialog=True):
        '''
        Disconnect the binary clock by closing communication, deleting the binaryClock
        object, and resetting the GUI elements to the disconnected state.
        '''
        try:
            self.binary_clock.close()
        except:
            pass
        try:
            del self.binary_clock
        except AttributeError:
            pass
        # make sure this doesn't get called again
        self.serial_connect_button.activate()
        self.serial_disconnect_button.deactivate()
        # don't allow any update functions to be called
        self.time_update_button.deactivate()
        self.set_all_button.deactivate()
        self.clear_all_button.deactivate()
        self.set_led_button.deactivate()
        self.clear_led_button.deactivate()
        self.color_update_button.deactivate()
        self.row_entry.deactivate()
        self.column_entry.deactivate()
        self.red_entry.deactivate()
        self.green_entry.deactivate()
        self.blue_entry.deactivate()
        self.time_entry.deactivate()
        self.time_override_cb.deactivate()
        if raise_error_dialog:
            messagebox.showerror(title="Connection Lost",
                                 message="It appears you have disconnected your clock. Please reconnect the device.")

    def set_LED(self):
        '''Sets an individual LED.'''
        # get row, col, and color information from UI elements
        row = int(self.row_entry.get_val())
        column = int(self.column_entry.get_val())
        red = 2 ** int(self.red_entry.get_val()) - 1
        green = 2 ** int(self.green_entry.get_val()) - 1
        blue = 2 ** int(self.blue_entry.get_val()) - 1
        try:
            retvals = []
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_MANUAL))
            retvals.append(self.binary_clock.set_LED(row, column, red, green, blue))
            if len(retvals) != 2:
                messagebox.showerror(title="Clock Unresponsive",
                                     message="Clock did not respond. Reset your clock and try again.")
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()

    def clear_LED(self):
        '''Clears an individual LED.'''
        # get row, col, and color information from UI elements
        row = int(self.row_entry.get_val())
        column = int(self.column_entry.get_val())
        try:
            retvals = []
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_MANUAL))
            retvals.append(self.binary_clock.clear_LED(row, column))
            if len(retvals) != 2:
                messagebox.showerror(title="Clock Unresponsive",
                                     message="Clock did not respond. Reset your clock and try again.")
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()

    def set_all_LEDs(self):
        '''Sets all LEDs to a color.'''
        # get color information from UI elements
        red = 2 ** int(self.red_entry.get_val()) - 1
        green = 2 ** int(self.green_entry.get_val()) - 1
        blue = 2 ** int(self.blue_entry.get_val()) - 1
        try:
            retvals = []
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_MANUAL))
            retvals.append(self.binary_clock.set_all_LEDs(red, green, blue))
            if len(retvals) != 2:
                messagebox.showerror(title="Clock Unresponsive",
                                     message="Clock did not respond. Reset your clock and try again.")
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()

    def update_color(self):
        '''Updates the color that is displayed when time is running.'''
        # get color information from UI elements
        red = 2 ** int(self.red_entry.get_val()) - 1
        green = 2 ** int(self.green_entry.get_val()) - 1
        blue = 2 ** int(self.blue_entry.get_val()) - 1
        try:
            retvals = []
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_TIME))
            retvals.append(self.binary_clock.set_color(red, green, blue))
            if len(retvals) != 2:
                messagebox.showerror(title="Clock Unresponsive",
                                     message="Clock did not respond. Reset your clock and try again.")
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()

    def clear_all_LEDs(self):
        '''Clears all LEDs.'''
        try:
            retvals = []
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_MANUAL))
            retvals.append(self.binary_clock.clear_all_LEDs())
            if len(retvals) != 2:
                messagebox.showerror(title="Clock Unresponsive",
                                     message="Clock did not respond. Reset your clock and try again.")
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()

    def set_time(self):
        '''Sets the binary clock time.'''
        retvals = []
        try:
            retvals.append(self.binary_clock.set_state(self.binary_clock.STATE_RUN_TIME))
        except serial.serialutil.SerialException:
            # disconnect here.
            self.disconnect()
        # check if time override is selected
        if self.time_override_cb.get_val():
            time_dict = self.time_entry.get_val()
            time_override_array = [time_dict["hour"], time_dict["minute"], time_dict["second"]]
            try:
                retvals.append(self.binary_clock.update_time(time_override_array))
            except serial.serialutil.SerialException:
                # disconnect here.
                self.disconnect()
        else:
            try:
                retvals.append(self.binary_clock.update_time())
            except serial.serialutil.SerialException:
                # disconnect here.
                self.disconnect()
        if len(retvals) != 2:
            messagebox.showerror(title="Clock Unresponsive",
                                 message="Clock did not respond. Reset your clock and try again.")

    def run(self):
        '''Runs the tk loop.'''
        self.root.mainloop()

    def on_closing(self):
        '''Asks user if they really want to close, then destroys the window.'''
        if messagebox.askquestion("Quit", "Do you want to quit?"):
            self.root.destroy()
        try:
            self.binary_clock.close()
        except:
            pass


if __name__ == '__main__':
    BC_GUI = BinaryClockGUI()
    BC_GUI.run()
