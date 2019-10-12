EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:mj-2135
LIBS:mounthole
LIBS:pcb-covox-amp-v2-rescue
LIBS:FTDI
LIBS:ferrite
LIBS:pcb-covox-amp-v2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Covox with Amplifier v2"
Date "2016-07-25"
Rev "1"
Comp ""
Comment1 "Designed by Yeo Kheng Meng"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L DB25 J1
U 1 1 57761B75
P 1000 1950
F 0 "J1" H 1050 3300 50  0000 C CNN
F 1 "DB25" H 950 600 50  0000 C CNN
F 2 "db25:DB25MC" H 1000 1950 50  0001 C CNN
F 3 "" H 1000 1950 50  0000 C CNN
	1    1000 1950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR01
U 1 1 57761E43
P 2100 3300
F 0 "#PWR01" H 2100 3050 50  0001 C CNN
F 1 "GND" H 2100 3150 50  0000 C CNN
F 2 "" H 2100 3300 50  0000 C CNN
F 3 "" H 2100 3300 50  0000 C CNN
	1    2100 3300
	1    0    0    -1  
$EndComp
Text GLabel 1600 2350 2    60   Output ~ 0
Data7
Text GLabel 1600 2150 2    60   Output ~ 0
Data6
Text GLabel 1600 1950 2    60   Output ~ 0
Data5
Text GLabel 1600 1750 2    60   Output ~ 0
Data4
Text GLabel 1600 1550 2    60   Output ~ 0
Data3
Text GLabel 1600 1350 2    60   Output ~ 0
Data2
Text GLabel 1600 1150 2    60   Output ~ 0
Data1
Text GLabel 1600 950  2    60   Output ~ 0
Data0
$Comp
L R R4
U 1 1 577677AB
P 4500 900
F 0 "R4" V 4580 900 50  0000 C CNN
F 1 "20K" V 4500 900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 900 50  0001 C CNN
F 3 "" H 4500 900 50  0000 C CNN
	1    4500 900 
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 57767BD6
P 4500 1100
F 0 "R5" V 4580 1100 50  0000 C CNN
F 1 "20K" V 4500 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1100 50  0001 C CNN
F 3 "" H 4500 1100 50  0000 C CNN
	1    4500 1100
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 57767D92
P 4500 1300
F 0 "R6" V 4580 1300 50  0000 C CNN
F 1 "20K" V 4500 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1300 50  0001 C CNN
F 3 "" H 4500 1300 50  0000 C CNN
	1    4500 1300
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 57767D98
P 4500 1500
F 0 "R7" V 4580 1500 50  0000 C CNN
F 1 "20K" V 4500 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1500 50  0001 C CNN
F 3 "" H 4500 1500 50  0000 C CNN
	1    4500 1500
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 57767EC0
P 4500 1700
F 0 "R8" V 4580 1700 50  0000 C CNN
F 1 "20K" V 4500 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1700 50  0001 C CNN
F 3 "" H 4500 1700 50  0000 C CNN
	1    4500 1700
	0    1    1    0   
$EndComp
$Comp
L R R9
U 1 1 57767EC6
P 4500 1900
F 0 "R9" V 4580 1900 50  0000 C CNN
F 1 "20K" V 4500 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 1900 50  0001 C CNN
F 3 "" H 4500 1900 50  0000 C CNN
	1    4500 1900
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 57767ECC
P 4500 2100
F 0 "R10" V 4580 2100 50  0000 C CNN
F 1 "20K" V 4500 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 2100 50  0001 C CNN
F 3 "" H 4500 2100 50  0000 C CNN
	1    4500 2100
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 57767ED2
P 4500 2300
F 0 "R11" V 4580 2300 50  0000 C CNN
F 1 "20K" V 4500 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 2300 50  0001 C CNN
F 3 "" H 4500 2300 50  0000 C CNN
	1    4500 2300
	0    1    1    0   
$EndComp
Text GLabel 4200 2300 0    60   Output ~ 0
Data7
Text GLabel 4200 2100 0    60   Output ~ 0
Data6
Text GLabel 4200 1900 0    60   Output ~ 0
Data5
Text GLabel 4200 1700 0    60   Output ~ 0
Data4
Text GLabel 4200 1500 0    60   Output ~ 0
Data3
Text GLabel 4200 1300 0    60   Output ~ 0
Data2
Text GLabel 4200 1100 0    60   Output ~ 0
Data1
Text GLabel 4200 900  0    60   Output ~ 0
Data0
$Comp
L R R12
U 1 1 57768432
P 5250 900
F 0 "R12" V 5330 900 50  0000 C CNN
F 1 "10K" V 5250 900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 900 50  0001 C CNN
F 3 "" H 5250 900 50  0000 C CNN
	1    5250 900 
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 577684C8
P 5250 1100
F 0 "R13" V 5330 1100 50  0000 C CNN
F 1 "10K" V 5250 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 1100 50  0001 C CNN
F 3 "" H 5250 1100 50  0000 C CNN
	1    5250 1100
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 577685B1
P 5250 1300
F 0 "R14" V 5330 1300 50  0000 C CNN
F 1 "10K" V 5250 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 1300 50  0001 C CNN
F 3 "" H 5250 1300 50  0000 C CNN
	1    5250 1300
	0    1    1    0   
$EndComp
$Comp
L R R15
U 1 1 577685B7
P 5250 1500
F 0 "R15" V 5330 1500 50  0000 C CNN
F 1 "10K" V 5250 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 1500 50  0001 C CNN
F 3 "" H 5250 1500 50  0000 C CNN
	1    5250 1500
	0    1    1    0   
$EndComp
$Comp
L R R16
U 1 1 57768723
P 5250 1700
F 0 "R16" V 5330 1700 50  0000 C CNN
F 1 "10K" V 5250 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 1700 50  0001 C CNN
F 3 "" H 5250 1700 50  0000 C CNN
	1    5250 1700
	0    1    1    0   
$EndComp
$Comp
L R R17
U 1 1 57768729
P 5250 1900
F 0 "R17" V 5330 1900 50  0000 C CNN
F 1 "10K" V 5250 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 1900 50  0001 C CNN
F 3 "" H 5250 1900 50  0000 C CNN
	1    5250 1900
	0    1    1    0   
$EndComp
$Comp
L R R18
U 1 1 5776872F
P 5250 2100
F 0 "R18" V 5330 2100 50  0000 C CNN
F 1 "10K" V 5250 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 2100 50  0001 C CNN
F 3 "" H 5250 2100 50  0000 C CNN
	1    5250 2100
	0    1    1    0   
$EndComp
$Comp
L R R19
U 1 1 57768735
P 5250 2300
F 0 "R19" V 5330 2300 50  0000 C CNN
F 1 "10K" V 5250 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 2300 50  0001 C CNN
F 3 "" H 5250 2300 50  0000 C CNN
	1    5250 2300
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 5776A199
P 4500 700
F 0 "R3" V 4580 700 50  0000 C CNN
F 1 "20K" V 4500 700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4430 700 50  0001 C CNN
F 3 "" H 4500 700 50  0000 C CNN
	1    4500 700 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR02
U 1 1 5776A1FD
P 3750 700
F 0 "#PWR02" H 3750 450 50  0001 C CNN
F 1 "GND" H 3750 550 50  0000 C CNN
F 2 "" H 3750 700 50  0000 C CNN
F 3 "" H 3750 700 50  0000 C CNN
	1    3750 700 
	0    1    1    0   
$EndComp
$Comp
L C C8
U 1 1 5776C2EF
P 5500 2550
F 0 "C8" H 5525 2650 50  0000 L CNN
F 1 "0.01uF" H 5525 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5538 2400 50  0001 C CNN
F 3 "" H 5500 2550 50  0000 C CNN
	1    5500 2550
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5776C3AA
P 5950 2550
F 0 "C9" H 5975 2650 50  0000 L CNN
F 1 "100nF" H 5975 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5988 2400 50  0001 C CNN
F 3 "" H 5950 2550 50  0000 C CNN
	1    5950 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5776C54D
P 5500 2800
F 0 "#PWR03" H 5500 2550 50  0001 C CNN
F 1 "GND" H 5500 2650 50  0000 C CNN
F 2 "" H 5500 2800 50  0000 C CNN
F 3 "" H 5500 2800 50  0000 C CNN
	1    5500 2800
	1    0    0    -1  
$EndComp
Text GLabel 6050 2850 2    60   Input ~ 0
Source-Audio
Text GLabel 9650 850  0    60   Input ~ 0
Source-Audio
$Comp
L MJ-2135MJ-2135 X2
U 1 1 5776E109
P 10800 950
F 0 "X2" H 10600 550 45  0000 L BNN
F 1 "Raw" H 10800 950 45  0001 L BNN
F 2 "mj2135:mj-2135-MJ-2135" H 10830 1100 20  0001 C CNN
F 3 "" H 10800 950 60  0000 C CNN
F 4 "Raw Source Audio" H 10850 1150 60  0000 C CNN "Purpose"
	1    10800 950 
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR04
U 1 1 5776E5B2
P 10000 750
F 0 "#PWR04" H 10000 500 50  0001 C CNN
F 1 "GND" H 10000 600 50  0000 C CNN
F 2 "" H 10000 750 50  0000 C CNN
F 3 "" H 10000 750 50  0000 C CNN
	1    10000 750 
	0    1    1    0   
$EndComp
$Comp
L USB_OTG P1
U 1 1 577736E1
P 7400 1650
F 0 "P1" H 7725 1525 50  0000 C CNN
F 1 "USB" H 7400 1850 50  0000 C CNN
F 2 "Connect:USB_Micro-B" V 7350 1550 50  0001 C CNN
F 3 "" V 7350 1550 50  0000 C CNN
	1    7400 1650
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR05
U 1 1 57774D70
P 7850 1450
F 0 "#PWR05" H 7850 1200 50  0001 C CNN
F 1 "GND" H 7850 1300 50  0000 C CNN
F 2 "" H 7850 1450 50  0000 C CNN
F 3 "" H 7850 1450 50  0000 C CNN
	1    7850 1450
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR06
U 1 1 57774ECD
P 8350 1850
F 0 "#PWR06" H 8350 1700 50  0001 C CNN
F 1 "+5V" H 8350 1990 50  0000 C CNN
F 2 "" H 8350 1850 50  0000 C CNN
F 3 "" H 8350 1850 50  0000 C CNN
	1    8350 1850
	0    1    1    0   
$EndComp
$Comp
L POT-RESCUE-pcb-covox-amp RV1
U 1 1 5777A309
P 9850 3000
F 0 "RV1" H 9850 2900 50  0000 C CNN
F 1 "10K" H 9850 3000 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Alps-RK163-single_15mm" H 9850 3000 50  0001 C CNN
F 3 "" H 9850 3000 50  0000 C CNN
F 4 "Volume" H 9850 2800 60  0000 C CNN "Purpose"
	1    9850 3000
	1    0    0    -1  
$EndComp
$Comp
L POT-RESCUE-pcb-covox-amp RV2
U 1 1 5777A420
P 9800 6000
F 0 "RV2" H 9800 5900 50  0000 C CNN
F 1 "10K" H 9800 6000 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Alps-RK163-single_15mm" H 9800 6000 50  0001 C CNN
F 3 "" H 9800 6000 50  0000 C CNN
F 4 "Gain" H 9800 5800 60  0000 C CNN "Purpose"
	1    9800 6000
	1    0    0    -1  
$EndComp
$Comp
L LM386-RESCUE-pcb-covox-amp U1
U 1 1 57780AC9
P 2400 6450
F 0 "U1" H 2450 6750 50  0000 L CNN
F 1 "LM386" H 2100 6750 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2500 6550 50  0001 C CNN
F 3 "" H 2600 6650 50  0000 C CNN
	1    2400 6450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5778173C
P 2400 7300
F 0 "#PWR07" H 2400 7050 50  0001 C CNN
F 1 "GND" H 2400 7150 50  0000 C CNN
F 2 "" H 2400 7300 50  0000 C CNN
F 3 "" H 2400 7300 50  0000 C CNN
	1    2400 7300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 57781DE3
P 9450 3050
F 0 "#PWR08" H 9450 2800 50  0001 C CNN
F 1 "GND" H 9450 2900 50  0000 C CNN
F 2 "" H 9450 3050 50  0000 C CNN
F 3 "" H 9450 3050 50  0000 C CNN
	1    9450 3050
	1    0    0    -1  
$EndComp
Text GLabel 10200 3000 2    60   Input ~ 0
Source-Audio
Text GLabel 9850 2700 1    60   Input ~ 0
Input+
Text GLabel 1100 6350 0    60   Input ~ 0
Input+
$Comp
L POT-RESCUE-pcb-covox-amp RV3
U 1 1 5778C8B1
P 9850 4650
F 0 "RV3" H 9850 4550 50  0000 C CNN
F 1 "10K" H 9850 4650 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Alps-RK163-single_15mm" H 9850 4650 50  0001 C CNN
F 3 "" H 9850 4650 50  0000 C CNN
F 4 "Bass" H 9850 4450 60  0000 C CNN "Purpose"
	1    9850 4650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR09
U 1 1 57790DCC
P 2400 4800
F 0 "#PWR09" H 2400 4650 50  0001 C CNN
F 1 "+5V" H 2400 4940 50  0000 C CNN
F 2 "" H 2400 4800 50  0000 C CNN
F 3 "" H 2400 4800 50  0000 C CNN
	1    2400 4800
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5779107A
P 2050 5850
F 0 "C4" V 2100 5700 50  0000 L CNN
F 1 "100nF" V 2000 5550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2088 5700 50  0001 C CNN
F 3 "" H 2050 5850 50  0000 C CNN
	1    2050 5850
	0    1    1    0   
$EndComp
$Comp
L GND #PWR010
U 1 1 57791273
P 950 5450
F 0 "#PWR010" H 950 5200 50  0001 C CNN
F 1 "GND" H 950 5300 50  0000 C CNN
F 2 "" H 950 5450 50  0000 C CNN
F 3 "" H 950 5450 50  0000 C CNN
	1    950  5450
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 57792DDC
P 1550 6050
F 0 "C1" H 1575 6150 50  0000 L CNN
F 1 "470pF" H 1575 5950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1588 5900 50  0001 C CNN
F 3 "" H 1550 6050 50  0000 C CNN
	1    1550 6050
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 57793C57
P 2050 5450
F 0 "C3" V 2100 5300 50  0000 L CNN
F 1 "100nF" V 2000 5150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2088 5300 50  0001 C CNN
F 3 "" H 2050 5450 50  0000 C CNN
	1    2050 5450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR011
U 1 1 57794F51
P 3650 5800
F 0 "#PWR011" H 3650 5550 50  0001 C CNN
F 1 "GND" H 3650 5650 50  0000 C CNN
F 2 "" H 3650 5800 50  0000 C CNN
F 3 "" H 3650 5800 50  0000 C CNN
	1    3650 5800
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 57795320
P 2850 5800
F 0 "R1" V 2930 5800 50  0000 C CNN
F 1 "10K" V 2850 5800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2780 5800 50  0001 C CNN
F 3 "" H 2850 5800 50  0000 C CNN
	1    2850 5800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR012
U 1 1 57797AC5
P 1100 6550
F 0 "#PWR012" H 1100 6300 50  0001 C CNN
F 1 "GND" H 1100 6400 50  0000 C CNN
F 2 "" H 1100 6550 50  0000 C CNN
F 3 "" H 1100 6550 50  0000 C CNN
	1    1100 6550
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 57799CC6
P 3150 6450
F 0 "R2" V 3230 6450 50  0000 C CNN
F 1 "10" V 3150 6450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3080 6450 50  0001 C CNN
F 3 "" H 3150 6450 50  0000 C CNN
	1    3150 6450
	0    1    1    0   
$EndComp
$Comp
L CP1 C2
U 1 1 5779AAF6
P 2050 5050
F 0 "C2" V 2100 4850 50  0000 L CNN
F 1 "100uF" V 2000 4750 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_10x10.5" H 2050 5050 50  0001 C CNN
F 3 "" H 2050 5050 50  0000 C CNN
	1    2050 5050
	0    1    1    0   
$EndComp
$Comp
L CP1 C5
U 1 1 5779C1C1
P 3300 5800
F 0 "C5" V 3350 5850 50  0000 L CNN
F 1 "10uF" V 3350 5550 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x5.3" H 3300 5800 50  0001 C CNN
F 3 "" H 3300 5800 50  0000 C CNN
	1    3300 5800
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C7
U 1 1 5779D741
P 3900 6450
F 0 "C7" V 3950 6550 50  0000 L CNN
F 1 "1000uF" V 3950 6100 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_10x10.5" H 3900 6450 50  0001 C CNN
F 3 "" H 3900 6450 50  0000 C CNN
	1    3900 6450
	0    -1   -1   0   
$EndComp
$Comp
L C C6
U 1 1 5779F392
P 3550 6700
F 0 "C6" H 3650 6600 50  0000 L CNN
F 1 "100nF" H 3300 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3588 6550 50  0001 C CNN
F 3 "" H 3550 6700 50  0000 C CNN
	1    3550 6700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5779FAD8
P 3550 7000
F 0 "#PWR013" H 3550 6750 50  0001 C CNN
F 1 "GND" H 3550 6850 50  0000 C CNN
F 2 "" H 3550 7000 50  0000 C CNN
F 3 "" H 3550 7000 50  0000 C CNN
	1    3550 7000
	1    0    0    -1  
$EndComp
Text GLabel 4600 6450 2    60   Input ~ 0
Amp-Audio
Text GLabel 2900 6700 3    60   Input ~ 0
Vout
Text GLabel 2500 6800 3    60   Input ~ 0
Gain1
Text GLabel 2600 6800 3    60   Input ~ 0
Gain8
$Comp
L CP1 C10
U 1 1 577AE6CB
P 10450 6000
F 0 "C10" V 10500 6050 50  0000 L CNN
F 1 "10uF" V 10500 5750 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_4x5.3" H 10450 6000 50  0001 C CNN
F 3 "" H 10450 6000 50  0000 C CNN
	1    10450 6000
	0    -1   -1   0   
$EndComp
Text GLabel 9800 5600 1    60   Input ~ 0
Gain1
Text GLabel 10750 6000 2    60   Input ~ 0
Gain8
$Comp
L C C11
U 1 1 577B1E46
P 9850 4100
F 0 "C11" H 9875 4200 50  0000 L CNN
F 1 "33000pF" H 9875 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9888 3950 50  0001 C CNN
F 3 "" H 9850 4100 50  0000 C CNN
	1    9850 4100
	1    0    0    -1  
$EndComp
Text GLabel 9850 3750 1    60   Input ~ 0
Vout
Text GLabel 10200 4650 2    60   Input ~ 0
Gain1
Text GLabel 9650 1650 0    60   Input ~ 0
Amp-Audio
$Comp
L MJ-2135MJ-2135 X1
U 1 1 577B6906
P 10800 1750
F 0 "X1" H 10600 1350 45  0000 L BNN
F 1 "Amp" H 10800 1750 45  0001 L BNN
F 2 "mj2135:mj-2135-MJ-2135" H 10830 1900 20  0001 C CNN
F 3 "" H 10800 1750 60  0000 C CNN
F 4 "Amp Audio" H 10850 1950 60  0000 C CNN "Purpose"
	1    10800 1750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR014
U 1 1 577B690C
P 10000 1550
F 0 "#PWR014" H 10000 1300 50  0001 C CNN
F 1 "GND" H 10000 1400 50  0000 C CNN
F 2 "" H 10000 1550 50  0000 C CNN
F 3 "" H 10000 1550 50  0000 C CNN
	1    10000 1550
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 57777603
P 8300 2500
F 0 "D1" H 8300 2600 50  0000 C CNN
F 1 "Yellow" H 8300 2400 50  0000 C CNN
F 2 "LEDs:LED_0805" H 8300 2500 50  0001 C CNN
F 3 "" H 8300 2500 50  0000 C CNN
	1    8300 2500
	0    -1   -1   0   
$EndComp
$Comp
L R R20
U 1 1 57777EB2
P 8300 2100
F 0 "R20" V 8380 2100 50  0000 C CNN
F 1 "1500" V 8300 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8230 2100 50  0001 C CNN
F 3 "" H 8300 2100 50  0000 C CNN
	1    8300 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 57777FD1
P 8300 2800
F 0 "#PWR015" H 8300 2550 50  0001 C CNN
F 1 "GND" H 8300 2650 50  0000 C CNN
F 2 "" H 8300 2800 50  0000 C CNN
F 3 "" H 8300 2800 50  0000 C CNN
	1    8300 2800
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 5777EBC8
P 7550 950
F 0 "C12" H 7575 1050 50  0000 L CNN
F 1 "100nF" H 7575 850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7588 800 50  0001 C CNN
F 3 "" H 7550 950 50  0000 C CNN
	1    7550 950 
	0    1    1    0   
$EndComp
$Comp
L MOUNTHOLE MH1
U 1 1 5778DB2B
P 6100 1000
F 0 "MH1" H 6250 1050 50  0001 C CNN
F 1 "MOUNTHOLE" H 6100 1150 50  0001 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_ISO14580_Pad" H 6100 1000 50  0001 C CNN
F 3 "" H 6100 1000 50  0000 C CNN
	1    6100 1000
	1    0    0    -1  
$EndComp
$Comp
L MOUNTHOLE MH3
U 1 1 5778DB91
P 6550 1000
F 0 "MH3" H 6700 1050 50  0001 C CNN
F 1 "MOUNTHOLE" H 6550 1150 50  0001 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_ISO14580_Pad" H 6550 1000 50  0001 C CNN
F 3 "" H 6550 1000 50  0000 C CNN
	1    6550 1000
	1    0    0    -1  
$EndComp
$Comp
L MOUNTHOLE MH2
U 1 1 5778DBF7
P 6100 1250
F 0 "MH2" H 6250 1300 50  0001 C CNN
F 1 "MOUNTHOLE" H 6100 1400 50  0001 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_ISO14580_Pad" H 6100 1250 50  0001 C CNN
F 3 "" H 6100 1250 50  0000 C CNN
	1    6100 1250
	1    0    0    -1  
$EndComp
$Comp
L MOUNTHOLE MH4
U 1 1 5778DC5D
P 6550 1250
F 0 "MH4" H 6700 1300 50  0001 C CNN
F 1 "MOUNTHOLE" H 6550 1400 50  0001 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_ISO14580_Pad" H 6550 1250 50  0001 C CNN
F 3 "" H 6550 1250 50  0000 C CNN
	1    6550 1250
	1    0    0    -1  
$EndComp
Text GLabel 7800 1650 2    60   Input ~ 0
USB_D+
Text GLabel 7800 1750 2    60   Input ~ 0
USB_D-
$Comp
L FERRITE FB1
U 1 1 5794D0AD
P 8000 1850
F 0 "FB1" H 7925 1775 40  0000 L CNN
F 1 "FERRITE" H 7925 1700 40  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 8000 1750 60  0001 C CNN
F 3 "" H 8000 1750 60  0000 C CNN
	1    8000 1850
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 5794D370
P 7750 2100
F 0 "C16" H 7600 2200 50  0000 L CNN
F 1 "10nF" H 7775 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7788 1950 50  0001 C CNN
F 3 "" H 7750 2100 50  0000 C CNN
	1    7750 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5794D3FF
P 7750 2800
F 0 "#PWR016" H 7750 2550 50  0001 C CNN
F 1 "GND" H 7750 2650 50  0000 C CNN
F 2 "" H 7750 2800 50  0000 C CNN
F 3 "" H 7750 2800 50  0000 C CNN
	1    7750 2800
	1    0    0    -1  
$EndComp
$Comp
L FT245RL U2
U 1 1 57953332
P 6750 4450
F 0 "U2" H 6750 4500 60  0000 C CNN
F 1 "FT245RL" H 6750 4600 60  0000 C CNN
F 2 "Housings_SSOP:SSOP-28_5.3x10.2mm_Pitch0.65mm" H 6750 4450 60  0001 C CNN
F 3 "" H 6750 4450 60  0000 C CNN
	1    6750 4450
	1    0    0    -1  
$EndComp
Text GLabel 5800 4150 0    60   Input ~ 0
USB_D+
Text GLabel 5800 4050 0    60   Input ~ 0
USB_D-
$Comp
L +5V #PWR017
U 1 1 57956CFA
P 5100 3500
F 0 "#PWR017" H 5100 3350 50  0001 C CNN
F 1 "+5V" H 5100 3640 50  0000 C CNN
F 2 "" H 5100 3500 50  0000 C CNN
F 3 "" H 5100 3500 50  0000 C CNN
	1    5100 3500
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 57956D66
P 5100 4100
F 0 "C14" H 5125 4200 50  0000 L CNN
F 1 "100nF" H 5125 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5138 3950 50  0001 C CNN
F 3 "" H 5100 4100 50  0000 C CNN
	1    5100 4100
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 57957EB6
P 4850 4100
F 0 "C13" H 4875 4200 50  0000 L CNN
F 1 "4.7uF" H 4875 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4888 3950 50  0001 C CNN
F 3 "" H 4850 4100 50  0000 C CNN
	1    4850 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 579584B5
P 5100 4400
F 0 "#PWR018" H 5100 4150 50  0001 C CNN
F 1 "GND" H 5100 4250 50  0000 C CNN
F 2 "" H 5100 4400 50  0000 C CNN
F 3 "" H 5100 4400 50  0000 C CNN
	1    5100 4400
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 579596CB
P 5550 5150
F 0 "C15" H 5575 5250 50  0000 L CNN
F 1 "100nF" H 5575 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5588 5000 50  0001 C CNN
F 3 "" H 5550 5150 50  0000 C CNN
	1    5550 5150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5795975E
P 6700 5850
F 0 "#PWR019" H 6700 5600 50  0001 C CNN
F 1 "GND" H 6700 5700 50  0000 C CNN
F 2 "" H 6700 5850 50  0000 C CNN
F 3 "" H 6700 5850 50  0000 C CNN
	1    6700 5850
	1    0    0    -1  
$EndComp
$Comp
L R R29
U 1 1 5795BFB4
P 7900 5100
F 0 "R29" V 7980 5100 50  0000 C CNN
F 1 "10K" V 7900 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7830 5100 50  0001 C CNN
F 3 "" H 7900 5100 50  0000 C CNN
	1    7900 5100
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR020
U 1 1 5795C049
P 8250 5100
F 0 "#PWR020" H 8250 4950 50  0001 C CNN
F 1 "+5V" H 8250 5240 50  0000 C CNN
F 2 "" H 8250 5100 50  0000 C CNN
F 3 "" H 8250 5100 50  0000 C CNN
	1    8250 5100
	0    1    1    0   
$EndComp
$Comp
L R R21
U 1 1 5795CC51
P 7800 3800
F 0 "R21" V 7750 4000 50  0000 C CNN
F 1 "100" V 7800 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 3800 50  0001 C CNN
F 3 "" H 7800 3800 50  0000 C CNN
	1    7800 3800
	0    1    1    0   
$EndComp
$Comp
L R R22
U 1 1 5795CFA9
P 7800 3900
F 0 "R22" V 7750 4100 50  0000 C CNN
F 1 "100" V 7800 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 3900 50  0001 C CNN
F 3 "" H 7800 3900 50  0000 C CNN
	1    7800 3900
	0    1    1    0   
$EndComp
$Comp
L R R23
U 1 1 5795D087
P 7800 4000
F 0 "R23" V 7750 4200 50  0000 C CNN
F 1 "100" V 7800 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4000 50  0001 C CNN
F 3 "" H 7800 4000 50  0000 C CNN
	1    7800 4000
	0    1    1    0   
$EndComp
$Comp
L R R24
U 1 1 5795D08D
P 7800 4100
F 0 "R24" V 7750 4300 50  0000 C CNN
F 1 "100" V 7800 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4100 50  0001 C CNN
F 3 "" H 7800 4100 50  0000 C CNN
	1    7800 4100
	0    1    1    0   
$EndComp
$Comp
L R R25
U 1 1 5795D4DB
P 7800 4200
F 0 "R25" V 7750 4400 50  0000 C CNN
F 1 "100" V 7800 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4200 50  0001 C CNN
F 3 "" H 7800 4200 50  0000 C CNN
	1    7800 4200
	0    1    1    0   
$EndComp
$Comp
L R R26
U 1 1 5795D4E1
P 7800 4300
F 0 "R26" V 7750 4500 50  0000 C CNN
F 1 "100" V 7800 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4300 50  0001 C CNN
F 3 "" H 7800 4300 50  0000 C CNN
	1    7800 4300
	0    1    1    0   
$EndComp
$Comp
L R R27
U 1 1 5795D4E7
P 7800 4400
F 0 "R27" V 7750 4600 50  0000 C CNN
F 1 "100" V 7800 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4400 50  0001 C CNN
F 3 "" H 7800 4400 50  0000 C CNN
	1    7800 4400
	0    1    1    0   
$EndComp
$Comp
L R R28
U 1 1 5795D4ED
P 7800 4500
F 0 "R28" V 7750 4700 50  0000 C CNN
F 1 "100" V 7800 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7730 4500 50  0001 C CNN
F 3 "" H 7800 4500 50  0000 C CNN
	1    7800 4500
	0    1    1    0   
$EndComp
Text GLabel 8200 4500 2    60   Output ~ 0
Data7
Text GLabel 8200 4400 2    60   Output ~ 0
Data6
Text GLabel 8200 4300 2    60   Output ~ 0
Data5
Text GLabel 8200 4200 2    60   Output ~ 0
Data4
Text GLabel 8200 4100 2    60   Output ~ 0
Data3
Text GLabel 8200 4000 2    60   Output ~ 0
Data2
Text GLabel 8200 3900 2    60   Output ~ 0
Data1
Text GLabel 8200 3800 2    60   Output ~ 0
Data0
Wire Wire Line
	1450 1850 2100 1850
Wire Wire Line
	2100 1650 2100 3300
Wire Wire Line
	1450 2350 1600 2350
Wire Wire Line
	1450 2150 1600 2150
Wire Wire Line
	1450 1950 1600 1950
Wire Wire Line
	1450 1750 1600 1750
Wire Wire Line
	1450 1550 1600 1550
Wire Wire Line
	1450 1350 1600 1350
Wire Wire Line
	1450 1150 1600 1150
Wire Wire Line
	1450 950  1600 950 
Wire Wire Line
	4350 900  4200 900 
Wire Wire Line
	4350 1100 4200 1100
Wire Wire Line
	4350 1300 4200 1300
Wire Wire Line
	4350 1500 4200 1500
Wire Wire Line
	4350 1700 4200 1700
Wire Wire Line
	4350 1900 4200 1900
Wire Wire Line
	4350 2100 4200 2100
Wire Wire Line
	4350 2300 4200 2300
Wire Wire Line
	4650 900  5100 900 
Wire Wire Line
	4650 1100 5100 1100
Wire Wire Line
	4650 1300 5100 1300
Wire Wire Line
	4650 1500 5100 1500
Wire Wire Line
	4650 1700 5100 1700
Wire Wire Line
	4650 1900 5100 1900
Wire Wire Line
	4650 2100 5100 2100
Wire Wire Line
	4650 2300 5100 2300
Wire Wire Line
	4900 900  4900 700 
Wire Wire Line
	4900 700  4650 700 
Connection ~ 4900 900 
Wire Wire Line
	5400 900  5500 900 
Wire Wire Line
	5500 900  5500 1000
Wire Wire Line
	5500 1000 4900 1000
Wire Wire Line
	4900 1000 4900 1100
Connection ~ 4900 1100
Wire Wire Line
	5400 1100 5500 1100
Wire Wire Line
	5500 1100 5500 1200
Wire Wire Line
	5500 1200 4900 1200
Wire Wire Line
	4900 1200 4900 1300
Connection ~ 4900 1300
Wire Wire Line
	5400 1300 5500 1300
Wire Wire Line
	5500 1300 5500 1400
Wire Wire Line
	5500 1400 4900 1400
Wire Wire Line
	4900 1400 4900 1500
Connection ~ 4900 1500
Wire Wire Line
	5400 1500 5500 1500
Wire Wire Line
	5500 1500 5500 1600
Wire Wire Line
	5500 1600 4900 1600
Wire Wire Line
	4900 1600 4900 1700
Connection ~ 4900 1700
Wire Wire Line
	5400 1700 5500 1700
Wire Wire Line
	5500 1700 5500 1800
Wire Wire Line
	5500 1800 4900 1800
Wire Wire Line
	4900 1800 4900 1900
Connection ~ 4900 1900
Wire Wire Line
	5400 1900 5500 1900
Wire Wire Line
	5500 1900 5500 2000
Wire Wire Line
	5500 2000 4900 2000
Wire Wire Line
	4900 2000 4900 2100
Connection ~ 4900 2100
Wire Wire Line
	5400 2100 5500 2100
Wire Wire Line
	5500 2100 5500 2200
Wire Wire Line
	5500 2200 4900 2200
Wire Wire Line
	4900 2200 4900 2300
Connection ~ 4900 2300
Wire Wire Line
	5400 2300 5950 2300
Wire Wire Line
	5500 2300 5500 2400
Wire Wire Line
	5950 2300 5950 2400
Connection ~ 5500 2300
Wire Wire Line
	5950 2700 5950 2850
Wire Wire Line
	9650 850  10300 850 
Wire Wire Line
	10300 950  10050 950 
Wire Wire Line
	10050 950  10050 850 
Connection ~ 10050 850 
Wire Wire Line
	10000 750  10300 750 
Wire Wire Line
	7700 1450 7850 1450
Wire Wire Line
	3750 700  4350 700 
Wire Wire Line
	5500 2800 5500 2700
Wire Wire Line
	5950 2850 6050 2850
Wire Wire Line
	2400 6750 2400 7300
Wire Wire Line
	9850 2850 9850 2700
Wire Wire Line
	1100 6350 2100 6350
Wire Wire Line
	1100 6550 2100 6550
Wire Wire Line
	2400 4800 2400 6150
Wire Wire Line
	950  5450 1900 5450
Wire Wire Line
	1550 6200 1550 6350
Connection ~ 1550 6350
Wire Wire Line
	1550 5050 1550 5900
Connection ~ 1550 5450
Wire Wire Line
	2200 5850 2400 5850
Connection ~ 2400 5850
Wire Wire Line
	1900 5850 1550 5850
Connection ~ 1550 5850
Wire Wire Line
	2200 5450 2400 5450
Connection ~ 2400 5450
Wire Wire Line
	2200 5050 2400 5050
Connection ~ 2400 5050
Wire Wire Line
	1900 5050 1550 5050
Wire Wire Line
	3450 5800 3650 5800
Wire Wire Line
	2600 6200 2600 5800
Wire Wire Line
	2600 5800 2700 5800
Wire Wire Line
	3000 5800 3150 5800
Wire Wire Line
	2800 6450 3000 6450
Wire Wire Line
	3300 6450 3750 6450
Wire Wire Line
	3550 6550 3550 6450
Connection ~ 3550 6450
Wire Wire Line
	3550 7000 3550 6850
Wire Wire Line
	4050 6450 4600 6450
Wire Wire Line
	2900 6700 2900 6450
Connection ~ 2900 6450
Wire Wire Line
	2500 6800 2500 6750
Wire Wire Line
	2600 6800 2600 6750
Wire Wire Line
	9800 5850 9800 5600
Wire Wire Line
	10050 6000 10300 6000
Wire Wire Line
	10600 6000 10750 6000
Wire Wire Line
	9850 4500 9850 4250
Wire Wire Line
	9850 3950 9850 3750
Wire Wire Line
	9650 1650 10300 1650
Wire Wire Line
	10300 1750 10050 1750
Wire Wire Line
	10050 1750 10050 1650
Connection ~ 10050 1650
Wire Wire Line
	10000 1550 10300 1550
Wire Wire Line
	8300 2800 8300 2700
Wire Wire Line
	8300 2300 8300 2250
Wire Wire Line
	7700 950  7800 950 
Wire Wire Line
	7800 950  7800 1450
Connection ~ 7800 1450
Wire Wire Line
	7300 1250 7300 950 
Wire Wire Line
	7300 950  7400 950 
Wire Wire Line
	10100 3000 10200 3000
Wire Wire Line
	9600 3000 9450 3000
Wire Wire Line
	9450 3000 9450 3050
Wire Wire Line
	10200 4650 10100 4650
Wire Wire Line
	1450 1650 2100 1650
Connection ~ 2100 1850
Wire Wire Line
	1450 2050 2100 2050
Connection ~ 2100 2050
Wire Wire Line
	1450 2450 2100 2450
Connection ~ 2100 2450
Wire Wire Line
	1450 2250 2100 2250
Connection ~ 2100 2250
Wire Wire Line
	1450 2650 2100 2650
Connection ~ 2100 2650
Wire Wire Line
	1450 2850 2100 2850
Connection ~ 2100 2850
Wire Wire Line
	1450 3050 2100 3050
Connection ~ 2100 3050
Wire Wire Line
	7700 1650 7800 1650
Wire Wire Line
	7700 1750 7800 1750
Wire Wire Line
	7700 1850 7900 1850
Wire Wire Line
	8100 1850 8350 1850
Wire Wire Line
	8300 1950 8300 1850
Connection ~ 8300 1850
Wire Wire Line
	7750 1850 7750 1950
Connection ~ 7750 1850
Wire Wire Line
	7750 2250 7750 2800
Wire Wire Line
	5950 4050 5800 4050
Wire Wire Line
	5950 4150 5800 4150
Wire Wire Line
	4850 3800 5950 3800
Wire Wire Line
	5100 3500 5100 3950
Wire Wire Line
	4850 3950 4850 3800
Connection ~ 5100 3800
Wire Wire Line
	5950 3900 5100 3900
Connection ~ 5100 3900
Wire Wire Line
	5100 4250 5100 4400
Wire Wire Line
	4850 4250 4850 4350
Wire Wire Line
	4850 4350 5100 4350
Connection ~ 5100 4350
Wire Wire Line
	6900 5850 6900 5500
Wire Wire Line
	5550 5850 6900 5850
Wire Wire Line
	6750 5500 6750 5850
Connection ~ 6750 5850
Wire Wire Line
	6650 5500 6650 5850
Connection ~ 6700 5850
Wire Wire Line
	6550 5500 6550 5850
Connection ~ 6650 5850
Wire Wire Line
	6400 5500 6400 5850
Connection ~ 6550 5850
Wire Wire Line
	5550 5300 5550 5850
Connection ~ 6400 5850
Wire Wire Line
	5550 5000 5550 4850
Wire Wire Line
	5550 4850 5950 4850
Wire Wire Line
	7550 5100 7750 5100
Wire Wire Line
	8050 5100 8250 5100
Wire Wire Line
	7550 3800 7650 3800
Wire Wire Line
	7550 3900 7650 3900
Wire Wire Line
	7550 4000 7650 4000
Wire Wire Line
	7550 4100 7650 4100
Wire Wire Line
	7550 4200 7650 4200
Wire Wire Line
	7550 4300 7650 4300
Wire Wire Line
	7550 4400 7650 4400
Wire Wire Line
	7550 4500 7650 4500
Wire Wire Line
	7950 3800 8200 3800
Wire Wire Line
	7950 3900 8200 3900
Wire Wire Line
	7950 4000 8200 4000
Wire Wire Line
	7950 4100 8200 4100
Wire Wire Line
	7950 4200 8200 4200
Wire Wire Line
	7950 4300 8200 4300
Wire Wire Line
	7950 4400 8200 4400
Wire Wire Line
	7950 4500 8200 4500
$Comp
L CONN_01X10 P2
U 1 1 57980F23
P 3850 3550
F 0 "P2" H 3850 4100 50  0000 C CNN
F 1 "CONN_01X10" V 3950 3550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 3850 3550 50  0001 C CNN
F 3 "" H 3850 3550 50  0000 C CNN
	1    3850 3550
	1    0    0    -1  
$EndComp
Text GLabel 3500 3800 0    60   Output ~ 0
Data7
Text GLabel 3500 3700 0    60   Output ~ 0
Data6
Text GLabel 3500 3600 0    60   Output ~ 0
Data5
Text GLabel 3500 3500 0    60   Output ~ 0
Data4
Text GLabel 3500 3400 0    60   Output ~ 0
Data3
Text GLabel 3500 3300 0    60   Output ~ 0
Data2
Text GLabel 3500 3200 0    60   Output ~ 0
Data1
Text GLabel 3500 3100 0    60   Output ~ 0
Data0
$Comp
L GND #PWR021
U 1 1 57983A4A
P 3000 3900
F 0 "#PWR021" H 3000 3650 50  0001 C CNN
F 1 "GND" H 3000 3750 50  0000 C CNN
F 2 "" H 3000 3900 50  0000 C CNN
F 3 "" H 3000 3900 50  0000 C CNN
	1    3000 3900
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR022
U 1 1 57983AD0
P 2850 4000
F 0 "#PWR022" H 2850 3850 50  0001 C CNN
F 1 "+5V" H 2850 4140 50  0000 C CNN
F 2 "" H 2850 4000 50  0000 C CNN
F 3 "" H 2850 4000 50  0000 C CNN
	1    2850 4000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3500 3100 3650 3100
Wire Wire Line
	3650 3200 3500 3200
Wire Wire Line
	3500 3300 3650 3300
Wire Wire Line
	3650 3400 3500 3400
Wire Wire Line
	3500 3500 3650 3500
Wire Wire Line
	3650 3600 3500 3600
Wire Wire Line
	3500 3700 3650 3700
Wire Wire Line
	3650 3800 3500 3800
Wire Wire Line
	3650 3900 3000 3900
Wire Wire Line
	3650 4000 2850 4000
$EndSCHEMATC
