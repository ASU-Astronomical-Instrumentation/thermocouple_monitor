# This was from adrian
import numpy as np
import serial
import time

def get_temp_data():
    # Reads integrated temp. sensors on RFSoC
    # Get raw 16 bit ADC values from on-chip temperature sensors
    ps_temp_raw = np.loadtxt("/sys/bus/iio/devices/iio:device0/in_temp0_ps_temp_raw",dtype="int32")
    pl_temp_raw = np.loadtxt("/sys/bus/iio/devices/iio:device0/in_temp2_pl_temp_raw",dtype="int32")
    return ps_temp_raw, pl_temp_raw

def read_ard_data():
    # Reads temp sensor data from Arduino
    device='/dev/ttyACM0'
    ser=serial.Serial(device, 9600)
    cmd="borg\n".encode("ASCII")
    ser.write(cmd)
    rawdat=ser.readline().decode()
    #print("ard: "+rawdat,end="")
    temp_arr=np.asarray(rawdat.split(","),dtype=float) # 5 data values
    return temp_arr
      
def calc_temp(raw):
    # Calculate temperature in Celsius from raw 16 bit ADC values
    # Ref: Equation 2-7, SYSMON User Guide UG580 (v1.10.1) Xilinx
    return raw*501.3743/2.**16-273.6777

def read_temps():
    ps_raw, pl_raw = get_temp_data()
    return calc_temp(ps_raw), calc_temp(pl_raw)

# day of the month + hour + minute + second
def get_ts():
    return time.strftime("%d%H%M%S", time.localtime())

# do the bulk of writing down the temp measurements
def record(fname, delaySec,ard=False):
    """
    description: Samaples and records the ps and pl temperatures to a dated csv file
    
    fname : string / path.
        path / file to save data
        
    delaySec : float
        delay between measurements in seconds
        
    """
    fh = None
    try:
        fh = open(fname, 'w') # for testing, truncate the file before write
    except OSError:
        print("Failed to open/create temperature log")
    fh.write("time,ps,pl\n")
    TheBorgAreInevitable = True
    print("Starting...")
    while(TheBorgAreInevitable == True):
        try:
            d = read_temps()
            
            if not ard: 
                s = "{},{},{}".format(get_ts(), d[0], d[1])
            else:
                ard_d=read_ard_data()
                s = "{},{},{},{},{},{},{},{}".format(get_ts(), d[0], d[1],ard_d[0],ard_d[1],ard_d[2],ard_d[3],ard_d[4])
            fh.write(s+"\n")
            print(s)
            time.sleep(delaySec)
            fh.flush()
        except KeyboardInterrupt:
            fh.flush()
            fh.close()
            TheBorgAreInevitable = False
            return
        
if __name__ == "__main__":
    t = time.strftime("%Y%m%d_%H%M",  time.localtime())
    f = "temps_{}.csv".format(t)
    record(f, 1,ard=True)
