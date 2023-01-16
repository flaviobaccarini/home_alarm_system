# HOME ALARM SYSTEM WITH ARDUINO
## The project
This project is an Home Alarm System made with [Arduino Nano 33 IOT](https://docs.arduino.cc/hardware/nano-33-iot).
The sensor used for the project is a simple PIR sensor that is activated if the correct password is 
typed on the keypad. If the PIR sensor detects some movements it triggers a passive buzzer. 
Arduino Nano 33 IOT has a WiFi module that is used in this project. Arduino can make a phone call on the smartphone.
The WiFi module is exploited, in fact, because Arduino makes a POST HTTP request and [IFTTT](https://ifttt.com/) sends to the user
an alarm phone call to notice the Alarm System is activated.
Here we can see the schematics:
<p align="center">
<img src="https://github.com/flaviobaccarini/home_alarm_system/blob/main/schematics/SCHEMATICS_bb.svg" alt="Schematics" width="500"/>
</p>
Three LEDs (red, yellow, green) are used to control the system: for example if the password typed by the user is wrong
the red LED blinks, instead if it's correct the green LED blinks. If the yellow LED is ON it means that the system waits
some inputs from the user.
The white LED is for the WiFi connection: if the LED is ON Arduino is correctly connected to the WiFI.
The LCD display is used to visualize some messages about the system status.
In the folder [motion_sensor_alarm_with_wifi](https://github.com/flaviobaccarini/home_alarm_system/tree/main/motion_sensor_alarm_with_wifi) we can find
all the C++ code for programming the Arduino board.

## Finite State Machine
The system is modeled as Finite State Machine (FSM).
A FSM is a mathematical model of computation. It is an abstract machine that can be in exactly one of a finite number of states at any given time.
The FSM can change from one state to another in response to some inputs; the change from one state to another is called a transition. 
An FSM is defined by a list of its states, its initial state, and the inputs that trigger each transition.

All the states for the system:
1) OFFLINE: initial state. The system is OFFLINE, so the PIR isn't searching for some movements.
2) WAITING: transitional state between different states.
3) PIR ACTIVATED: the PIR is now activated and look for some movements to detect.
4) BUZZER ACTIVATED: if the PIR detects some movements it triggers the buzzer. In this state the speaker is activated.
5) INCORRECT: state if the user types the wrong password.

## Requirements
Two libraries were used for this project:
1) [Adafruit Keypad](https://www.arduinolibraries.info/libraries/adafruit-keypad)
2) [Password library](https://playground.arduino.cc/Code/Password/)

## Working principle
To see better how the Alarm System works please see the folder [slides](https://github.com/flaviobaccarini/home_alarm_system/tree/main/slides).
[ELECTRONICS](https://github.com/flaviobaccarini/home_alarm_system/blob/main/slides/ELECTRONICS.pdf) is a presentation for the Alarm System and
in the folder [images_and_videos](https://github.com/flaviobaccarini/home_alarm_system/tree/main/slides/images_video) we can find all the videos
that are present in the static PDF to better understand what is the working principle of the overall system.

<p align="center">
<img src="https://github.com/flaviobaccarini/home_alarm_system/blob/main/slides/images_video/SYSTEM.jpg" alt="Schematics" width="500"/>
</p>
