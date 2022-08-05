# FlightSimThrottle

_____________________________________
Thingiverse joystick files: https://www.thingiverse.com/thing:5440863 <br/>
Thingiverse light panel files: https://www.thingiverse.com/thing:5449724
Video preview of basic throttle: https://www.youtube.com/shorts/Dck7o5My2vg <br/>

To use either the basic throttle control or the control with extras you will need to download the joystick library: https://github.com/MHeironimus/ArduinoJoystickLibrary <br/>
Once downloaded you will need to drag the zip into the correct file. In the arduino IDE select sketch > include library > add .ZIP library. from there select the zipped Joystick library. 
NOTE: to use this project and the joystick library you will need a compatible arduino board, an in depth list of compatible boards can be found on the joystick library github page. I am using an official arduino micro.
![add library](https://user-images.githubusercontent.com/52784821/181112569-012a6443-702c-403d-8ca1-a49eeda75db4.png)

_____________________________________
The Arduino schematic for the basic throttle control can be seen below.

![schematic](https://user-images.githubusercontent.com/52784821/181105736-2a00b1b9-e485-41f2-888d-2ea1aba94c9e.png)

_____________________________________
The Arduino schematic for the throttle control extras can be seen below.<br/>
This includes the orginal throttle control aswell as a parking brake indicator light, landing gear indicator lights and a dimmer to control the brightness of the gear indicator lights.

![circuit2](https://user-images.githubusercontent.com/52784821/181107426-8869a6a9-5150-49f3-a255-911e778de55f.png)


To use the circuit with extras you will need to use a piece of software to interface with the data that MSFS2020 sends. I used "SimConnect To Arduino" by Seahawk240: https://github.com/Seahawk240/Microsoft-FS2020-To-Arduino

My current set of variables in Sim connect are:
![image](https://user-images.githubusercontent.com/52784821/181108493-4c9022a5-d4a5-4d6b-9a97-7afe2dd84ba6.png)

_____________________________________
