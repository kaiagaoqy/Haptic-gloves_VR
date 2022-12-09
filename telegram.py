import serial, sys
import re
import time
port = ["COM10","COM11"]
baudrate = 115200
ser = serial.Serial(port=port[0],baudrate=baudrate,timeout=0.15)
ser2 = serial.Serial(port=port[1],baudrate=baudrate,timeout=0.15)
data = bytes()
if ser.isOpen():
    while True:
        time.sleep(0.15)
        n = ser.inWaiting()
        
        
        if n:
            data += ser.read(n)
            if len(data)>10:
                ser2.write(data)
                # print(data)
                data = bytes()

    

else:
    print("not open")

# while True:
#     ser.open()
#     try: 
#         cur = ser.read()
#         data += cur
#         # obj = re.search("A\d{3}.*",cur)
#         # data = obj.group()
#         # print(cur)
#         if(data.length == 100):
#         # ser.write(b"{}\n".format("A607B207C247D247E207"))
#         # print(data) 
#         time.sleep(1)
        
        
#     except :
#         pass
#     # print(cur)
    
#     # sys.stdout.write(data)
#     # sys.stdout.flush()
    
#     ser.close()