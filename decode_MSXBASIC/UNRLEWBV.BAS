4000 REM ==============================================
4010 REM unRLEWB to VRAM for BASIC v1 (06 Feb 2022)
4020 REM Decompress RLEWB data to VRAM
4030 REM Input: 
4040 REM        RESTORE [line number] --> RLEWB datas
4050 REM        HL --> VRAM address            
4060 REM ==============================================
4100 READ A
4110 IF A=&H80 THEN 4150
4120 VPOKE HL,A
4130 HL=HL+1
4140 GOTO 4100
4150 READ A
4160 IF A=&HFF THEN RETURN
4170 IF A=0 THEN A=&H80:GOTO 4120
4180 READ D
4190 FOR HL=HL TO HL+A
4200 VPOKE HL,D
4210 NEXT
4220 GOTO 4100   