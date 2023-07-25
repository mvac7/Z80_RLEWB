9000 '=================================
9010 ' unRLEWB to VRAM for MSX BASIC
9020 ' Decompress RLEWB data to VRAM
9030 ' Input: 
9040 '  RESTORE [line] <-- DATAs
9050 '              DE <-- VRAM address
9060 '=================================
9100 READ A
9110 IF A=128 THEN 9130
9120 VPOKE DE,A:DE=DE+1:GOTO 9100
9130 READ B
9140 IF B=255 THEN RETURN
9150 IF B=0 THEN 9120
9160 READ A
9170 FOR DE=DE TO DE+B:VPOKE DE,A:NEXT
9180 GOTO 9100 