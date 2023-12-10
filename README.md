# LockBox Secure+


## Introduction to the problem and the solution

Traditional safes are fraught with security vulnerabilities due to reliance on antiquated key-based or basic lock mechanisms, failing to combat modern security threats effectively. Their lack of real-time monitoring, immediate alerts, and visual evidence during unauthorized access attempts creates a void in safeguarding valuables. Owners face challenges in responding promptly to potential threats, amplifying the shortcomings of conventional safes.

Recognizing these limitations, the LockBox Secure+ initiative introduces a revolutionary safe solution. Equipped with a smart lock and Numpad Pad, it offers heightened security over traditional locks. The integrated camera captures access attempts, swiftly transmitting images to owners via secure channels like Telegram. Motion sensors enhance security by detecting tampering, triggering alerts, while an integrated buzzer adds audible notifications. This multifaceted approach empowers users with real-time information, redefining security standards for both residential and commercial settings.


## Hardware design and implementation details

The hardware design and schematic are foundational in shaping the operational dynamics of the envisioned safety box, ensuring its security and functionality. Employing sophisticated tools and technologies, this section reveals the intricate hardware framework contributing to a robust and reliable system. Each selected tool strategically serves a purpose, collectively contributing to the safety box's creation.

Key Components:

![image](https://github.com/irbirojodoh/brankas/assets/87791160/3fd14483-a5e3-48aa-b515-7f5f3e75ec7a)

Safety Box Simulation Box:
Facilitates real-world simulation for testing and validating implemented features.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/7fd0f395-7a27-467f-9c80-6d19ca6de765)

Esp32:
Vital component enabling buzzer connectivity and enhancing overall safety box functionality.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/9b8c6e97-9db6-4231-a3bf-e85e03da3e65)

ESP32 Cam:
Captures images during access attempts, transmitting them securely to the owner via Telegram for heightened security.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/91e374a7-92f5-4a1d-ae81-d4f7045da26b)

Keypad:
Integral mechanism for safe access, offering users a secure and user-friendly input method.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/c28d0738-0928-4777-991a-aa1b435a8f96)

Buzzer:
Provides alerts for specific events like unauthorized access or detected motion.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/3bd8607a-1895-4ace-b338-e3141bad508b)

Accelerometer:
Functions as a motion sensor, detecting movement near the safety box and triggering immediate notifications via the buzzer.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/b78ed3a6-ae35-47be-86c8-7ad927fbf7dd)

Protoboard/PCB Dot Matrix (2):
Crucial for interconnecting hardware elements, ensuring a stable and reliable system setup.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/02ab580d-2f43-405b-ace9-27cc4fd64648)

Servo:
Operates as a controlled lock mechanism through the Esp32, enhancing security measures.

These carefully selected components work synergistically, contributing to the safety box's robustness and functionality. The combination of these tools forms a sophisticated hardware setup, establishing a reliable security framework for safeguarding valuables.


## Network infrastructure

![skematik](https://github.com/irbirojodoh/brankas/assets/87791160/e00709f2-b208-4be7-8740-5add16a8e3ea)


The network infrastructure of the LockBox Secure+ orchestrates a cohesive connection between its key components, ensuring seamless communication and functionality. Here's an outline of the network architecture:

Component Connectivity:

  - Keypad, Servo, Accelerometer, Buzzer to ESP32:
The keypad, servo, accelerometer, and buzzer establish a physical connection to the ESP32 using cables, enabling data transmission and control.

  - ESP32 to ESP32 Cam via ESP Now Connection:
The ESP32 establishes an ESP Now Connection, a low-power, peer-to-peer communication protocol, to link with the ESP32 Cam. This connection facilitates efficient data transfer between the two components.

  - ESP32 Cam to Telegram via Wi-Fi:
The ESP32 Cam connects to Telegram through a Wi-Fi network. Captured images from the ESP32 Cam are promptly transmitted to the Telegram platform, ensuring swift and secure communication with the owner.


## Software implementation details

The software architecture for the LockBox Secure+ comprises two distinct sets of code, one for the ESP32 and another for the ESP32-CAM. These codes enable the functioning and coordination between the various components of the security system.

ESP32 Code:
The ESP32 code encompasses functionalities for managing the keypad, servo, accelerometer, and buzzer operations. It leverages libraries like Keypad, ESP32Servo, and MPU6050. The code establishes an ESP Now Connection with the ESP32-CAM, allowing seamless communication between the two devices.

The ESP32 code handles multiple tasks:

  - Keypad Functionality: It captures input from the keypad, verifying the entered PIN against the predefined correct PIN.

  - Motion Detection: Utilizing the MPU6050 accelerometer, it detects motion around the safe. Upon detection, it triggers actions such as activating the buzzer and notifying the ESP32-CAM to capture a photo for further security validation.

  - Buzzer Control: This manages the buzzer's activation for various events, enhancing the security alert system.

  - ESP Now Communication: Facilitates communication between ESP32 and ESP32-CAM, allowing the transmission of specific commands or alerts.


ESP32-CAM Code:
The ESP32-CAM code integrates with the camera and establishes connectivity with Telegram for remote monitoring and alerts. It utilizes ESP Now to communicate with the ESP32, receiving commands or notifications regarding security events.

The ESP32-CAM code performs the following key functions:

  - Camera Initialization: Initializes the camera module, configuring parameters for capturing images based on predefined specifications.

  - Wi-Fi Connectivity: Connects to a specified Wi-Fi network to enable communication with the Telegram platform for sending captured images.

  - Telegram Integration: Utilizes the UniversalTelegramBot library to interact with Telegram. It enables receiving commands from a predefined chat and sending captured photos to authorized users upon request.

  - ESP Now Communication: Receives messages and notifications from the ESP32, triggering actions such as capturing photos upon detecting security breaches or suspicious activities.

Telegram Integration Process:

Our software development approach creatively integrates Telegram as both the user interface and monitoring system for our safety box, avoiding the need for a separate software interface. This integration revolves around two crucial identifiers: the Bot Token (an authentication key issued by Telegram's BotFather) and the Chat ID (a numerical address for message destinations).

![image](https://github.com/irbirojodoh/brankas/assets/87791160/4f514d3b-2a2b-49fa-bb7c-7c222f46de79)
![image](https://github.com/irbirojodoh/brankas/assets/87791160/0d3da6e8-979b-4747-b4c3-0feef3af3e9b)

We initiated the connection by creating a Telegram bot via BotFather, which provided a Bot Token for authentication. Conversing with the bot on Telegram helped us obtain the Chat ID, specifying the recipient of messages or media.

Incorporating these identifiers into the ESP32-CAM code enabled seamless communication between the device and Telegram. This allowed the ESP32-CAM to send messages and images to the designated chat effortlessly. We integrated the camera module's code, enabling the ESP32-CAM to capture and transmit photos.

![image](https://github.com/irbirojodoh/brankas/assets/87791160/dd60325f-315f-4adc-8554-e409f05225dc)

Upon capturing a photo, we utilized the Telegram API to send the image to the specified chat by embedding the Bot Token and Chat ID within the code.

Outcomes:

  - Successful ESP32-CAM integration with Telegram, enabling photo transmission to the designated Chat ID.
  - Implementation of key functionalities for photo capture and secure communication between the ESP32-CAM and Telegram.
    

## Test results and performance evaluation

Testing Results :

Motion Detection Testing:

![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/b0ae074b-3733-478f-b36a-2712fc3b8f61)

  - Motion detection functioned accurately, triggering alerts when significant motion was detected.
  - Images were promptly captured and transmitted, enhancing the security aspect of the system.
    
Keypad Input and Servo Testing:

![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/0652d282-df91-47ad-a808-bb5f8d3b064c)
![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/0cc3a9ea-d1d3-4b48-84f9-5f6ab3e92316)
![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/10d4e267-5c60-4ac6-ac30-3ddc2f9fb66e)

  - Keypad inputs were effectively recognized, granting access with correct codes and denying unauthorized entries.
  - The servo responded reliably to valid inputs, efficiently locking and unlocking the safe.
    
ESP Connection Testing:

![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/48c19344-5f57-4465-afd5-a6f00d4fb35f)

  - ESP32 devices demonstrated consistent and reliable communication through the ESP Now connection.
    
Telegram Integration Testing:

![image](https://github.com/irbirojodoh/Lockbox-SecurePlus/assets/87791160/b5d03bb5-5b9c-45d8-94ad-01e4f613e3c3)

  - Telegram integration was successful, allowing the system to send images and notifications promptly.
  - Notifications were received in a timely manner on the designated device.

In our project evaluation, we've identified key areas for improvement. 
First, ensuring consistent power is crucial; occasional instability due to the current power source affects components like the buzzer and accelerometer. Exploring more reliable power sources is essential for stable operation.
Second, we plan to replace the servo with a solenoid and spring for a more robust locking mechanism, improving security and simplifying the locking process.
We aim to add features for remote management, allowing locking/unlocking via Telegram and a status monitoring system for real-time feedback.
Additionally, we'll integrate remote image capture for enhanced surveillance. These improvements target functionality, security, and user experience, aiming to elevate effectiveness and user satisfaction.
    
## Conclusion and future work

In conclusion, the LockBox Secure+ project stands as a successful and innovative IoT solution for securing valuables. The Numpad interface ensures reliable access control, while the vibration sensor significantly bolsters security by detecting unauthorized movements. Seamless integration with Telegram enables swift alerts and image transmission, meeting acceptance criteria effectively.
Looking ahead, future enhancements could focus on refining the locking mechanism, adding more security features, and optimizing power management. Leveraging ESP-NOW, MPU6050 sensor, and FreeRTOS demonstrates the project's technical robustness.
The project's success validates its practicality and effectiveness, offering scope for ongoing improvements and customization to cater to diverse user needs. LockBox Secure+ not only fulfills its primary objective but also paves the way for continuous evolution, maintaining its position as a leading IoT security solution.
