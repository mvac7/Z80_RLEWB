9000 '=================================
9010 ' unRLEWB to RAM for MSX BASIC 
9020 ' Decompress RLEWB data to RAM
9030 ' Input: 
9040 '  RESTORE [line] <-- DATAs
9050 '              DE <-- RAM address
9060 '=================================
9100 READ A
9110 IF A=128 THEN 9130
9120 POKE DE,A:DE=DE+1:GOTO 9100
9130 READ A
9140 IF A=255 THEN RETURN
9150 IF A=0 THEN A=128:GOTO 9120
9160 READ B
9170 FOR DE=DE TO DE+A:POKE DE,B:NEXT
9180 GOTO 9100  