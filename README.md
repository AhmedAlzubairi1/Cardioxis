# Cardioxis
This was the code from the Cardioxis, a machine I alone created for Dr. Qi Wang's lab in Columbia University biomedical department. 

To use the code, you need to change the value of the #define _SS_MAX_RX_BUFF from a 64 to a 256. #define _SS_MAX_RX_BUFF 
is located in the sofwareserial.h file that is preinstalled when the Arduino IDE is installed. After this value is changed, upload the code to an arduino.
To create the machine, connect the DB-9 cable from a Pulse Oximeter to the ardunio via a RS232 Shifter. 
For the output wires, use two 18awg 2-wire. One of the wires sends out information about the oxygen percentage, and 
one sends out information about the heart rate. Attach the heart rate wire to pin 10 and the other wire into pin 11.
After turning on the machine, the machine will analyse the data from the Pulse Oximeter into to the wires as a 
voltage output. For the user's use, the voltages should be interpeted as such: multiply the oxgen wire's voltage
output by 20, and multiply the heart rate voltage output by 100. The heart rate values are accurate within 3 beats per minute.
This shouldn't be a problem for animals whose heart beats a lot, such as mice.
