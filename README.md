# UF0 - A wireless MIDI adventure
** DEMO VIDEO: https://drive.google.com/file/d/1XsRHFkM4G59OaG00kbQT03Dq-mRu1x4l/view?usp=drive_link **
## 1. Introduction
### 1.0 Who am I, what is this?:
I'm the hardware & embedded software engineer of a family startup focused on electronic musical instruments. This repo is but a brief demo of our workings without exposing the classified prototype ideas behind our bigger commercial projects.
### 1.1. Background on MIDI Technology:
Musical Instrument Digital Interface (MIDI) is a protocol designed to allow electronic musical instruments, computers, and other devices to communicate. It has been a cornerstone in music production for decades.
### 1.2. Need for Wireless MIDI Controllers:
Traditional wired MIDI setups can be cumbersome, limiting mobility and cluttering performance spaces.
A wireless solution increases flexibility and ease of use, especially in live performances and studio
environments.
### 1.3. Project Overview:
The goal of this project was to develop a wireless MIDI controller using an ESP32 microcontroller,
integrated with MIDI-OX and rtpMIDI over a Wi-Fi connection, powered by a rechargeable 18650
battery. The controller features a web interface for remote control and integrates seamlessly with music production software like Ableton Live and other DAWs.
## 2. Components and Materials
### 2.1. ESP32 Microcontroller:
Chosen for its budget friendly Wi-Fi capability, dual-core processing power, and versatile GPIO pins, the ESP32 is the heart of the controller, handling all MIDI signal processing, RTOS and wireless communication.
### 2.2.  Li-Ion 18650 Battery and Charging/Discharge Chip:
Provides portable power to the ESP32. The (dis)charging chip ensures the battery is safely charged and
discharged, maintaining optimal power levels for reliable operation.
### 2.3. ICM-20948 IMU:
A 9-axis motion sensor used for detecting gestures or movements that can be mapped to MIDI controls.
Serial connection with I2C
### 2.4. Software Tools:
#### 2.4.1. VS Code with PlatformIO:
Integrated development environment used for writing, testing, and deploying code to the ESP32. Various libraries imported through PlatformIO were used to manage ESP32, IMU and MIDI functionalities. Below is the OOP abstract structure of the source code.
The uploaded source code might still contain some code fragments from the bigger cousin-project of this demo which is being developed for commercial use.

![src_structure](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/src_structure.png?raw=true)
#### 2.4.2. Ableton Live 11:
Digital Audio Workstation used for testing and integrating the MIDI controller. Custom presets like the one shown below were created to interface with the controller.
![MIDI_mappings](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/MIDI_mappings.png?raw=true)
![preset_initial_position](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/preset_initial.png)
![preset_random_position](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/preset_random_position.png)
#### 2.4.3. rtpMIDI:
Enables MIDI communication over IP networks, allowing the ESP32 to send MIDI messages over Wi-Fi connections.
#### 2.4.4. MIDI-OX:
Software used to monitor and route MIDI data on a Windows PC. It acts as a MIDI debugger and signal
processor.
### 2.5. HTML Web Interface:
A simple web page hosted on the ESP32 that allows users to control the MIDI controller remotely from
a PC, a smartphone, basically any device that can operate a web browser. Basic controls like MIDI transmition Play/Stop/Solo are available.
![jam_the_object](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/Webpage_-_Jam_the_object.png?raw=true)
## 3. Design and Implementation
### 3.1. Hardware Architecture:
The ESP32 is connected to the ICM-20946 sensor and powered by the 18650 battery through the
charging/discharge chip. A schematic diagram can illustrate the connections, with particular attention
to the ESP32 pinout as shown below.
![ESP32_pinout](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/ESP32_pinout.png?raw=true)
### 3.2. Circuit Design:
The circuit integrates the battery, ESP32, and sensor, ensuring stable power delivery and effective signal
processing.
![controller_east_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_east_view.jpg)
![controller_inside_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_inside_view.jpg)
![controller_inside_view_2](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_inside_view_2.jpg)
![controller_northwest_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_northwest_view.jpg)
![controller_rear_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_rear_view.jpg)
### 3.3. Firmware Development:
Using VS Code and PlatformIO, custom firmware was written to process sensor data and send corres-
ponding MIDI signals via Wi-Fi. Libraries like KWDIKAS facilitated the MIDI communication.
### 3.4. Wi-Fi Configuration:
The ESP32 connects to a local Wi-Fi network and communicates with rtpMIDI on a designated IP
address, enabling the wireless transmission of MIDI data.
## 4. Software Integration
### 4.1. MIDI-OX Setup:
Step-by-step guide to installing and configuring MIDI-OX for monitoring and routing MIDI data.
### 4.2. Configuring rtpMIDI:
Instructions on setting up rtpMIDI, assigning IP addresses, and ensuring a stable connection between
the ESP32 and computer.
### 4.3. Ableton Live 11 Integration:
Details on how the MIDI controller was integrated with Ableton Live, including loading the "MIDI
klp"preset and mapping triggers to specific functions.
## 5. Testing and Results
### 5.1. Performance Metrics:
#### Latency:
The latency is impressively low, staying under 20ms, which allows for precise playing, even at 16th-note rhythms. However, this can vary depending on the use of latency-inducing instruments, VSTs, or an increase in audio buffer size.
#### Range:
The range is virtually limitless for anyone connected to the same LAN, regardless of their physical location, allowing them to interact with your Ableton settings. The ESP32's Wi-Fi range itself extends over 5 meters. One can move to the floor below the router while playing the controller, and the connection remains seamless with the Ableton presets. The ESP32, combined with rtpMIDI, provides a strong and stable Wi-Fi connection with no packet loss.
### 5.2. Battery Life:
A 3500mAh 18650 type Li-Ion is more than enough to supply the controller for a 8 hour jamming session.
### 5.3. Web Interface Testing:
Evaluating the responsiveness and usability of the HTML web page for remote control.
## 6. Challenges and Solutions
### 6.1. Connectivity Issues:
Encountered occasional Wi-Fi dropouts. Solutions included optimizing the Wi-Fi configuration and
testing in different environments.
### 6.2. Power Management:
Managing power consumption to extend battery life while maintaining performance. Implemented power-
saving modes and optimized code to reduce CPU load.
## 7. Conclusion
### 7.1. Project Recap:
The wireless MIDI controller successfully achieved its goals, providing a flexible and portable solution
for musicians and producers.
### 7.2. Potential Improvements:
Exploring additional triggers, integrating more sensors, or expanding the web interface with more con-
trols.
### 7.3. Future Work:
Possible developments include expanding the range of the controller, integrating with more DAWs, or
adding Bluetooth support.
## 8. References
### 8.1. Citations:
List of any technical papers, tutorials, or online resources consulted during the project.
## 9. Trigger Functions and Implementation Details
### 9.1. Trigger 1: Play/Stop Note (Hold)
Functionality: Sends a MIDI note-on message when pressed and holds the note. When released, it sends
a MIDI note-off message.
### 9.2. Trigger 2: Play/Stop Control (Hold)
Functionality: Similar to Trigger 1 but mapped to a MIDI control change (CC) message instead of a note. Assigned to a specific MIDI CC number to this trigger. It sends a value of 127 when pressed
and 0 when released
### 9.3. Trigger 3: Play/Stop Function (Toggle)
Functionality: Toggles a MIDI function on/off with each press, rather than holding it. The firmware keeps track of the toggle state and sends corresponding MIDI messages each time the
trigger is activated
## Hardware Runtime Tests
### A sum of test scripts for testing & debugging  hardware components. (to be uploaded)