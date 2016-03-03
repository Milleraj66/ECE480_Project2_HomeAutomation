"""
    FileName: WeatherDriver.py
    Authors: Arthur J. Miller, Dillon Smolinski, Keith Brase
    Date: 03-02-2016
    Purpsose:   Winter 2016, Cal Poly Pomona, ECE 480, Project 3
                This is the python driver file for an arduino controller.
                1. This file waits for the controller to send the signal "Weather" over a seriel port on the computer.
                2. When recieved The program grabs a json file from weatherunderground.
                3. Usefull data is parsed from the json file
                4. This data is passed as a list into a TKINTER gui class object
                5. This data is displayed for the user on a TKINTER gui window



"""
import serial.tools.list_ports
import serial, time
import urllib2
import json
from Tkinter import Tk, Label
from PIL import Image, ImageTk

# Dynamically find which port arduino is plugged into
def findArduinoUnoPort():
    portList = list(serial.tools.list_ports.comports())
    for port in portList:
        if "VID:PID=2341:0043" in port[0]\
            or "VID:PID=2341:0043" in port[1]\
            or "VID:PID=2341:0043" in port[2]:
            #please note: it is not sure [0]
            #returned port[] is no particular order
            #so, may be [1], [2]
            return port[0]
# Define seriel port as port which arduino is plugged into
unoPort = findArduinoUnoPort()
ser = serial.Serial(unoPort, 9600)

# Handles Tkinter GUI
class WeatherGUI:
    def __init__(self, master,infolist):
        self.master = master
        master.title("Current Weather Conditions")
        self.label = Label(master, text="The time is: %s"%infolist[0],font=("Helvetica", 30))
        self.label.pack()
        self.label = Label(master, text="The weather for %s is "%infolist[1],font=("Helvetica", 30))
        self.label.pack()
        self.label = Label(master, text="%s Degrees & %s"%(infolist[2],infolist[3]),font=("Helvetica", 30))
        self.label.pack()
        if(infolist[3]=="Partly Cloudy"):
           image=Image.open("partlycloudy.jpg")
           master.photo=ImageTk.PhotoImage(image)
           self.label = Label(master,image=master.photo)
           self.label.pack()
        elif(infolist[3]=="Cloudy"):
           image=Image.open("cloudy.jpg")
           master.photo=ImageTk.PhotoImage(image)
           self.label = Label(master,image=master.photo)
           self.label.pack()
        elif(infolist[3]=="Sunny"):
           image=Image.open("Sunny.jpg")
           master.photo=ImageTk.PhotoImage(image)
           self.label = Label(master,image=master.photo)
           self.label.pack()

# Main Logic loop. Waits for the arduino to send weather signal -> then gets,parses,and displays data on gui
while 1:
   serial_line = ser.readline()
   if (serial_line == 'Weather\r\n'):
       state = 'CA'
       city = 'Pomona'
       f = urllib2.urlopen('http://api.wunderground.com/api/fc5428169d227648/geolookup/conditions/q/%s/%s.json' %(state, city))
       json_string = f.read()
       parsed_json = json.loads(json_string)
       location = parsed_json['location']['city']
       temp_f = parsed_json['current_observation']['temp_f']
       weather = parsed_json['current_observation']['weather']
       time = parsed_json['current_observation']['local_time_rfc822']
       print ("Current temperature on %s in %s is: %s" % (time, location, temp_f))
       print("Weather in %s is %s" % (location, weather))
       infolist = [time,location, temp_f, weather]
       for i in range (4):
           print infolist[i]
       f.close()
       root=Tk()
       Weather=WeatherGUI(root,infolist)
       root.mainloop()
       break

ser.close() # Only executes once the loop exits
