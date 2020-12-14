# Routine for reading data from DSP on inverter board into Raspberry Pi
#
# Author: Thomas Gwasira, MLT Power
# Date: 09/12/2020
# Version: 1.0

import serial

# Setup primary UART (miniUART) and read from Powerstar card
with serial.Serial('/dev/serial0', 115200, timeout=20) as ser:
    print("Serial port opened.")
    ser.flushInput          # clear input serial buffer

    print("Reading serial connection to Powerstar card.")
    s = ser.read(10)        # read up to ten bytes (timeout)

    print(s)
    ser.close()



# ser = serial.Serial('/dev/serial0', 115200) # open serial port for primary UART (miniUART) with baud rate 115200
# ser.timeout = 20
# ser.flushInput() # clear input serial buffer

# Read data from Powerstar 10 card
ser.write(b'-twh\r-tw\r'); result = ser.read(6000); result.split("\r")
ser.close()

print("Hi. I'm Tom.")

# convert the result into an array of numbers, first split up the string, and then convert to ints
result_temp = result.split("\r")
headings = result_temp[1].split(",")
data = result_temp[5:-2]
data
data = [x.split(",") for x in  result_temp[5:-2]]
data
data_int = [[int(y) for y in x[:-1]] for x in data]
src_volts = [x[0]/12 for x in data_int]
src_i     = [x[2]/12 for x in data_int]




Freq_CRMS_Ampl=[0,100,0,20]
Freq_CRMS_Angl=[0,0,0,0]
gf=50  #grid signal freq
grid_amplitude =300
t =1/gf  #period of fundamental
nb_cycles =64
sample_per_cycle=64
total_samples=nb_cycles*sample_per_cycle 
src_volts =[0] *total_samples
for n in range(0,total_samples):
  # init grid_freq
  src_volts[n] = \
    grid_amplitude* math.sin(2* math.pi*n*gf/sample_per_cycle/gf+math.pi/2) \
    #+grid_amplitude/3* math.sin(2* math.pi*n*gf*3/sample_per_cycle/gf+3*math.pi/2) \
    #+ np.random.normal(1,10)

print ("Sample rate =" ,total_samples) 
src_sink_90 =[0] *total_samples
src_sink_0  =[0] *total_samples
src_v_prod  =[0] *total_samples
sum_v_prod  =[0] *total_samples
src_v_inphase=[0]*total_samples
sink_f = 50 # starting reference frequency
sink_amplitude=300
sum_vprod=0     # initial value of integral of prod of gf and src_sink_90
gain_product=200
for n in range(0,total_samples):
  src_sink_90[n] = sink_amplitude*math.cos((2 * math.pi * (sink_f + sum_vprod/gain_product) *n/sample_per_cycle/gf)) 
  src_sink_0[n]  = sink_amplitude*math.sin((2 * math.pi * (sink_f + sum_vprod/gain_product) *n/sample_per_cycle/gf))
  src_v_prod[n]  = src_volts[n]*src_sink_90[n]/sink_amplitude
  sum_vprod += (src_v_prod[n]+src_v_prod[n-int(n-sample_per_cycle/4)])/sink_amplitude
  sum_v_prod[n] = sum_vprod
#This following section determines the start of valid in phase vales
#from first positive zero crossing of sink in sink fundamental.
edge_pos=0
sample_good=0
for p in range(total_samples-32,total_samples-1):
  freq_final= sum_v_prod[p] /gain_product+sink_f
  print(freq_final)
for m in range(int(total_samples/2),total_samples):
  if  abs(src_sink_0[m]) <5 and edge_pos ==0 and src_sink_0[m]>src_sink_0[m-1]:
      edge_pos=1
      sample_good=m 
      print(sample_good)
      #freq_error=sum_v_prod[sample_good]/gain_product
      #print(freq_error)
  elif edge_pos ==1:
      src_v_inphase[m]=src_sink_0[m]
#This section determines in phase signal before first good sample
#for o in range(sample_good,0):
#      src_v_inphase[o]=
x=np.linspace(0,t*nb_cycles,total_samples)
#yf=rfft(,total_samples)
#for i in range(L):
#    print('The ', i ,'harmonic is', np.round(np.abs(yf[i]/(N/2))/np.sqrt(2),4,))
plot(x, src_volts,  label='volts')
#plt.plot(x,src_sink_90, label='volt sink_90')
#plt.plot(x,src_sink_0, label='volt sink_0' )
#plt.plot(x,src_v_prod, label='volt product')
plt.plot(x,sum_v_prod, label='volt product sum')
plt.plot(x,src_v_inphase, label='volt_in_phase')
plt.legend()
plt.grid()
gcf().set_size_inches(10,3)
show()
