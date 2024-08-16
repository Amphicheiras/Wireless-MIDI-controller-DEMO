# UF0 - A wireless MIDI adventure
** DEMO VIDEO: https://drive.google.com/file/d/1XsRHFkM4G59OaG00kbQT03Dq-mRu1x4l/view?usp=drive_link **

** In the demo video I reset the ESP32 controller to boot up fresh, then leave the controller on the leveled surface of my desk to calibrate, and then I jam my Ableton preset with the controller. The bandpass frequency filter is my DIY DSP filter made with JUCE shown [here](https://github.com/Amphicheiras/BandpassFilter).**
## 1. Introduction
### 1.0 Who am I, what is this?:
I'm the hardware & embedded software engineer of a family startup focused on electronic musical instruments. This repo is a brief demo of our workings without exposing the classified prototype ideas behind our bigger commercial projects.
### 1.1. Background on MIDI Technology:
Musical Instrument Digital Interface (MIDI) is a protocol designed to allow electronic musical instruments, computers, and other devices to communicate. It has been a cornerstone in music production for decades.
### 1.2. Need for Wireless MIDI Controllers:
Traditional wired MIDI setups can be cumbersome, limiting mobility and cluttering performance spaces.
A wireless solution increases flexibility and ease of use, especially in live performances and studio environments.
### 1.3. Project Overview:
The goal of this project was to develop a wireless MIDI controller using an ESP32 microcontroller, integrated with MIDI-OX and rtpMIDI over a Wi-Fi connection, powered by a rechargeable 18650 battery.
The controller features a web interface for remote control and integrates seamlessly with music production software like Ableton Live and other DAWs.
## 2. Components and Materials
### 2.0. Market Research and Cost:
As a DIY project, I aimed for budget friendly solutions, at least for the research part, there werent many compromises regarding component quality, and where it really mattered, there was no compromise at all (ICM-20948 chip).
### 2.1 Caseing:
For the sake of this demo I crafted a carton caseing to hold the hardware together. This is a temporal solution.
For the bigger projects, there are 3D files for 3D printed caseings which unfortunately have not been printed yet.
### 2.2. ESP32 Microcontroller:
Chosen for its budget friendly Wi-Fi capability, dual-core processing power, and versatile GPIO pins, the ESP32 is the heart of the controller, handling all MIDI signal processing, RTOS and wireless communication.
### 2.3. Li-Ion 18650 Battery and Charging/Discharge Chip:
Provides portable power to the ESP32. The (dis)charging chip ensures the battery is safely charged and
discharged, maintaining optimal power levels for reliable operation.
### 2.4. ICM-20948 IMU:
A 9-axis motion sensor connected via I2C serial connection, used for detecting gestures or movements that can be mapped to MIDI controls.
### 2.5 Other Hardware:
Other passive hardware such as buttons, LEDs, vibrators, etc. There is code for the bigger projects that already integrates these components.
### 2.6. Software Tools:
#### 2.6.1. VS Code with PlatformIO:
Integrated development environment used for writing, testing, and deploying code to the ESP32. Various libraries imported through PlatformIO were used to manage ESP32, IMU and MIDI functionalities. Below is the OOP abstract structure of the source code.
The uploaded source code might still contain some code fragments from the bigger cousin-project of this demo which is being developed for commercial use.

![src_structure](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/src_structure.png?raw=true)
#### 2.6.2. Ableton Live 11:
Digital Audio Workstation used for testing and integrating the MIDI controller. Custom presets like the one shown below were created to interface with the controller.
![MIDI_mappings](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/MIDI_mappings.png?raw=true)
![preset_initial_position](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/preset_initial.png)
![preset_random_position](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/preset_random_position.png)
#### 2.6.3. rtpMIDI:
Enables MIDI communication over IP networks, allowing the ESP32 to send MIDI messages over Wi-Fi connections.
#### 2.6.4. MIDI-OX:
Software used to monitor and route MIDI data on a Windows PC. It acts as a MIDI debugger and signal
processor.
### 2.7. HTML Web Interface:
A simple HTML web page is hosted on the ESP32 allowing users to control the MIDI controller remotely from
a PC, a smartphone, basically any device that can operate a web browser. Controls like MIDI transmition play, stop, and solo are available and also an option so save the current, or load the last MIDI configuration.
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
![controller_northwest_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_northwest_view.jpg)
![controller_rear_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_rear_view.jpg)
![controller_inside_view](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_inside_view.jpg)
![controller_inside_view_2](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO/blob/MIDI-controller/media/controller_inside_view_2.jpg)
### 3.3. Firmware Development:
Using VS Code and PlatformIO, custom firmware was written to process sensor data and send corresponding 
MIDI signals via Wi-Fi. Libraries like AppleMIDI facilitated the MIDI communication.
### 3.4. Wi-Fi Configuration:
If the ESP32 cannot connect to a WiFi network, it starts in WiFi setup mode. In this mode it acts as a router in which one must connect to the device and enter the network's credential for the controller to connect to. Once the correct credentials are entered the controller connects to the Wifi network with automatic IP address assignment. Then the ESP32 communicates with rtpMIDI on its designated IP address, enabling the wireless transmission of MIDI data.
## 4. Software Integration
### 4.1. Configuring rtpMIDI:
To use the rtpMIDI you must first create a session for your computer, then create a directory which contains the controller's info such as Name, IP-address and Port and then just press the connect button. The rtpMIDI has a built in latency gauge but I find it is not very precise since is measures the latency based on the signals transmitted from the controller, so if the actual latency is for example 50ms but a MIDI signal is transmitted every 1000ms then the latency gauge will write 1000ms.
the ESP32 and computer.
### 4.2. Ableton Live 11 Integration:
Using AppleMIDI, the controller can behave as a MIDI device. Then, on the Ableton settings I use a MackeControl surface with input from the computer's rtpMIDI session created in the previous step. Then, Ableton can see the MIDI signals from every MIDI channel the controller is transmitting. Then one can map each MIDI CC from the different channels in whatevere parameter he likes using the MIDI Mappings editor in Ableton. After you setup your mappings you can just save the project and the mappings will load automatically when loading the project.
## 5. Testing and Results
### 5.1. Performance Metrics:
#### Latency:
The latency is impressively low, staying under 20ms, which allows for precise playing, even at 16th-note rhythms. However, this can vary depending on the use of latency-inducing VSTs, or increased audio buffer size.
#### Range:
The range is virtually limitless for anyone connected to the same LAN, regardless of their physical location, allowing them to interact with your Ableton settings. The ESP32's Wi-Fi range itself extends over 5 meters. One can move to the floor below the router while playing the controller, and the connection remains seamless with the Ableton presets. The ESP32, combined with rtpMIDI, provides a strong and stable Wi-Fi connection with no packet loss.
### 5.2. Battery Life:
A 3500mAh 18650 type Li-Ion is more than enough to supply the controller for a 8 hour jamming session.
### 5.3. Web Interface Testing:
The webpage intercommunication is seamless and stable.
## 6. Challenges and Solutions
### 6.1. Connectivity Issues:
One sort of an issue is that when a device occupies the IP address of the controller's last session, then the controller will assign itself a different IP address. Then you will have no way of getting that address and you will have to setup the connection again. There is research on working around this issue. Since minimum latency is required, hosting a very hitech website with lots of processing power would drain the cpu power and battery of the ESP32 so for now the webpage will stay as minimal as possible.
### 6.2. Power Management:
The radio transmition is the most demanding process on the ESP32. There are builtin ESP32 power options regarding WiFi usage, closing and opening the WiFi antenna and lowering the rate at which packages are being sent. If there is a stable latency of 20ms, then the ESP32 can sleep for most of the time, wake up to transmit or receive and then fall asleep again. These techniques are being researched for optimum power performance. The code it self is quite lite, and the processing power needed for it is of minimal impact on the battery life.
## 7. Conclusion
### 7.1. Project Recap:
The wireless MIDI controller successfully achieved its goals, providing a flexible and portable solution
for musicians and producers.
### 7.2. Potential Improvements:
Integrated additional buttons & functions, integrating more sensors & HID, or expanding the web interface with more controls.
### 7.3. Future Work:
3D printed casing.
## 8. References
### 8.1. Citations:
List of any technical papers, tutorials, or online resources consulted during the project.
## 9. Trigger Functions and Implementation Details
### 9.1. Trigger 1: Play/Stop Note (Hold)
Functionality: Sends a MIDI note-on message when pressed and holds the note. When released, it sends a MIDI note-off message.
### 9.2. Trigger 2: Play/Stop Control (Hold)
Functionality: Similar to Trigger 1 but mapped to a MIDI control change (CC) message instead of a note. Assigned to a specific MIDI CC number to this trigger. It sends a value of 127 when pressed and 0 when released
### 9.3. Trigger 3: Play/Stop Function (Toggle)
Functionality: Toggles a MIDI function on/off with each press, rather than holding it. The firmware keeps track of the toggle state and sends corresponding MIDI messages each time the
trigger is activated
## Hardware Runtime Tests
### A sum of test scripts for testing & debugging hardware components.
