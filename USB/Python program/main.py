import serial
import keyboard
# com 11 is the right port
# com 10 is the left down port
ser1 = serial.Serial('COM13',  9600, timeout=1) # opens the serial port
ser = serial.Serial('COM14',  9600, timeout=1) # opens the other serial port, and creates an object of it
while 1:
    res = ser.readline() # reads the whole line, dont know how big the buffer is tho, so there has to be a \n in the message
    print(res.decode('Ascii')) #reading what we receive for debugging
    res1 = res.decode('ascii') #reading it and decoding it
    res3 = str(res1) #turning it to string
    ser1.write(res3.encode("Ascii")) # transmits the data to the port wanted (depends on the object)
    if keyboard.is_pressed('q'):
        print("done")
        break
ser.close()
ser1.close() # closing them properly

