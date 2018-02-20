0# -*- coding: utf-8 -*-
"""
Created on Fri Sep 15 10:22:06 2017

@author: strat
"""

import json
import os, time, sys, subprocess
from random import randint, uniform
from time import sleep

walkAmount = 0.001

def trueOrFalse():
    user_input = input('y/n ')
    
    if user_input == 'y':
        return True
    elif user_input == 'n':
        return False
    else:
        return trueOrFalse()

#Runs a background app the displays an overlay over the screen with the unhcfreg logo
def image():
   # os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\myOverlay.exe"')
    print("Running background App")
    path = '"D:\\Student Projects\\Peter Gromkowski\\Python Things\\myOverlay.exe"'
    DETACHED_PROCESS = 0x00000008
    pid = subprocess.Popen([sys.executable, 'myOverlay.exe'])
    print("Launching")

#Expands the chaperone collision bounds by "multiply"
#Commit runs a program that reloads the chaperone info from the disk
def expandBounds(multiply):
    print('Expanding the boundries of the Chaperone')
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
    print('commit changes?')
    if trueOrFalse():
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')
        print("Running background App")
       
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
        
        print('Enable Camera?')
        if(trueOrFalse()):
            data['camera']['enableCamera'] = true
        else:
            data['camera']['enableCamera'] = true
        
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
    print('Rebooting SteamVR')

def takePic():
    os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\NinjaCamera.exe"')

def moveChaperone():
    print('Translating')
    for i in range(0,100):
        
        with open('C:\Program Files (x86)\Steam\config\oculus\driver_oculus.vrchap', 'r+') as data_file:    
            data = json.load(data_file)
            dist = uniform(-9, 9)
            axis = randint(0,2)
            
            for calibrate in data['universes']:
                calibrate['standing']['translation'][axis] += dist
                calibrate['standing']['yaw'] += dist

            data_file.seek(0)
            json.dump(data, data_file)
            data_file.truncate()
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')
        
def walk(axis, amount):
    print('Walking: ' + str(axis) + '  amount: ' + str(amount))
    
    with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
                data = json.load(data_file)
                    
                for calibrate in data['universes']:
                    calibrate['standing']['translation'][axis] += amount
                        

                data_file.seek(0)
                json.dump(data, data_file)
                data_file.truncate()
    os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')
            
def firstpoint():
    print('Moving to first point')
    for i in range(0,1200):
        with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
                data = json.load(data_file)
                
                for calibrate in data['universes']:
                    calibrate['seated']['translation'][2] += walkAmount
                    calibrate['standing']['translation'][2] += walkAmount
                    

                data_file.seek(0)
                json.dump(data, data_file)
                data_file.truncate()
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')
        
    print('Done')

def secondpoint():
    print('Moving to second point')
    for i in range(0,1640):
        with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
                data = json.load(data_file)
                
                for calibrate in data['universes']:
                    calibrate['seated']['translation'][0] -= 1061/1640 * walkAmount
                    calibrate['seated']['translation'][2] -= 1247/1640 * walkAmount
                    calibrate['standing']['translation'][0] -= 1061/1640 * walkAmount
                    calibrate['standing']['translation'][2] -= 1247/1640 * walkAmount
                    

                data_file.seek(0)
                json.dump(data, data_file)
                data_file.truncate()
        os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\moveChaperone.exe"')
    print('Done')



while(True):

    print(  'c  --  Camera Options \n'
            '1  --  Move to Point 1\n'
            '2  --  Move to Point 2\n'
            'q  --  Quit'
              )
    
    user_input = input()
    
    if user_input == 'c':
        camera()
    
    if user_input == 'd':
        image()
    
    if user_input == 'e':
        print('Amount to multiply? ')
        mult = input()
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

    if user_input == 't':
        takePic()

    if user_input == 'u':
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
    if user_input == '1':
        firstpoint()
    if user_input == '2':
        secondpoint();
        
    if user_input == 'q':
        print('Exiting')
        break

        
    
