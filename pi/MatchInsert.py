
#!/usr/bin/python
import serial, time, re, subprocess, sys, socket, bluetooth

import mysql.connector
def nodeParser(arduino_sensor, node) :
    #print arduino_sensor
    nodeDict = {}

    if node == 1:
      values = ['node', 'temp1', 'temp2', 'light','motion']
    elif node == 3:
      values = ['node', 'temp', 'temp2', 'light', 'motion', 'plant1','plant2', 'plant3']
    elif node == 2:
      values = ['node', 'temp', 'humidity', 'light']

    for searches in values:
      searchString = "(%s=)[\d\.]+" % searches
      p = re.compile(searchString)
      result =  p.search(arduino_sensor).group()
      replacer = "%s=" % searches
      result = result.replace(replacer,"")
      nodeDict[searches]= result
    for key, value in nodeDict.iteritems() :
      print key, value
    arduino_sensor = arduino_sensor.replace(";"," ")
    return nodeDict

    #sensor2 = sensor2.replace(";"," ")
    #print sensor2

def tcpSend(line):

    TCP_IP = 'splunk_server'
    TCP_PORT = 8101

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    s.send(line)
    s.close()
def locater(name):
    if 'mike' in name:
        result = bluetooth.lookup_name('mymac', timeout=5)
        if (result != None):
            output = "locator=mike loctation=1"
            locationstatus=1
        else:
            output = "locator=mike loctation=0"
            locationstatus=0
    elif 'heather' in name:
        result = bluetooth.lookup_name('heathers_mac', timeout=5)
        if (result != None):
            output = "locator=heather loctation=1"
            locationstatus=1
        else:
            output = "locator=heather loctation=0"
            locationstatus=0
    add_locater_request = ("INSERT INTO home "
               "(node, notes_1) "
               "VALUES (%s, %s)")
    data_locater_request = (name, locationstatus)
    cursor.execute(add_locater_request,data_locater_request)
    cnx.commit()
    tcpSend(output)
    

def FetchOneAssoc(cursor) :
    data = cursor.fetchone()
    if data == None :
        return None
    desc = cursor.description

    dict = {}

    for (name, value) in zip(desc, data) :
        dict[name[0]] = value

    return dict




ser = serial.Serial('/dev/ttyUSB0',  9600, timeout = 0.1)

#if you only want to send data to arduino (i.e. a signal to move a servo)
def send( theinput ):
  ser.write( theinput )
  while True:
    try:
      time.sleep(0.01)
      break
    except:
      pass
  time.sleep(0.1)

#if you would like to tell the arduino that you would like to receive data from the arduino
def send_and_receive( theinput ):
  ser.write( theinput )
  while True:
    try:
      time.sleep(0.01)
      state = ser.readline()
      #print state
      if state > 0:
        return state
    except:
      pass
  time.sleep(0.1)



#cnx = mysql.connector.connect(host='sql_server', user='t0rque', database='files', password='password')
#cursor = cnx.cursor()

while 1 :
   #cursor.execute( "delete from home where node=3|| node=1 || node=2 || node='mike' || node='heather';")
   #cnx.commit()

   arduino_sensor1 = send_and_receive('1')
   time.sleep(1)
   if (arduino_sensor1 != "") and ("node=1" in arduino_sensor1) :
      sensor1a = arduino_sensor1.replace(";"," ")
      tcpSend(sensor1a)
      #nodeValueDict=nodeParser(arduino_sensor1, 1)
      #add_sensordata_request = ("INSERT INTO home "
        #       "(node, temp_1, temp_2, light_1, motion_1) "
       #        "VALUES (%s, %s, %s, %s, %s)")
      #data_sensordata_request = (nodeValueDict['node'], nodeValueDict['temp1'], nodeValueDict['temp2'], nodeValueDict['light'], nodeValueDict['motion'])
      #cursor.execute( "delete from home where node=1;")
      #cnx.commit()
      #cursor.execute(add_sensordata_request,data_sensordata_request)
      #cnx.commit()

      break

      break
time.sleep(1)
while 1 :
   arduino_sensor2 = send_and_receive('2')
   time.sleep(1)
   if (arduino_sensor2 != "" ) and ("node=2" in arduino_sensor2):
       sensor2a = arduino_sensor2.replace(";"," ")
       tcpSend(sensor2a)
       #nodeValueDict=nodeParser(arduino_sensor2, 2)
       #add_sensordata_request = ("INSERT INTO home "
         #      "(node, temp_1, light_1, notes_1) "
        #       "VALUES (%s, %s, %s, %s)")
       #data_sensordata_request = (nodeValueDict['node'], nodeValueDict['temp'], nodeValueDict['light'], nodeValueDict['humidity'])
       #cursor.execute( "delete from home where node=2;")
       #cnx.commit()
       #cursor.execute(add_sensordata_request,data_sensordata_request)
       #cnx.commit()
       
       break
time.sleep(1)
while 1 :
   arduino_sensor3 = send_and_receive('3')
   time.sleep(1)
   if (arduino_sensor3 != "") and ("node=3" in arduino_sensor3) :
       sensor3a = arduino_sensor3.replace(";"," ")
       tcpSend(sensor3a)      
       nodeValueDict=nodeParser(arduino_sensor3, 3)
       #add_sensordata_request = ("INSERT INTO home "
      #        "(node, temp_1, temp_2, motion_1, misc_1, misc_2, misc_3, light_1) "
        #       "VALUES (%s, %s, %s, %s, %s, %s, %s, %s)")
       #data_sensordata_request = (nodeValueDict['node'], nodeValueDict['temp'], nodeValueDict['temp2'], nodeValueDict['motion'], nodeValueDict['plant1'], nodeValueDict['plant2'], nodeValueDict['plant3'], nodeValueDict['light'])
       #cursor.execute( "delete from home where node=3;")
       #cnx.commit()
       #cursor.execute(add_sensordata_request,data_sensordata_request)
       #cnx.commit()
       
      
       break
#cursor.execute( "delete from home where node='mike';")
#cnx.commit()
#locater("mike")
#cursor.execute( "delete from home where node='heather';")
#cnx.commit()
#locater("heather")
#cursor.close()
#cnx.close()

