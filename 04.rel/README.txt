1. Motorola .srec 파일 생성.
..\bin2srec.exe -o 800C000 -l 32 application.bin > application.srec

2. .srec에서 .ax 파일 생성.
..\makeax application.srec /o application.ax /pd 10571 /pc 1 /bv 50 /su 00000000 /cm "ECOTRONIX Corp." /pn "MCU-006" /fv 60