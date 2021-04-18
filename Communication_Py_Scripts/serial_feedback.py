#! /usr/bin/env python3
import serial
import rospy
from rospy_message_converter import message_converter
from std_msgs.msg import String

# -------------------Port Initialization--------------------
ser = serial.Serial(port="/dev/ttyACM1", baudrate=115200,timeout = None)  # open serial port


# frame = "111R222T333S"    # This frame is only for testing 
messagexd = 'x'

def talker():
    # Creating three different topics 
    pub= rospy.Publisher('Rotation', String, queue_size=10)
    pub2= rospy.Publisher('Throttle', String, queue_size=10)
    pub3= rospy.Publisher('Speed', String, queue_size=10)

    # Publisher Node init 
    rospy.init_node('Feedback', anonymous= True)
    rate= rospy.Rate(8) 

    while not rospy.is_shutdown():
        ser.write(messagexd.encode('ascii', 'ignore'))
        frame = str(ser.read(5))    #the size of our frame is 61 bytes according to this function (sys.getsizeof())
        print(frame)
        #-----------------Extracting values--------------
        R= frame[(frame.index('R')-3):(frame.index('R'))]  
        #T= frame[(frame.index('T')-3):(frame.index('T'))]
        #S= frame[(frame.index('S')-3):(frame.index('S'))]

        
        #------------Putting values inside dictionaries------------
        Rotation_dict = { 'data': R}
        #Throttle_dict = { 'data': T}
        #Speed_dict = { 'data': S}

        #------------Converting dictionaries into rosmsgs-----------------
        message = message_converter.convert_dictionary_to_ros_message('std_msgs/String', Rotation_dict)
        #message2 = message_converter.convert_dictionary_to_ros_message('std_msgs/String', Throttle_dict)
        #message3 = message_converter.convert_dictionary_to_ros_message('std_msgs/String', Speed_dict)

        #----------Publishing the msgs into topics-------------------
        pub.publish(message)
        #pub2.publish(message2)
        #pub3.publish(message3)
    
        rate.sleep()
        
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass

