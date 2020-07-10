#!/usr/bin/env python3

import serial
import time
import re

PORT = '/dev/ttyUSB0'
BAUD = 9600
PS1='$ '
SLEEP= 0.1
RE_TAKE_FLOAT='(?<={field}).*\d+\.\d*'
RE_TAKE_HEX='(?<={field})*0x[a-fA-F0-9]+'

C=3e8

log=open('../results/client_output/serial.log','a')
valid_results=open('../results/client_output/results.csv','w')
valid_results.write('try,time [ns]\n')

def read_response(ser,wait_for,must_also_contain=''):
	red=''
	piece=ser.read(100).decode()
	red=red+piece
	while (must_also_contain not in red):
		piece=ser.read(100).decode()
		red=red+piece
	l=len(red)
	while (wait_for not in red[l:]):
		piece=ser.read(100).decode()
		red=red+piece
		# time.sleep(SLEEP)
	# log.write(red)
	return red



with serial.Serial(PORT, BAUD,timeout=0) as ser:
	thre=210

	#ser.write(b'gain 12\n')
	# ser.write(b'gain 1\n')
	#ser.write('thre {}\n'.format(thre).encode('utf-8'))
	# ser.write(b'thre 255\n')
	#ser.write(b'ovd 0\n')
	ser.reset_input_buffer()

	for j in range(1000):
		print('Measurement:',j)
		received=False
		i=0
		while not received:
			i+=1
			# if not i%10:
			# 	thre-=1
			# 	print('Decreasing thre:',thre)
			# 	ser.write('thre {}\n'.format(thre).encode('utf-8'))

			print('Try{}...'.format(i))

			ser.reset_input_buffer()
			ser.write(b'measure1\n')
			red1=read_response(ser,PS1,must_also_contain='measure')

			m=re.search(RE_TAKE_HEX.format(field='status='),red1)
			if m:
				status=int(m.group(),16)
			else:
				status=None
				raise Exception('status not found, check log')


			if status==0x19:
				received=True
				m=re.search(RE_TAKE_FLOAT.format(field='tof_ns='),red1)
				if m:
					tof_ns=float(m.group())#-34.8759
				else:
					tof_ns=None
					raise Exception('time not found, check log')

			# time.sleep(1)
		valid_results.write('{},{}\n'.format(j,tof_ns))
		print('Distance:',tof_ns*C*1e-9,'m')

# with serial.Serial(PORT, BAUD,timeout=0) as ser:
# 	# send_string = input('Send: ').encode('utf-8')

# 	send_string = "help\n".encode('utf-8')
# 	ser.write(send_string)
# 	red=read_response(ser,PS1)
# 	print(red)
