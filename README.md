# arduino_servo_parser
## Description
This Arduino program parses a string input into appropriate values for servo control. For an example string input of 0.1.1000.2000.90, the servo pin is enabled at pin 0, its maximum and minimum pulse width are set respectively to 1 ms and 2 ms, and its position is set at 90 degrees. It can be used independently as a debugging tool, or used as an way to facilitate communication through the serial port between computer programs such as ROS and Unity and the Arduino board.
## Dependencies
This program uses the Adafruit PWM Servo Shield and its accompanying library. Using the default Servo.h library makes controlling multiple servos complicated, and therefore not recommended.  
## Uses
The program will only parses string that has the form of [pin].[enabled].[min_pulse_width].[max_pulse_width].[position]. [pin] is limited from 0 to 50, [enabled] can only be either 0 (disabled) or 1 (enabled), [min_pulse_width] and [max_pulse_width] are limited from 0 microseconds to 3000 microseconds, and [position] is limited from 0 to 180. Any other string are not excepted, and will disable servos.