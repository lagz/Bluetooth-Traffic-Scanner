Hello, thank you for showing interest in our project. This project demonstrates a pretty standard IoT application utilizing an ESP32
and a Raspberry Pi microcontroller. The ESP32 is known as the Node computer, and the Pi is the Base Station for reference in this project.
In order for this to function as intended, one must have a Node and a Base Station computer. Firstly, ensure that the python files, 
basestation.py, and mqttsub.py are installed and running on the Base Station computer. Next flash our arduino compatable microcontroller 
with the NodeFlashFile.ino program. All of these files need to be configured with the proper IP addresses in order to ensure correct functionality.
All of these devices also need to be held on the same network. This project utilized the Node computer to detect all the exposed bluetooth 
endpoints within an area, and then translates the number of devices and their MAC addresses back to the Base Station computer, where the data
is then processed and posted to a local HTML server, viewable from a local web browser on the same network by typing in the IP and Port set
to host the HTML server.
