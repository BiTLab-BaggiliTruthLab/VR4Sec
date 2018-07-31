# -*- coding: utf-8 -*-
"""
Created on Fri Sep 15 10:22:06 2017

@author: strat
"""

import json
import os, time, socket, subprocess, sys
from random import randint, uniform

s = socket.socket()
host = '10.2.33.15'
port = 9999
s.connect((host, port))
walkAmount = 0.1

def trueOrFalse():
    data = s.recv(1024)
    user_input = data.decode('utf-8')
    
    if user_input == 'y':
        return True
    elif user_input == 'n':
        return False
    else:
        return trueOrFalse()

#Runs a background app the displays an overlay over the screen with the unhcfreg logo
def image(sleepTime):
    s.send(str.encode("Running background App\n"))
    path = '"D:\\Student Projects\\Peter Gromkowski\\Python Things\\myOverlay.exe"  %s' % str(sleepTime)
    os.system(path)
    

#Expands the chaperone collision bounds by "multiply"
#Commit runs a program that reloads the chaperone info from the disk
def expandBounds(multiply):
    s.send(str.encode('Expanding the boundries of the Chaperone\n' 
                      'Commit Changes? y/n  '))
    with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
        data = json.load(data_file)
    
        for calibrate in data['universes']:
            for wall in calibrate['collision_bounds']:
                for i in range(0,3):
                    wall[i][0] = wall[i][0] * multiply
                    wall[i][2] = wall[i][2] * multiply
            
        data_file.seek(0)
        json.dump(data, data_file)
        data_file.truncate()
    
    if trueOrFalse():
        s.send(str.encode("Running background App  "))
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\myOpenVr.exe"')
        print('Ran myOpenVr')
    else:
        s.send(str.encode('Not committed '))
       
def setOpacity(a, b, r, perimOn):
    with open('C:\Program Files (x86)\Steam\config\steamvr.vrsettings', 'r+') as data_file:
        data = json.load(data_file)
        
        data['collisionBounds']['CollisionBoundsColorGammaR'] = r
        data['collisionBounds']['CollisionBoundsColorGammaA'] = a
        data['collisionBounds']['CollisionBoundsColorGammaB'] = b
        data['collisionBounds']['CollisionBoundsGroundPerimeterOn'] = perimOn
        
        data_file.seek(0)
        json.dump(data, data_file)
        data_file.truncate()


def camera():
    with open('C:\Program Files (x86)\Steam\config\steamvr.vrsettings', 'r+') as data_file:
        data = json.load(data_file)
        
        s.send(str.encode('Enable Camera? y/n? '))
        data['camera'] = {'enableCamera' : True}
      #  data['camera'].append('enableCamera')
      #  data['camera']['enableCamera'] = trueOrFalse()
        s.send(str.encode('Done  '))
        
        data_file.seek(0)
        json.dump(data, data_file)
        data_file.truncate()

def reboot():
    os.system("taskkill /f /im vrmonitor.exe")
    os.system("taskkill /f /im vrcompositor.exe")
    os.system("taskkill /f /im vrdashboard.exe")
    os.system("taskkill /f /im vrserver.exe")
    print('Killed SteamVR')
    time.sleep(1)
    os.system('"C:\\Program Files (x86)\\Steam\\steamapps\\common\\SteamVR\\bin\\win64\\vrstartup.exe"')
    s.send(str.encode('Rebooting SteamVR   '))

def takePic():
    s.send(str.encode('Taking Pictures   '))
    os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\CameraClient.exe"')
    data = s.recv(1024)
    os.kill('CameraClient.exe')

def moveChaperone():
    s.send(str.encode('Translating  \n'))
    for i in range(0,100):
        
        with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
            data = json.load(data_file)
            dist = uniform(-0.1, 0.1)
            axis = randint(0,2)
            
            for calibrate in data['universes']:
                calibrate['standing']['translation'][axis] += dist
                calibrate['standing']['yaw'] += dist

            data_file.seek(0)
            json.dump(data, data_file)
            data_file.truncate()
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')

def walk(axis, amount):
    s.send(str.encode('Walking\n  '))
    with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
            data = json.load(data_file)
            
            for calibrate in data['universes']:
                calibrate['standing']['translation'][axis] += amount
                

            data_file.seek(0)
            json.dump(data, data_file)
            data_file.truncate()
    os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')


def helpMenu():
    s.send(str.encode(
            'c  --  Camera Options \n'
            'i  --  Display image on screen\n'
            'e  --  Expand the bounds of the Chaperone\n'
            'r  --  Reboot Steam VR\n'
            'o  --  Set Opacity Values \n'
            't  --  Take Pictures\n'
            'm  --  Randomly Wobble the screen \n'
            'q  --  Quit\n  '
              ))


while(True):
    
    data = s.recv(1024)
    user_input = data.decode('utf-8')

    if user_input == '?':
        helpMenu()
    
    if user_input == 'c':
        camera()
    
    if user_input[:1] == 'i':
        sleepTime = int(user_input[1:])
        image(sleepTime)
    
    if user_input == 'e':
        s.send(str.encode('Amount to multiply? '))
        data = s.recv(1024)
        mult = data.decode('utf-8')
        expandBounds((float(mult)))

    if user_input == 'o':
         print('Set the Gamma Values...')
         A = input('A?')
         B = input('B?')
         R = input('R?')
         print('Collision Bounds Ground Perimeter On?')
         setOpacity(A,B,R,trueOrFalse())
    
    if user_input == 'r':
        reboot()

    if user_input[:1] == 't':
        takePic()

    if user_input == 'm':
        moveChaperone();

    if user_input == 'w':
        walk(0 , walkAmount)
    if user_input == 'a':
        walk(2 , -walkAmount)
    if user_input == 's':
        walk(0 , -walkAmount)
    if user_input == 'd':
        walk(2 , walkAmount)
    if user_input == 'z':
        walk(1 , walkAmount)
    if user_input == 'x':
        walk(1 , -walkAmount)
        
        
    if user_input == 'q':
        s.send(str.encode('Exiting'))
        break

        
    
