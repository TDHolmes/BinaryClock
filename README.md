# Binary Clock
A simple but well designed clock that displays the time in [binary coded decimal][BCD wiki link]

## PCB

__PCB picture goes here__

The current design uses an Atmel [ATtiny4313][attiny link] microcontroller in conjunction with a Maxim [DS3231M][ds3231m link] [RTC][RTC wiki link] for accurate time keeping. The LEDs are driven with a [STP16CPC26][stp16cpc26 link] constant current LED driver along with [high-side MOSFETs][mosfet wiki link] for [multiplexing][multiplexing wiki link].

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
[PCB picture]: PCB link goes here "Binary Clock v1.0"
[RTC wiki link]: https://en.wikipedia.org/wiki/Real-time_clock
[mosfet wiki link]: https://en.wikipedia.org/wiki/MOSFET#Single-type
[multiplexing wiki link]: https://en.wikipedia.org/wiki/Multiplexing
[GUI picture]: https://raw.githubusercontent.com/TDHolmes/BinaryClock/master/documents/pictures/BinaryClockGUI.png "Binary Clock GUI v1.0"
[UART wiki link]: https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter
