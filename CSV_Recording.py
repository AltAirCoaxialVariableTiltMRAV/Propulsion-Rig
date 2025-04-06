# Author: Guy Hasewinkel
# System: Propulsio rig
# Program: Propulsion rig sensor script
 
# Imported libraries
import serial
import time
import threading
import queue
 
# CSV filename to record to
fileName = "static_15_the_holy_guac__13X55MR_16.8V_GT2820_985Kv.csv"
 
# Open serial monitor for motor arduino and sensor arduino.  
motor_serial = serial.Serial('COM8', 2000000) # Match this to the motor Arduino port, timeout is defaulted to 0
sensors_serial = serial.Serial('COM7', 2000000) # Match this to the sensor Arduino port, timeout is defaulted to 0
 
# Need to queue the data due to the two threads
queue_two_threads = queue.Queue()  
 
# Function to configure the ESC. Adapt the duty cycle sequence if necessary
def configureESC():
    print("ESC initialisation...")
    motor_serial.write(f"{100}\n".encode())
    time.sleep(1)  
    print("...")
    motor_serial.write(f"{100}\n".encode())
    motor_serial.write(f"{100}\n".encode())
    motor_serial.write(f"{100}\n".encode())
    time.sleep(4)  
    print("...")
    motor_serial.write(f"{-1}\n".encode())
    motor_serial.write(f"{-1}\n".encode())
    motor_serial.write(f"{-1}\n".encode())
    time.sleep(1)  
    print("ESC initialised.")
    time.sleep(6)
 
# Function to read the serial monitor data from the sensor arduino and write it to a csv file
def SensorsToCSV():
    with open(fileName, 'a') as file: # opening the CSV file in append mode
        while True:
            if not queue_two_threads.empty():
                last_motor = queue_two_threads.get()
            data = sensors_serial.readline().decode('utf-8')[0:][:-2]
            csv_line = f"{data},{last_motor}"
            print(csv_line)
            file.write(csv_line + "\n")
 
# Function to send the PWM signal to the motor arduino, and write the PWM value to the csv file
def PWMtoMotor():
    # Motor PWM
    motor_PWM = 20
    # Queing the motor PWM
    queue_two_threads.put(motor_PWM)
    motor_serial.write(f"{motor_PWM}\n".encode())
    time.sleep(1)
 
    # Example static test sequence below
    for i in range(10, 100):
        motor_PWM = i
        queue_two_threads.put(motor_PWM)
        motor_serial.write(f"{motor_PWM}\n".encode())
        time.sleep(0.5)
    for i in range(100, 10, -1):
        motor_PWM = i
        queue_two_threads.put(motor_PWM)
        motor_serial.write(f"{motor_PWM}\n".encode())
        time.sleep(0.5)
 
    motor_PWM = -1
    queue_two_threads.put(motor_PWM)
    motor_serial.write(f"{motor_PWM}\n".encode())
    motor_serial.write(f"{-10}\n".encode())
    motor_serial.close()
    sensors_serial.close()
 
# Configure the ESC
configureESC()
 
# Start the function thread for the motor arduino serial monitor
motor_thread = threading.Thread(target=PWMtoMotor)
motor_thread.start()
 
# Start the function thread for the sensor arduino serial monitor
data_thread = threading.Thread(target=SensorsToCSV)
data_thread.start()
 
# Abort sequence
try:
    while True:
        time.sleep(1)
# Abort the testing sequence with the keyboard interrupt (ctrl+c)
except KeyboardInterrupt:
    motor_serial.write(f"{-10}\n".encode())
    motor_serial.close()
    sensors_serial.close()