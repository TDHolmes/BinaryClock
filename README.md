# Binary Clock
A simple but well designed clock that displays the time in [binary coded decimal][BCD wiki link]

## PCB

![alt text][PCB picture]

The current design uses an Atmel [ATtiny4313][attiny link] microcontroller in conjunction with a Maxim [DS3231M][ds3231m link] [RTC][RTC wiki link] for accurate time keeping. The LEDs are driven with a [STP16CPC26][stp16cpc26 link] constant current LED driver along with [high-side MOSFETs][mosfet wiki link] for [multiplexing][multiplexing wiki link].

## LED Driving Scheme

The LEDs are driven by multiplexing the rows and columns. The columns are dirven individually via PMOS transistors. The rows are daisy chained and driven by the [STP16CPC26][stp16cpc26 link]. The columns are refreshed at an effective rate of 360 Hz to be equivallent to 60 Hz refresh rate.

In order to get more than basic red, green and blue combinations, the LEDs have four time slots to create more color combinations. So in reality, the LEDs are updated at (60 Hz) * (6 columns) * (4 bit color) = 1440 Hz.

Below shows the connectivity of the LEDs to the driver chips.

![alt text][Multiplex - off]

Below is a snapshot in time when the minute ones column is active.

![alt text][Multiplex - on]

Below is a gif of multiplexing in action.

![alt text][Multiplex - gif]

## GUI
The user interface communicates to the PCB via a [UART][UART wiki link] through a USB to serial converter chip. The user interface can do a few things:
- set the time to the current computer system time
- set a custom time
- set one or all LEDs to a certain color
- clear one or all LEDs

![alt text][GUI picture]


[BCD wiki link]: https://en.wikipedia.org/wiki/Binary-coded_decimal
[attiny link]: http://www.atmel.com/devices/ATTINY4313.aspx
[ds3231m link]: https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231M.html
[stp16cpc26 link]: http://www.st.com/content/st_com/en/products/power-management/led-drivers/led-array-drivers/stp16cpc26.html
[Multiplex - off]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/binaryClock_multiplexLogic_notOn.png "Diagram of the connections if the LEDs"
[Multiplex - on]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/binaryClock_multiplexLogic_on.png "Diagram of a snapshot when the minute ones column is active."
[Multiplex - gif]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/binary_clock_demo.gif "Live gif of the multiplexing."
[PCB picture]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/BinaryClock_front_final.jpeg "final Binary Clock displaying 12:35:47"
[RTC wiki link]: https://en.wikipedia.org/wiki/Real-time_clock
[mosfet wiki link]: https://en.wikipedia.org/wiki/MOSFET#Single-type
[multiplexing wiki link]: https://en.wikipedia.org/wiki/Multiplexing
[GUI picture]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/BinaryClockGUI.png "Binary Clock GUI v1.0"
[UART wiki link]: https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter
