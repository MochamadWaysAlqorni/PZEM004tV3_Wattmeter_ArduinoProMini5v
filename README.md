# PZEM004tV3_Wattmeter_ArduinoProMini328p5v

Ordinary pzem004t v3.0 interface with promini-328-16MHz with modified energy counter reset.  
This will usefull if you are uploading or having another device connected to tx and rx pin of promini (I always upload sketch with USBtoTTL which using tx and rx pin, so using hardware serial is not an option) or you have problem accessing pzem.resetEnergy function like me :) ..  

< BE CAREFUL! >  
This sketch writing to eeprom everytime you press reset energy button, eeprom of atmega328 have limit of about 100k write/erase cycle (based on my googling result). So don't be wasteful of your eeprom cycle, use it wisely.  
