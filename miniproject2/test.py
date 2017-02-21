import time
import miniproject2 as m

mt = m.jtest()

#while(True):
angleBytes = mt.get_angle() #gets angle measurement in bytes
print angleBytes
first = int(angleBytes[0])+int(angleBytes[1])*256 #formats to integer
second = int(angleBytes[2])+int(angleBytes[3])*256 #formats to integer
print "first: ",'Bin: {0:016b} Dec: {0:0d}'.format(first) 
print "second: ",'Bin: {0:016b} Dec: {0:0d}'.format(second) 
#print in angle code in binary and hex
#the decimal section is a fractional number between 0 and 1 
#    time.sleep(.01)
    #pause for .01 amount of seconds
