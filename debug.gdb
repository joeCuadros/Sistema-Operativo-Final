file /mnt/f/Repositorios/Sistemas Operativos/dumpOS /build/bin/kernel.elf
target remote 172.24.208.1 :1234
set disassembly-flavor intel
b _start
c
