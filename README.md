# wakey-wakey-alarm
 
# Wakey-Wakey Alarm: Open Source (1.0)

The Wakey-Wakey Alarm is an Arduino Uno R3-based alarm system designed to make waking up a challenge! 🚀
It ensures you’re outta bed by using a buzzer and LEDs, and it won’t stop until you press the button and interact with the obstacle avoidance sensor while your Lights are on 💡.

## Table of Contents

- [Features 🚀](#Features)
- [Hardware-requirements 💻](#Hardware-requirements)
- [How to Setup? 🔧](#Deployment)
- [Usage 💡](#Usage)
- [License©️](#License)

## Features

- Set customizable date and time with code (Soon adding UI in Beta version) 📅
- Buzzer won't stop until you interact with system 🔊
- 10 mins Snooze functionality with IR Remote 💤
- Blinking Leds for support in Dark 🚨
- Light sensor that will automatically stop when lights are on!!💡


## Hardware-requirments

- Arduino Uno R3 x1
- LDR(Light Dependent Reistor) x1
- IR Obstacle Avoidance sensor x1
- Buzzer x1
- Momentary Switch x1 
- IR Receiver(i.e. `TSOP38238`) x1
- IR Remote x1
- LEDS x5
## How to Setup?

To Use this Wakey-Wakey Alarm system:

```bash
  git clone https://github.com/Nothig-007/wakey-wakey-alarm.git
```
- Open the **wakey-wakey-alarm.ino** file in the Arduino IDE.
- Upload the code to your Arduino board.
- Connect components using the **schematicView.pdf** file or **breadboardView.pdf** .
- Setup the date and time using **wakey-wakey-alarm.ino** (using integer section at the beginning of the file to modify the time and date)



## Usage
- Set the alarm time by modifying the `current_hr`, `current_mins`, `current_date`, `input_hrs`, `input_mins`, and `input_date` variables in the code.
- Use the IR remote to snooze the alarm.
- Place the LDR in a bright location for light based alarm deactivation.
## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for more details.
## Acknowledgements

-Special thanks to my efforts
