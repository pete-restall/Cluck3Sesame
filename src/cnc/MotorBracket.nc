%
( Use a Spoil Board, true and at least 5mm thick )
( Secure the sheet with clamps external to the profile )
( After the drilling, use self-tapping screws to secure the inside of the profile, leaving the clamps in place )
( Zero is the XY centre of the bottom-left hole, with Z on top of the aluminium sheet )
( Aluminium stock is assumed to be 1050 grade, 1.5mm thick )

(Project MotorBracket)
(Created by Estlcam version 11 build 11.107)
(Machining time about 00:03:53 hours)

(Required tools:)
(HSS Twist Drill, 3mm (R))
(HSS 1-Flute Endmill, 3.175mm)

G90
M03 S9000

G00 Z-20.0000


(No. 1: Drill 1)
G00 X0.0000 Y0.0000
G00 Z-0.5000
G01 Z2.0000 F300 S9000
G00 Z-0.5000
G00 Z1.5000
G01 Z3.0000
G00 Z-20.0000


(No. 2: Drill 2)
G00 X10.0000 Y0.0000 Z-20.0000
G00 Z-0.5000
G01 Z2.0000 F300
G00 Z-0.5000
G00 Z1.5000
G01 Z3.0000
G00 Z-20.0000


(No. 3: Drill 3)
G00 X20.0000 Y0.0000 Z-20.0000
G00 Z-0.5000
G01 Z2.0000 F300
G00 Z-0.5000
G00 Z1.5000
G01 Z3.0000
G00 Z-20.0000


(No. 4: Drill 4)
G00 X0.0000 Y14.4600 Z-20.0000
G00 Z-0.5000
G01 Z2.0000 F300
G00 Z-0.5000
G00 Z1.5000
G01 Z3.0000
G00 Z-20.0000


(No. 5: Drill 5)
G00 X0.0000 Y32.4600 Z-20.0000
G00 Z-0.5000
G01 Z2.0000 F300
G00 Z-0.5000
G00 Z1.5000
G01 Z3.0000
G00 Z-20.0000


(No. 6: Part 1)
M05
T1 M06 (Change tool: HSS 1-Flute Endmill, 3.175mm)
M03
G00 X0.0230 Y-6.5875 Z-20.0000
G00 Z-0.5000
G01 Z0.0000 F100 S12000
G01 X0.8890 Z0.2500
G01 X0.0230 Z0.5000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G01 X0.8890 Z0.7500 F100
G01 X0.0230 Z1.0000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G01 X0.8890 Z1.2500 F100
G01 X0.0230 Z1.5000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G01 X0.8890 Z1.7500 F100
G01 X0.0230 Z2.0000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G01 X0.8890 Z2.2500 F100
G01 X0.0230 Z2.5000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G01 X0.8890 Z2.7500 F100
G01 X0.0230 Z3.0000
G01 X25.0000 F1000
G03 X26.5875 Y-4.9800 I-0.0001 J1.5877
G01 Y7.3600
G03 X26.3380 Y8.2146 I-1.5901 J-0.0005
G01 X6.3485 Y40.3976
G03 X4.9800 Y41.1475 I-1.3489 J-0.8381
G01 X-5.0000
G03 X-6.5835 Y39.6723 I0.0000 J-1.5875
G01 X-6.5875 Y39.5400
G01 Y-5.0000
G03 X-6.5166 Y-5.4691 I1.5875 J0.0000
G03 X-4.9800 Y-6.5875 I1.5169 J0.4694
G01 X0.0230
G00 Z-20.0000


(No. 7: Engraving 1)
G00 X-5.0000 Y6.1800 Z-20.0000
G00 Z-0.5000
G01 Z0.0000 F100
G01 X-4.4804 Z0.1500
G01 X-5.0000 Z0.3000
G01 X25.0000 F1000
G00 Z-20.0000
G00 X0.0000 Y0.0000

M05
M30
%

