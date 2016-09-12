'''
Custom wrappers around Tkinter classes & functions.
'''

# tk stuff
import Tkinter as tk
import tkMessageBox as messagebox
import ttk


class CheckBox:
    '''
    Class for creating a checkbox. Used when you need a boolean decision from the user.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the check box is put into.
    :param int column: what column the check box is put into.
    :param funct true_callback_funct: This function is called when the box is checked by the user.
    :param funct false_callback_funct: This function is called when the box is unchecked by the user.
    :param str cb_text: this is the label for the check box. [default: ""]
    :param int padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int pady: This is how much y axis padding is given to the check box. [default: 3 px]
    '''
    def __init__(self, master, row, column, callback_funct=None, cb_text="",
                 padx=3, pady=3, columnspan=1):
        self.val = tk.BooleanVar()
        self.callback_funct = callback_funct
        self.widget = tk.Checkbutton(master, text=cb_text, variable=self.val, command=self.cb)
        self.widget.configure(background="white")
        self.widget.grid(row=row, column=column, sticky=tk.W, padx=padx, pady=pady, columnspan=columnspan)

    def cb(self):
        '''
        This function is called when the check box changes state. if it is checked, it calls
        *self.true_callback_funct()*. if it is unchecked, it calls *self.false_callback_funct()*
        '''
        if self.callback_funct is not None:
            self.callback_funct(self.get_val())

    def get_val(self):
        '''Returns the current value of the check box.'''
        return self.val.get()

    def deactivate(self):
        '''Deactivates the check box to prevent the user from changing it's value.'''
        self.widget.configure(state=tk.DISABLED)

    def activate(self):
        '''Activates the check box to allow the user to change it's value.'''
        self.widget.configure(state=tk.NORMAL)

    def select(self):
        self.val.set(True)

    def deselect(self):
        self.val.set(False)


class EntryBox:
    '''
    Class for creating an Entry box. Used when you need a string input from the user.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the entry box is put into.
    :param int column: what column the entry box is put into.
    :param str default_val: String that first populates the box. [default: ""]
    :param int \*\*padx: This is how much x axis padding is given to the entry box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the entry box. [default: 3 px]
    :param int \*\*box_width: Sets how wide the input box is [default: 20 px]
    :param str \*\*state: Sets the state of the entry box (disabled, readonly, etc.) [default: "active"]
    :param \*\*sticky: Sets which side of the cell the Entry box sticks to. (e.g. tk.E) [default: ""]
    :param \*\*justify: Sets how the text in the Entry box aligns. [default: tk.LEFT]
    '''
    def __init__(self, master, row, column, default_val="", **kwargs):
        self.val = tk.StringVar()
        # get various named arguments
        padx = kwargs.get("padx", 3)
        pady = kwargs.get("pady", 3)
        box_width = kwargs.get("box_width", 20)
        state = kwargs.get("state", "active")
        sticky = kwargs.get("sticky", "")
        justify = kwargs.get("justify", tk.LEFT)
        self.row = row
        self.column = column
        self.sticky = sticky
        self.padx = padx
        self.pady = pady
        self.widget = ttk.Entry(master, textvariable=self.val, width=box_width, justify=justify, state=state)
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky, padx=self.padx, pady=self.pady)
        if type(default_val) is int:
            default_val = str(default_val)
        self.widget.insert(0, default_val)

    def get_val(self):
        '''Returns the current value of the Entry box.'''
        return self.val.get()

    def activate(self):
        '''Activates the entry box to allow the user to change it's value.'''
        self.widget.config(state=tk.NORMAL)

    def deactivate(self):
        '''Deactivates the entry box to prevent the user from changing it's value.'''
        self.widget.config(state=tk.DISABLED)

    def focus(self):
        '''Moves the user's cursor to this box.'''
        self.widget.focus()

    def bind(self, key, func):
        self.widget.bind(key, func)

    def delete(self, index, last=None):
        self.widget.delete(index, last)

    def insert(self, index, s):
        self.widget.insert(index, s)

    def forget(self):
        '''Removes the label from the GUI temporarily.'''
        self.widget.grid_forget()

    def remember(self):
        '''Brings the label back into view.'''
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky, padx=self.padx, pady=self.pady)


class Dropdown:
    '''
    Class for creating a Dropdown menu. Used when you need an option from a fixed list the user.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the dropdown is put into.
    :param int column: what column the dropdown is put into.
    :param list \*values: String list that populates the options.
    :param int \*\*padx: This is how much x axis padding is given to the dropdown box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the dropdown box. [default: 3 px]
    :param int \*\*sticky: Sets where the dropdown box populates in it's cell. [default: None]
    :param int \*\*columnspan: Sets how many columns the label spans. [default: 1]
    :param funct \*\*trace_callback: Callback function for when the dropdown variable has been changed.
                                     [default: None]
    :param funct \*\*bind_callback: Callback function binded to the key *bind_key*. [default: None]
    :param str \*\*bind_key: Key that binds the function *bind_callback*. [default: None]
    '''
    def __init__(self, master, row, column, *options, **kwargs):
        self.val = tk.StringVar()
        # get various named arguments
        padx = kwargs.get("padx", 3)
        pady = kwargs.get("pady", 3)
        sticky = kwargs.get("sticky", "")
        self.columnspan = kwargs.get("columnspan", 1)
        self.trace_calback = kwargs.get("trace_callback", None)
        self.bind_callback = kwargs.get("bind_callback", None)
        if self.bind_callback is not None:
            self.bind_key = kwargs.get("bind_key", None)
            if self.bind_key is None:
                raise AttributeError
        self.row = row
        self.column = column
        self.sticky = sticky
        self.padx = padx
        self.pady = pady
        self.options = options
        if len(self.options) == 0:
            self.options = [""]
        self.widget = ttk.OptionMenu(master, self.val, self.options[0], *self.options)
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky, padx=self.padx, pady=self.pady,
                         columnspan=self.columnspan)
        if self.trace_calback is not None:
            self.val.trace_variable("w", self.trace_calback)
        if self.bind_callback is not None:
            self.widget.bind(self.bind_key, self.bind_callback)

    def get_val(self):
        '''Returns the current value.'''
        return self.val.get()

    def activate(self):
        '''Activates the box to allow the user to change it's value.'''
        self.widget.config(state=tk.NORMAL)

    def deactivate(self):
        '''Deactivates the box to prevent the user from changing it's value.'''
        self.widget.config(state=tk.DISABLED)

    def focus(self):
        '''Moves the user's cursor to this box.'''
        self.widget.focus()

    def update_list(self, options, index=None):
        '''reset the values in the option menu

        if index is given, set the value of the menu to
        the option at the given index
        '''
        # check if there's no real update to be made
        # if options == self.options:
        #     return
        # if there is, update the current list
        menu = self.widget["menu"]
        menu.delete(0, len(self.options) - 1)
        self.options = options
        # if there's no choices, set the current value to nothing.
        if self.options == []:
            self.val.set("")
        else:
            self.val.set(options[0])
        for string in options:
            menu.add_command(label=string,
                             command=lambda value=string: self.val.set(value))
        if index is not None:
            self.val.set(options[index])

    def forget(self):
        '''Removes the label from the GUI temporarily.'''
        self.widget.grid_forget()

    def remember(self):
        '''Brings the label back into view.'''
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky, padx=self.padx, pady=self.pady)


class Labels:
    '''
    Class for the various labels in the GUI. useful so we can forget & remember them
    when we need them to be invisable or visable, respectively.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the label is put into.
    :param int column: what column the label is put into.
    :param str default_val: String that first populates the box. [default: ""]
    :param int \*\*padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the check box. [default: 0 px]
    :param int \*\*columnspan: Sets how many columns the label spans. [default: 1]
    :param int \*\*sticky: Sets where the dropdown box populates in it's cell. [default: None]
    '''
    def __init__(self, master, row, column, label, **kwargs):
        self.row = row
        self.column = column
        self.sticky = kwargs.get("sticky", '')
        self.columnspan = kwargs.get("columnspan", 1)
        self.padx = kwargs.get("padx", 3)
        self.pady = kwargs.get("pady", 0)
        self.border = kwargs.get("border", 0)
        self.widget = ttk.Label(master, text=label, border=self.border)
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky,
                         columnspan=self.columnspan, padx=self.padx, pady=self.pady)
        self.widget.configure(background="white")

    def forget(self):
        '''Removes the label from the GUI temporarily.'''
        self.widget.grid_forget()

    def remember(self):
        '''Brings the label back into view.'''
        self.widget.grid(row=self.row, column=self.column, sticky=self.sticky,
                         columnspan=self.columnspan, padx=self.padx)

    def config(self, state):
        '''configures the label. used for changing between DISABLED and NORMAL states.'''
        self.widget.config(state=state)


class ScrollBox:
    '''
    Class for creating a scroll box. Used when you need an integer input from the user in a certain range.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the check box is put into.
    :param int column: what column the check box is put into.
    :param int min_val: Minimum value that the Scroll box can be.
    :param int max_val: Maximum value that the Scroll box can be.
    :param int start_val: Initial value of the box.
    :param int \*\*padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the check box. [default: 3 px]
    :param int \*\*box_width: Sets how wide the input box is [default: 19 px]
    '''
    def __init__(self, master, row, column, min_val, max_val, start_val, **kwargs):
        self.val = tk.StringVar()
        self.val.set(start_val)
        self.min_val = min_val
        self.root = master
        self.max_val = max_val
        self.start_val = start_val
        padx = kwargs.get("padx", 3)
        pady = kwargs.get("pady", 3)
        box_width = kwargs.get("box_width", 19)
        self.widget = tk.Spinbox(master, from_=min_val, to=max_val, textvariable=self.val,
                                 width=box_width)
        self.widget.grid(row=row, column=column, sticky=tk.E, padx=padx, pady=pady)
        self.widget.bind('<KeyRelease>', self._e_check)
        self.widget.bind('<FocusOut>', self._validate)

    def get_val(self):
        '''Returns the current value of the Scroll box.'''
        return self.val.get()

    def set_val(self, value):
        '''Sets the current value of the Scroll box.'''
        self.val.set(value)

    def deactivate(self):
        '''Deactivates the Scroll box to prevent the user from changing it's value.'''
        self.widget.config(state=tk.DISABLED)

    def activate(self):
        '''Activates the Scroll box to allow the user to change it's value.'''
        self.widget.config(state="normal")  # state="readonly")

    def _backspace(self, entry):
        '''Removes the most recently entered character.'''
        cont = self.get_val()
        entry.delete(0, tk.END)
        entry.insert(0, cont[:-1])

    def _e_check(self, e):
        '''Checks for an erronious input.'''
        cont = self.get_val()
        if cont == "":
            self.root.bell()
            return
        if not cont[-1].isdigit():
            self.root.bell()
            self._backspace(self.widget)
            self.widget.focus()

    def _validate(self, e):
        '''Checks all of the values for proper ranges.'''
        cont = self.get_val()
        error = False
        if not cont.isdigit():
            self.widget.delete(0, tk.END)
            self.widget.insert(0, str(self.start_val))
            error = True
        elif int(cont) > self.max_val:
            self.widget.delete(0, tk.END)
            self.widget.insert(0, str(self.max_val))
            error = True
        elif int(cont) < self.min_val:
            self.widget.delete(0, tk.END)
            self.widget.insert(0, str(self.min_val))
            error = True
        if error:
            self.widget.focus()
            messagebox.showerror(title="Invalid Input",
                                 message="Must be a number from {:} to {:}".format(self.min_val, self.max_val))
        # print "looks good captain."


class Button:
    '''
    Class for creating an Entry box. Used when you need a string input from the user.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the check box is put into.
    :param int column: what column the check box is put into.
    :param int min_val: Minimum value that the Scroll box can be.
    :param int max_val: Maximum value that the Scroll box can be.
    :param int start_val: Initial value of the box.
    :param int \*\*padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the check box. [default: 3 px]
    :param int \*\*columnspan: Sets how many columns the label spans. [default: 1]
    :param int \*\*sticky: Sets where the dropdown box populates in it's cell. [default: None]
    '''
    def __init__(self, master, row, column, callback, label, **kwargs):
        self.row = row
        self.column = column
        self.columnspan = kwargs.get("columnspan", 1)
        self.padx = kwargs.get("padx", 3)
        self.pady = kwargs.get("pady", 3)
        sticky = kwargs.get("sticky", "")
        self.widget = ttk.Button(master, command=callback, text=label)
        self.widget.grid(row=self.row, column=self.column, columnspan=self.columnspan,
                         padx=self.padx, pady=self.pady, sticky=sticky)

    def deactivate(self):
        '''Deactivates the button to prevent the user from changing it's state.'''
        self.widget.config(state=tk.DISABLED)

    def activate(self):
        '''Activates the button to allow the user to change it's state.'''
        self.widget.config(state=tk.NORMAL)

    def forget(self):
        self.widget.grid_forget()

    def remember(self):
        self.widget.grid(row=self.row, column=self.column, columnspan=self.columnspan,
                         padx=self.padx, pady=self.pady)


class TimeEntry:
    '''
    Class for creating a time enry box. Used when you need a time input from the user.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the check box is put into.
    :param int column: what column the check box is put into.
    :param dict default_val_dict: Dictionary of values that first populates the boxes.
        [default: {"hour": 12, "minute": 30, "second": 42}]
    :param int padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int pady: This is how much y axis padding is given to the check box. [default: 3 px]
    :param int box_width: Sets how wide the input box is [default: 20 px]
    '''
    def __init__(self, master, row, column, padx=3, pady=3, default_val_dict={"hour": 12, "minute": 30, "second": 42}):
        hour_init_val = default_val_dict["hour"]
        min_init_val = default_val_dict["minute"]
        sec_init_val = default_val_dict["second"]
        self.root = master
        self.frame = tk.Frame(master)
        self.frame.grid(row=row, column=column, padx=3, pady=3, sticky=tk.E)
        self.frame.columnconfigure(0, minsize=20)
        self.frame.columnconfigure(1, minsize=4)
        self.frame.columnconfigure(2, minsize=20)
        self.frame.columnconfigure(3, minsize=4)
        self.frame.columnconfigure(4, minsize=20)
        self.hour_box = EntryBox(self.frame, 0, 0, default_val=hour_init_val, box_width=3, border=1,
                                 justify=tk.CENTER, padx=0, pady=0, sticky=tk.W)
        self.label1 = Labels(self.frame, 0, 1, ":", padx=0, pady=0, border=1)
        # ttk.Label(self.frame, text=":", border=1).grid(row=0, column=1, padx=0, pady=0)
        self.min_box = EntryBox(self.frame, 0, 2, default_val=min_init_val, box_width=3, border=1,
                                justify=tk.CENTER, padx=0, pady=0)
        self.label2 = Labels(self.frame, 0, 3, ":", padx=0, pady=0, border=1)
        # ttk.Label(self.frame, text=":", border=1).grid(row=0, column=3, padx=0, pady=0)
        self.sec_box = EntryBox(self.frame, 0, 4, default_val="{:0>2}".format(sec_init_val), box_width=3,
                                border=1, justify=tk.CENTER, padx=0, pady=0, sticky=tk.E)

        self.sec_box.bind('<KeyRelease>', self._err_sec_check)
        self.min_box.bind('<KeyRelease>', self._err_min_check)
        self.hour_box.bind('<KeyRelease>', self._err_hour_check)

    def _backspace(self, entry):
        '''called when we need to delete the most recent character.'''
        self.root.bell()
        cont = entry.get_val()
        entry.delete(0, tk.END)
        entry.insert(0, cont[:-1])

    def _err_sec_check(self, e):
        '''Error checking function for the first box (the seconds category).'''
        cont = self.sec_box.get_val()
        if cont == "":
            return
        if len(cont) > 2 or not cont[-1].isdigit():
            self._backspace(self.sec_box)
            self.sec_box.focus()
            return
        if int(cont) >= 60:
            self.sec_box.delete(0, tk.END)
            self.sec_box.insert(0, "59")
            self.root.bell()

    def _err_min_check(self, e):
        '''Error checking function for the second box (the minutes category).'''
        cont = self.min_box.get_val()
        if cont == "":
            return
        if len(cont) > 2 or not cont[-1].isdigit():
            self._backspace(self.min_box)
            self.min_box.focus()
            return
        if int(cont) >= 60:
            self.min_box.delete(0, tk.END)
            self.min_box.insert(0, "59")
            self.root.bell()

    def _err_hour_check(self, e):
        '''Error checking function for the third box (the hours category).'''
        cont = self.hour_box.get_val()
        if cont == "":
            return
        if len(cont) > 3 or not cont[-1].isdigit():
            self._backspace(self.hour_box)
            return
        if int(cont) > 24:
            self.hour_box.delete(0, tk.END)
            self.hour_box.insert(0, "24")
            self.root.bell()

    def get_val(self):
        '''Gets the value input by the user.

        :returns dict: Time input by user. Valid keys: hour, minute, second.
        '''
        second = int(self.sec_box.get_val())
        minute = int(self.min_box.get_val())
        hour = int(self.hour_box.get_val())
        return {"hour": hour, "minute": minute, "second": second}

    def deactivate(self):
        '''Deactivates all input boxes.'''
        self.sec_box.deactivate()
        self.min_box.deactivate()
        self.hour_box.deactivate()

    def activate(self):
        '''Activates all input boxes.'''
        self.sec_box.activate()
        self.min_box.activate()
        self.hour_box.activate()

    def bind(self, event, func):
        self.min_box.bind(event, func)
        self.sec_box.bind(event, func)
        self.hour_box.bind(event, func)


class ProgressBar:
    '''
    Class for creating a Progress bar.

    :param master: This is the root variable of the GUI. (*tk.TK()*)
    :param int row: what row the check box is put into.
    :param int column: what column the check box is put into.
    :param int max_val: Maximum value that the progress bar can go to. [default: 100]
    :param int columnspan: Determines how many columns this widget spans. [default: 1]
    :param str mode: "determinate" means we know the current progress of the process this progress bar is
                     tracking. "indeterminate" means we don't know when the current process will end.
                     [default: determinate]
    :param int \*\*padx: This is how much x axis padding is given to the check box. [default: 3 px]
    :param int \*\*pady: This is how much y axis padding is given to the check box. [default: 3 px]
    '''
    def __init__(self, master, row, column, columnspan=1, max_val=100, mode="determinate", **kwargs):
        width = root.winfo_width()
        col, row = root.grid_size()
        width = (width / col)
        padx = kwargs.get("padx", 10)
        pady = kwargs.get("pady", 10)
        self.widget = ttk.Progressbar(master, maximum=max_val, mode=mode, length=str(width * columnspan))
        self.widget.grid(row=row, column=column, columnspan=columnspan, padx=padx, pady=pady)
        self.cur_val = 0
        self.max_val = max_val

    def update(self, val):
        '''
        updates the progress bar based on the current progress of the process running. (val is usually
        a percentage)

        :param int val: Current progress of the process running.
        '''
        if self.max_val < val:
            self.widget.step(self.max_val - self.cur_val)
            self.cur_val = self.max_val
        else:
            self.widget.step(val - self.cur_val)
            self.cur_val = val

    def destroy(self):
        '''
        destroys the widget and removes it from the GUI.
        '''
        self.widget.destroy()


class BusyManager:
    '''
    Allows us to assign the busy pointer to all widgets when
    the program is busy.
    from: http://effbot.org/zone/tkinter-busy.htm
    '''
    def __init__(self, widget):
        self.toplevel = widget.winfo_toplevel()
        self.widgets = {}

    def busy(self, widget=None):
        '''
        attach busy cursor to toplevel, plus all windows
        that define their own cursor.
        '''
        if widget is None:
            w = self.toplevel  # myself
        else:
            w = widget

        if not (str(w)) in self.widgets:
            try:
                # attach cursor to this widget
                cursor = w.cget("cursor")
                if cursor != "watch":
                    self.widgets[str(w)] = (w, cursor)
                    w.config(cursor="watch")
            except tk.TclError:
                pass

        for w in w.children.values():
            self.busy(w)

    def notbusy(self):
        '''restore cursors'''
        for w, cursor in self.widgets.values():
            try:
                w.config(cursor=cursor)
            except tk.TclError:
                pass
        self.widgets = {}
