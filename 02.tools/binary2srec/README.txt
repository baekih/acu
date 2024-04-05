1. Motorola .srec 파일 생성.
bin2srec.exe -o 8040000 -l 32 fi-din.bin > app.srec

2. .srec에서 .ax 파일 생성.
makeax app.srec /o app.ax /pd 10571 /pc 1 /bv 1 /fv 1 /su 00000000 /cm "ECOTRONIX Corp." /pn "ECO-DIN"