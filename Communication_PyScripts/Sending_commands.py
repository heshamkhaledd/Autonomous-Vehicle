#!/usr/bin/env python
import rospy
import serial

speed = -10.21
steering = 24
brakes = '1'

steeringChar = 'R'
speedChar = 'T'
brakesChar = 'B'

serialPort=0

# -----------------Decoding Layer Functions ---------------#
def serialCommunicationInit():
    global serialPort
    serialPort= serial.Serial(port = "/dev/ttyACM1", baudrate=115200)

def resize(var):
    sign = '0'
    temp = 0

    if var >= 0:
        sign = '0'
    else: sign = '-'

    temp = round(abs(var),2)
    tempStr = str(temp)
    zerosNumber = 5 - len(tempStr)
    
    for i in range (zerosNumber):
        tempStr = '0' + tempStr

    return sign + tempStr


if __name__ == '__main__':

    rospy.init_node('serialHandling_node', anonymous=True)
    serialCommunicationInit() 
    rate= rospy.Rate(8) #10Hz
    
    while(1):
        steeringReSized = resize(steering)
        speedReSized = resize(speed)
        message = "F" + steeringReSized + steeringChar + speedReSized + speedChar + brakes + brakesChar
        print(message)
        message = message.encode('ascii', 'ignore')
        serialPort.write(message)
        if(steering ==25):
            steering= -24
        else:
            steering=25
        rate.sleep()
    rospy.spin()