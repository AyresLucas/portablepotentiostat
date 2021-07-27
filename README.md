# portablepotentiostat
This is the repository for the portable potentiostat published by our research group (https://scienceweb.clemson.edu/uacl/). Here you will find information regarding the firmware, hardware, graphical user interface as well as assemble instructions. More details regarding the instrumental characterization and analytical performance can be found in this paper.

Firmware:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
We have two versions of firmware available: firmware1_0 and firmwareUSB. Both versions can be uploded to ESP 32 microcontroller by using the Arduino IDE or by any other method of your preference (e.g. LUA, micropython, espress IDF and etc). If you are not familiar uploading code to ESP 32 by Arduino IDE, we recommend the following article: https://randomnerdtutorials.com/getting-started-with-esp32/.

**firmware1_0:** This is the main firmware of our portable potantiostat and it is responsible to comunicate to our broker via wifi. Basically, the code is responsible for processing a string of characters incoming from the broker. Each position of the string carrys it's varaiable for the desired analytical technique. For example, the first position tell us either the choosen techique is amperometry [0] or ciclic voltametry [1]. The folowwing positions are reserved for input parameters set up by the user. A Detailed documentation of the procotol of communication will be released soon. <br/> <br/>

**firmwareUSB:** As the name suggests, this firmware can be used for data acquistion and management of the instrument by any other software through USB comunication.  


Hardware:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**Graphical User Interface (GUI):** This application was developed in node js and it is goal is to manage the portable instrument via Wifi. The remote management includes the paramaters configuration for the analytical technique as well as data acquisition of the instrumentation developed. The sofware has two main parts for the folowwing techniques: Amperometry and Ciclic Voltametry. Regarding ampetometry, standard parameters such as pottential applied, time as well as sample interval are avaibale are easily set up on it is main screen. With regard ciclic voltametry, parameters such as enitial potential, high potential, low potential, scan rate as well as sweep segments are availabe to be set up according to the user needs. Addtionally, a "Help Section" is avaible under the main menu and has some details about the instrumentation developed as well as practical guide for begginers users.


Assemble instructions:
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
