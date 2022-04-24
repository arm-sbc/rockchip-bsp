## rockchip-bsp
Rockchip vendor BSP, kernel version 4.4 and uboot version 2017

to compile install following packages on Ubuntu/Debian host machine tested on ubuntu 20/21/22

	sudo apt install flex bison gcc-arm-linux-gnueabihf gcc-aarch64-linux-gnu build-essential git debootstrap u-boot-tools device-tree-compiler  libssl-dev libncurses-dev binfmt-support qemu-user-static pkg-config automake sudo ibudev-dev libusb-1.0-0-dev dh-autoreconf gawk python3 python-is-python3  
	
then

	git clone https://github.com/arm-sbc/rockchip-bsp.git
	cd rockchip-bsp
	git clone https://github.com/arm-sbc/rk-kernel.git
	git clone https://github.com/arm-sbc/rk-uboot.git
	git clone https://github.com/arm-sbc/rkbin.git
	git clone https://github.com/arm-sbc/rkdeveloptool.git

**to install rkdeveloptool on host PC**

	cd rkdeveloptool
	( you need gcc 9 to compile rkdeveloptool)
	aclocal
	autoreconf -i
	autoheader
	automake --add-missing
	./configure
	sudo make
	sudo make install
	cd ..

**to cpmile u-boot**

	cd rk-uboot
	ls configs | grep 3399 or 3288 
( check correct defconfig)

	./build.sh evb-rk3399 
( this will create uboot for rk3399-evb board)

after compile there will be uboot.img, trust.img and rk3399_loader_v1.25.126.bin

**to use with eMMC do the following (RK3399)**

connect the board to host PC through USB cable, get the board into Maskroom, ( press and hold uboot button while inserting power) , then release the uboot button, connect the board through UART2 port with 1500000 baud rate ( for 3399 boards, 3288 board has 115200 buad rate)

	rkdeveloptool db rk3399_loader_v1.25.126.bin
	rkdeveloptool ul rk3399_loader_v1.25.126.bin
	rkdeveloptool wl 0x4000 uboot.img
	rkdeveloptool wl 0x6000 trust.img
	rkdeveloptool rd

board will boot, but it doesnt have kernel, dtb file and rootfs, let us create that in the next step

**to use with SD card do the following (RK3399)**

create idbloader , with two variants

	tools/mkimage -n rk3399 -T rksd -d ..//rkbin/bin/rk33/rk3399_ddr_800MHz_v1.25.bin idbloader.img
	cat rk3399_loader_v1.25.126.bin >> idbloader.img

or

	tools/mkimage -n rk3399 -T rksd -d tpl/u-boot-tpl.bin idbloader.img
	cat spl/u-boot-spl.bin >> idbloader.img

then

clean the SD card, below command will delete all partitions in the SD card and will fill with zero

	sudo dd if=/dev/zero of=/dev/sdx bs=8192 
then 
	sudo fdisk /dev/sdx
	
Command (m for help): n
	
Partition type

   p   primary (0 primary, 0 extended, 3 free)
   
   e   extended (container for logical partitions)
   
Select (default p): p

Partition number (1-4, default 1): 1

First sector (2048-61951999, default 2048): 100000

Last sector, +/-sectors or +/-size{K,M,G,T,P} (100000-61951999, default 61951999): (press enter)

Created a new partition 1 of type 'Linux' and of size 29.5 GiB

Command (m for help):w

now, 
	sudo mkfs.ext4 /dev/sdx1
	dd if=idbloader.img of=sdb seek=64
	dd if=uboot.img of=sdb seek=16384
	dd if=trust.img of=sdb seek=24576

insert the SD card into the board and connect 12V power, board will boot without kernel, dtb files and rootfs

cd ..

**to compile kernel**

cd rk-kernel ( for RK3399)
	make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4   rockchip_linux_defconfig

	make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- menuconfig

then scroll down to device drivers, networking, and check required wifi, bluetooth, touch screen drivers are enabled

	make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4 Image

	make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4 dtbs

for RK3288

	make ARCH=arm CROSS_COMPILE=..//prebuilts/gcc/linux-x86/arm/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- rockchip_linux_defconfig

	make ARCH=arm CROSS_COMPILE=..//prebuilts/gcc/linux-x86/arm/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- -j4 zImage

	make ARCH=arm CROSS_COMPILE=..//prebuilts/gcc/linux-x86/arm/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- dtbs

it will create zImage ( arch/arm/boot) , and dtbs ( arch/arm/boot/dts for RK3288 )

and 

it will create Image ( arch/arm64/boot) , and dtbs ( arch/arm64/boot/dts/rockchip for RK33399 )

Now we need to create boot.img, 

	mkdir boot 
( then copy Image and dtb files to boot directory)

	cp arch/arm64/boot/Image boot/ , 
cp arch/arm64/boot/dts/rockchip/rk3399-evb.dtb boot/rk3399.dtb ( you need to use correct dtb file for your board)

	mkdir boot/extlinux
	nano boot/extlinux/extlinux.conf then copy below to extlinux.conf

	label rockchip-kernel-4.4
  		kernel /Image
  		fdt /rk3399.dtb
  	append earlycon=uart8250,mmio32,0xff1a0000 root=PARTUUID=B921B045-1D rootwait rootfstype=ext4 init=/sbin/init
  
save , then 

	genext2fs -b 32768 -B $((32*1024*1024/32768)) -d boot/ -i 8192 -U boot_rk3399.img

this will create a boot_rk3399.img

then , for eMMC , power on the board while holiding down uboot button ( bring into Maskroom mode)

	rkdeveloptool db ..//rk-uboot/rk3399_loader_v1.25.126.bin
	rkdeveloptool wl 0x8000 boot_rk3399.img
	rkdeveloptool rd

board will boot, it will find extlinux.conf, and will load dtb file and kernel,  BUT we still dont have rootfs, let us make it 







