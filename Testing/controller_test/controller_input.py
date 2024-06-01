from inputs import get_gamepad
import serial
ser = serial.Serial("COM5", 115200)
   
# Send character 'S' to start the program
ser.write(bytearray('S','ascii'))

# Read line   
while 1:
    bs = ser.readline()
    print(bs)
while 1:
    events = get_gamepad()
    for event in events:
        print(event.ev_type, event.code, event.state)