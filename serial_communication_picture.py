import serial
import time
import threading
import subprocess
from datetime import datetime

SERIAL_PORT = '/dev/ttyACM0'  
BAUD_RATE = 9600

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

THRESHOLD_RPM = 200
photo_taken = False

def take_photo():
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"/home/pi/JSP/SUA_PICTURE_{timestamp}.jpg"
    threading.Thread(target=subprocess.run, args=(["libcamera-still", "-o", filename, "-t", "500"],)).start()
    print(f"photo taken: {filename}")

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(f"Recieve data: {line}")  

            if line.startswith("RPM: "):
                try:
                    rpm = int(line.split(": ")[1])
                    print(f"Current RPM: {rpm}")

                    if rpm >= THRESHOLD_RPM and not photo_taken:
                        take_photo()
                        photo_taken = True

                    if rpm < THRESHOLD_RPM:
                        photo_taken = False 

                except ValueError:
                    print(f"Invalid data: {line}")
            else:
                print(f"Invalid data format: {line}")
        time.sleep(0.1) 

except KeyboardInterrupt:
    print("program terminated")

finally:
    ser.close()
