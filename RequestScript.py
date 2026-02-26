#! /usr/bin/python3

import json
import socket
import binascii
import time
import sys
import numpy as np 
import os
import argparse

# Argument parser setup
parser = argparse.ArgumentParser(description="")
parser.add_argument("--asic", type=int, default=0, help="ASIC address (default: 0)")
parser.add_argument("--fpga", type=int, default=0, help="FPGA address (default: 0)")
args = parser.parse_args()
# Get ASIC and FPGA addresses from arguments
asic_address = args.asic
fpga_address = args.fpga

register_names = [
    "Top",
    "CM_2",
    "CM_3",
    "Channel_36",
    "Channel_37",
    "Channel_38",
    "Channel_39",
    "Channel_40",
    "Channel_41",
    "Channel_42",
    "Channel_43",
    "Channel_44",
    "Channel_45",
    "Channel_46",
    "Channel_47",
    "Channel_48",
    "Channel_49",
    "Channel_50",
    "Channel_51",
    "Channel_52",
    "Channel_53",
    "Channel_54",
    "Channel_55",
    "Channel_56",
    "Channel_57",
    "Channel_58",
    "Channel_59",
    "Channel_60",
    "Channel_61",
    "Channel_62",
    "Channel_63",
    "Channel_64",
    "Channel_65",
    "Channel_66",
    "Channel_67",
    "Channel_68",
    "Channel_69",
    "Channel_70",
    "Channel_71",
    "CALIB_1",
    "Reference_Voltage_1",
    "Global_Analog_1",
    "Master_TDC_1",
    "Digital_Half_1",
    "HalfWise_1",
    "CM_0",
    "CM_1",
    "Channel_0",
    "Channel_1",
    "Channel_2",
    "Channel_3",
    "Channel_4",
    "Channel_5",
    "Channel_6",
    "Channel_7",
    "Channel_8",
    "Channel_9",
    "Channel_10",
    "Channel_11",
    "Channel_12",
    "Channel_13",
    "Channel_14",
    "Channel_15",
    "Channel_16",
    "Channel_17",
    "Channel_18",
    "Channel_19",
    "Channel_20",
    "Channel_21",
    "Channel_22",
    "Channel_23",
    "Channel_24",
    "Channel_25",
    "Channel_26",
    "Channel_27",
    "Channel_28",
    "Channel_29",
    "Channel_30",
    "Channel_31",
    "Channel_32",
    "Channel_33",
    "Channel_34",
    "Channel_35",
    "CALIB_0",
    "Reference_Voltage_0",
    "Global_Analog_0",
    "Master_TDC_0",
    "Digital_Half_0",
    "HalfWise_0"
]
# ------------------------------------------------------------------------------------------------------------------------------------------------
# Global Things
# ------------------------------------------------------------------------------------------------------------------------------------------------
HOST   = "10.1.2.207"
UDP_IP = f"10.1.2.{208 + fpga_address}"
PORT   = 11000
# ------------------------------------------------------------------------------------------------------------------------------------------------
# Basic two commands
# ------------------------------------------------------------------------------------------------------------------------------------------------
WriteRegister   = [0xa0,0x00,0x11,0x00,0x00,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
ReadRegisters   = [0xa0,0x00,0x10,0x00,0x00,0x95,0x05,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
WriteRegister_temp   = [0xa0,0x00,0x11,0x00,0x00,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
ReadRegisters_temp   = [0xa0,0x00,0x10,0x00,0x00,0x95,0x05,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
# ------------------------------------------------------------------------------------------------------------------------------------------------
# Different addresses initialization
# ------------------------------------------------------------------------------------------------------------------------------------------------
ReadRegisters_temp[0] = 160 + asic_address
ReadRegisters_temp[1] = fpga_address
WriteRegister_temp[0] = 160 + asic_address
WriteRegister_temp[1] = fpga_address
# ------------------------------------------------------------------------------------------------------------------------------------------------
# Socket Init...
# ------------------------------------------------------------------------------------------------------------------------------------------------
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM , 0)
s.bind((HOST,PORT))
s.settimeout(5) # Sets the socket to timeout after "SocketTimeOut" second of no activity
# ------------------------------------------------------------------------------------------------------------------------------------------------
# Different addresses initialization
# ------------------------------------------------------------------------------------------------------------------------------------------------
CFG_Top                 = [0x15,   0x05,   0xa0]
CFG_Digital_half_0      = [0x1b,   0x0b,   0x20]
CFG_Digital_half_1      = [0x1b,   0x05,   0x60]
CFG_Global_Analog_0     = [0x0f,   0x0a,   0xe0]
CFG_Global_Analog_1     = [0x0f,   0x05,   0x20]
CFG_Master_TDC_0        = [0x10,   0x0b,   0x00]
CFG_Master_TDC_1        = [0x10,   0x05,   0x40]
CFG_Reference_Voltage_0 = [0x0b,   0x0a,   0xc0]
CFG_Reference_Voltage_1 = [0x0b,   0x05,   0x00]
CFG_Half_Wise_0         = [0x0f,   0x0b,   0x40]
CFG_Half_Wise_1         = [0x0f,   0x05,   0x80]
Channels                = [0x0f,   0x00,   0x00] #this a dummy for channels
CFG_CM_2                = [0x0f,   0x00,   0x00]
CFG_CM_3                = [0x0f,   0x00,   0x20]
CFG_CM_0                = [0x0f,   0x05,   0xc0]
CFG_CM_1                = [0x0f,   0x05,   0xe0]
CFG_CALIB_0             = [0x0f,   0x0a,   0x80]
CFG_CALIB_1             = [0x0f,   0x04,   0xe0]

# ------------------------------------------------------------------------------------------------------------------------------------------------
# Now read and write all the registers
# ------------------------------------------------------------------------------------------------------------------------------------------------
for register_name in register_names:
    #remove everything first
    for i in range(5, 40):
        ReadRegisters_temp[i] = 0
        
    if register_name == "CM_0":
        ReadRegisters_temp[5] = CFG_CM_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_CM_0[1]
        ReadRegisters_temp[7] = CFG_CM_0[2]
    if register_name == "CM_1":
        ReadRegisters_temp[5] = CFG_CM_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_CM_1[1]
        ReadRegisters_temp[7] = CFG_CM_1[2]
    if register_name == "CM_2":
        ReadRegisters_temp[5] = CFG_CM_2[0] + 0x80
        ReadRegisters_temp[6] = CFG_CM_2[1]
        ReadRegisters_temp[7] = CFG_CM_2[2]
    if register_name == "CM_3":
        ReadRegisters_temp[5] = CFG_CM_3[0] + 0x80
        ReadRegisters_temp[6] = CFG_CM_3[1]
        ReadRegisters_temp[7] = CFG_CM_3[2]


    if register_name == "CALIB_0":
        ReadRegisters_temp[5] = CFG_CALIB_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_CALIB_0[1]
        ReadRegisters_temp[7] = CFG_CALIB_0[2]
    if register_name == "CALIB_1":
        ReadRegisters_temp[5] = CFG_CALIB_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_CALIB_1[1]
        ReadRegisters_temp[7] = CFG_CALIB_1[2]


    if register_name == "Top":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Top[0] + 0x80
        ReadRegisters_temp[6] = CFG_Top[1]
        ReadRegisters_temp[7] = CFG_Top[2]
        
    if register_name == "Digital_Half_0":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Digital_half_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_Digital_half_0[1]
        ReadRegisters_temp[7] = CFG_Digital_half_0[2]
        
    if register_name == "Digital_Half_1":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Digital_half_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_Digital_half_1[1]
        ReadRegisters_temp[7] = CFG_Digital_half_1[2]
                
    if register_name == "Global_Analog_0":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Global_Analog_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_Global_Analog_0[1]
        ReadRegisters_temp[7] = CFG_Global_Analog_0[2]
    if register_name == "Global_Analog_1":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Global_Analog_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_Global_Analog_1[1]
        ReadRegisters_temp[7] = CFG_Global_Analog_1[2]

    if register_name == "Master_TDC_0":
        #print(f"{registername} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Master_TDC_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_Master_TDC_0[1]
        ReadRegisters_temp[7] = CFG_Master_TDC_0[2]
    if register_name == "Master_TDC_1":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Master_TDC_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_Master_TDC_1[1]
        ReadRegisters_temp[7] = CFG_Master_TDC_1[2]
        
    if register_name == "Reference_Voltage_0":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Reference_Voltage_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_Reference_Voltage_0[1]
        ReadRegisters_temp[7] = CFG_Reference_Voltage_0[2]
    if register_name == "Reference_Voltage_1":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Reference_Voltage_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_Reference_Voltage_1[1]
        ReadRegisters_temp[7] = CFG_Reference_Voltage_1[2]

    if register_name == "HalfWise_0":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Half_Wise_0[0] + 0x80
        ReadRegisters_temp[6] = CFG_Half_Wise_0[1]
        ReadRegisters_temp[7] = CFG_Half_Wise_0[2]
    if register_name == "HalfWise_1":
        #print(f"{register_name} = {hex_values}")
        ReadRegisters_temp[5] = CFG_Half_Wise_1[0] + 0x80
        ReadRegisters_temp[6] = CFG_Half_Wise_1[1]
        ReadRegisters_temp[7] = CFG_Half_Wise_1[2]        
                
    if register_name.startswith("Channel_"):
        try:
            ich = int(register_name.split("_")[1])
        except ValueError:
            continue  # Skip if the channel number is invalid
        if ich < 36:
            decimal_value = 1536 + ich * 32
            high_dec = decimal_value // 256
            low_dec = decimal_value % 256
            ReadRegisters_temp[5] = 0x8f
            ReadRegisters_temp[6] = int(high_dec)
            ReadRegisters_temp[7] = int(low_dec)
        if ich > 35:
            decimal_value = 64 + (ich-36)*32
            high_dec = decimal_value // 256
            low_dec = decimal_value % 256
            ReadRegisters_temp[5] = 0x8f
            ReadRegisters_temp[6] = int(high_dec)
            ReadRegisters_temp[7] = int(low_dec)

    #send to the ASIC:
    print([hex(val) for val in ReadRegisters_temp])
    s.sendto(bytes(ReadRegisters_temp), (UDP_IP, PORT))
    data, add = s.recvfrom(1024)
    hex_string = binascii.hexlify(data).decode('utf-8')
    hex_string2 = r" 0x" + r" 0x".join(hex_string[n : n+2] for n in range(0, len(hex_string), 2))
    print("ASIC%d Register %s: %s" % (asic_address, register_name, hex_string2))

    
