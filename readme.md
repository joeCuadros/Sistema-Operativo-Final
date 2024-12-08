# Trabajo final de Sistema Operativo
dumpOS -> Sistemas operativos

## Componentes para la compilacion
- makefile
- gcc: Compilador de C `gcc (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0`
- gdb: `GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.2) 9.2`
- mmd:
- gdb: 
```bash
sudo apt install build-essential
sudo apt install mtools
sudo apt install gdb
```
- Qemu: Emulador de Sistema operativo 
- limine: Es un cargador de arranque (bootloader) y un firmware UEFI 
```bash 
git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1
```
- OVMFbin: Virtualización (QEMU/KVM)
```bash 
git clone https://github.com/Absurdponcho/OVMFbin
```
## Ejecucion del programa
Ejecutar para prepara archivos 
```bash 
make setup 
```
Compilar y ejecutar
```bash
build.sh
```

## Informacion de los archivos

### limine = descargado desde git 
### OVMFbin = Sistema de arranque ()
### principal
- linker.ld = construir kernel a ELF64 
- startup.nsh = inicializa y gestiona el arranque del sistema operativo
- makefile = Rapida compilacion 
- limine.cfg = Configuracion del bootloader limine
### src 
- limine.h = Importado desde la carpeta limine 
- limine_request.h  
- bootservices.h, bootservices.c  = servicio de bootloader 
- kernel.c = archivo principal para ejecutar
