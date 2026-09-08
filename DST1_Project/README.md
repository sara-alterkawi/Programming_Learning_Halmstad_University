# Smart Greenhouse Project
Description:
○ The aim of this project is to monitor and control a Greenhouse climate.
○ Temperature and Light intensity are to be controlled.
○ The temperature must be maintained between 20 - 25 Celsius.
System components:
● Microchip (Atmel) SAM3X8E – ARM embedded computer platform (Arduino due)
● Keypad (Port I/O)
● Photosensor (A/D)
● Temperature sensor (time)
● RC servo motor (PWM)
● Switch (HW-int)
● Display (port I/O)
● LEDs
Requirements:
Req. 1: The system should have a calendar that allows showing date and time. The user should be able to configure it whenever it is needed using the keypad and display. The date should be
represented as DD/MM/YYYY, while the clock as hh:mm:ss. (Use the 24-hour clock system). SysTick can be used as a base for the calendar.
Purpose: timestamp
Req. 2: Periodic temperature recording every minute for a duration of 7 days. The recorded temperature should be time-stamped with the time moment that it was measured. During
recording the temperature, if the system memory buffer is full, the recording should manage to delete an old recording and add a new one instead (deleting an old node from the Linked List
and adding a new one instead). Linked Lists must be used as a data structure to hold the recorded information.
Purpose: Recording of temperature.
Req. 3: Presentation of recorded data on the LCD by text. Each day is presented by the minimum, average, and maximum values for temperature. Maximum and minimum values
should be presented along with their timestamp.
Purpose: Presentation of logged data.
Req. 4: The greenhouse has a large horizontal glass roof surface, shades and a motorcontrolled mirrors that faces the sun to reflect the sun light through the glass roof to the plants
inside. The motorized mirrors tracks the sun (with the help of the photosensor) to reflect it inside the green house, while the shades control the amount of light that enters the greenhouse by
turning it on and off. For the healthy growth, the plants should have 16 hours of light and 8 hours of darkness every day. Unfortunately, the required ratio of light to dark cannot be
maintained naturally since sun light duration varies drastically between summer and winter in Sweden. To tackle this issue, a lighting system is installed (modelled by the LED) to
compensate the lack of sunlight in winter. The system should track the sun and control the mirrors and shades along with the lighting system (LED) to achieve the healthy growth light to
darkness ratio for the plants. The sun can be simulated by any bright light source and the lighting system can be modeled by the LED. The implemented system should show on the
display the position of the sun (in degrees), the number of hours of sun light, the number of hours of Lighting system, the number of hours of darkness, the plant got in the current moment
(Problem, to achieve this req. Should operate in periodically)
Purpose: light to darkness ratio, Sun position
Req. 5: The greenhouse temperature should be maintained between a defined upper and lower limit. An alarm should be raised in case these limits were crossed. The alarm signal could be
turning on a led or showing a blinking alarm message on the lcd. The alarm signal should be kept on until the user acknowledge its occurrence and resets it manually. The upper and lower
limits must be configurable by the user through the keypad and the lcd display.
Purpose: Alarm at high or low temperature.
Req. 6: Create a fast mode that achieves Req. 2 where each 30 minutes are simulated by a second.
Purpose: High speed simulation to simplify testing.
Req. 7: Draw a functional block diagram (Not a flowchart or schematic or wiring diagram), which illustrates graphically how the system is composed. The functional block diagram should be
included in the project documentation (not as a separate file). Also, the diagram should be followed by a paragraph that explains it in detail.
Purpose: Illustration of how things are connected from a higher abstract view.
Req. 8: Documentation of the project. A report which specifies the modules of the system, with a clear connection to the project requirements, so that it is possible to track which requirements
are achieved. The documentation is a part of the deliverables for the final exam. Missing the submission of the documentation before the mentioned deadline means failure.
Purpose: Document what you have done according to the project instructions.
