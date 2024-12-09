override IMG_RAW := raw.img
override KERNEL := kernel.elf
override LIMINECFG := limine.cfg
override ISO := limine-cd.iso
override IMG := limine-cd.img

# directorios
BUILDDIR := build/bin
OBJDIR := build/lib
BUILDHOME := build
ISOBUILDDIR := build/iso_root
ISODIR := build/image
LMNREPO := https://github.com/limine-bootloader/limine.git
LMNBRCH := v3.0-branch-binary
LMNDIR := limine
KERNEL_ENTRY := _start
BOOTEFI := ./limine/BOOTX64.EFI
SRCDIR := src

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
# compiladores 
QEMU := "/mnt/c/Program Files/qemu/qemu-system-x86_64.exe"
ASMC := "/mnt/c/Users/USUARIO/AppData/Local/bin/NASM/nasm.exe"
LD := /usr/bin/ld
CC := /usr/bin/cc

ABSDIR := $(shell pwd) # ruta absoluta
WSLHOSTIP := $(shell ipconfig.exe | grep 'vEthernet (WSL)' -a -A4 | tail -n1 | cut -d":" -f 2 | tail -n1 | sed -e 's/\s*//g') # dirrecion IP de wsl

GDBPORT ?= 1234
# flag de compilacion
#QFLAGSEXP ?= -cpu qemu64 -machine q35 -m 512 -boot d -d cpu_reset -drive if=pflash,format=raw,unit=0,file=./OVMFbin/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=./OVMFbin/OVMF_VARS-pure-efi.fd -net none -drive file=
# -m <num> (num -> ram)
QFLAGSEXP ?= -cpu qemu64 -machine q35 -m 256 -boot d -d cpu_reset -drive if=pflash,format=raw,unit=0,file=./OVMFbin/OVMF_CODE-pure-efi.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=./OVMFbin/OVMF_VARS-pure-efi.fd -net none -drive file=
CFLAGS ?= -O2 -g -Wall -Wextra -Wpedantic -pipe -std=c11 -fno-stack-protector
NASMFLAGS ?= -F dwarf -g


# Archivos
override CFILES :=$(call rwildcard,$(SRCDIR),*.c) 
override ASFILES := $(call rwildcard,$(SRCDIR),*.S)
override NASMFILES := $(call rwildcard,$(SRCDIR),*.asm)
override OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CFILES))
# para futuro ensamblador
# override OBJS += $(patsubst $(SRCDIR)/%.S, $(OBJDIR)/%_s.o, $(ASFILES))
# override OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(NASMFILES))
override LDFLAGS +=         \
    -nostdlib               \
    -static                 \
    -z max-page-size=0x1000 \
    -T linker.ld			
	
# reglas de compilacion
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@

#$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
#	@ mkdir -p $(@D)
#	@$(ASMC) $^ -felf64 -o $@
#
#$(OBJDIR)/%_s.o: $(SRCDIR)/%.S
#	@ mkdir -p $(@D)
#	@$(ASMC) $(NASMFLAGS) $^ -f elf64 -o $@


run:
	@echo "Construir kernel..."
	@make kernel
	@echo "Construir img..."
	@make buildimgexp
	@echo "Ejecutar QEMU..."
	@make run_exp
	@echo "Done!

setup:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(ISOBUILDDIR)
	@mkdir -p $(ISODIR)
	@dd if=/dev/zero of=$(ISODIR)/$(IMG_RAW) bs=4096 count=65527
	@git clone $(LMNREPO) --branch=$(LMNBRCH) --depth=1
	@cp -v $(LMNDIR)/limine.sys $(LMNDIR)/limine-cd.bin $(LMNDIR)/limine-cd-efi.bin $(ISOBUILDDIR)
	@echo file $(ABSDIR)/$(BUILDDIR)/$(KERNEL) > debug.gdb
	@echo target remote $(WSLHOSTIP):$(GDBPORT) >> debug.gdb
	@echo set disassembly-flavor intel >> debug.gdb
	@echo b $(KERNEL_ENTRY) >> debug.gdb
	@echo c >> debug.gdb

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(OBJS)
	@rm -f $(ISOBUILDDIR)/$(KERNEL)
	@rm -f $(ISOBUILDDIR)/$(LIMINECFG)
	@rm -f $(BUILDDIR)/$(KERNEL)
	@rm -f $(ISODIR)/$(ISO)

cleansetup:
	@rm -rf $(BUILDHOME)
	@rm -f debug.gdb
	@rm -rf $(LMNDIR)

kernel: $(OBJS) link

buildimgexp:
	@cp $(ISODIR)/$(IMG_RAW) $(ISODIR)/$(IMG)
	@mkfs.fat -F32 $(ISODIR)/$(IMG)
	@mmd -i $(ISODIR)/$(IMG) ::/EFI
	@mmd -i $(ISODIR)/$(IMG) ::/EFI/BOOT
	@mcopy -i $(ISODIR)/$(IMG) $(BOOTEFI) ::/EFI/BOOT
	@mcopy -i $(ISODIR)/$(IMG) ./startup.nsh ::
	@mcopy -i $(ISODIR)/$(IMG) $(BUILDDIR)/$(KERNEL) ::
	@mcopy -i $(ISODIR)/$(IMG) ./limine.cfg ::

run_exp:
	$(QEMU) $(QFLAGSEXP)$(ISODIR)/$(IMG)

link: 
	@$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)


help:
	@echo "make run = Compilar y ejecutar" 
	@echo "make clean = Limpiar" 
	@echo "make setup = inicializar carpeta y archivos necesarios " 
	@echo "make cleansetup = limpiar setup " 
	@echo "make run_exp = Solo ejecutar " 
