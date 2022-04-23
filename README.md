# rockchip-bsp
Rockchip vendor BSP, kernel version 4.4 and uboot version 2017

to compile install follwing packages on Ubuntu/Debian host machine

sudo apt install flex bison gcc-arm-linux-gnueabihf gcc-aarch64-linux-gnu build-essential git debootstrap u-boot-tools device-tree-compiler libssl-dev libncurses-dev binfmt-support qemu-user-static pkg-config automake sudo ibudev-dev libusb-1.0-0-dev dh-autoreconf gawk python-is-python3 python3 

git clone https://github.com/arm-sbc/rockchip-bsp.git

cd rockchip-bsp

git clone https://github.com/arm-sbc/rk-kernel.git

git clone https://github.com/arm-sbc/rk-uboot.git

git clone https://github.com/arm-sbc/rkbin.git

git clone https://github.com/arm-sbc/rkdeveloptool.git

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

cd rk-uboot

rk-uboot$ ls configs | grep 3399 or 3288 ( check correct defconfig)

rk-uboot$ ./build.sh evb-rk3399 ( this will create uboot for rk3399-evb board)

after compile there will be uboot.img, trust.img and rk3399_loader_v1.25.126.bin

to use with eMMC do the following (RK3399)
---------------------------------

connect the baord to host PC through USB cable, get teh baord into Maskroom, ( press and hold uboot button while inserting power) , then release the uboot button, connect the board through UART2 port with 1500000 baud rate ( for 3399 boards, 3288 board has 115200 buad rtae)

rkdeveloptool db rk3399_loader_v1.25.126.bin

rkdeveloptool ul rk3399_loader_v1.25.126.bin

rkdeveloptool wl 0x4000 uboot.img

rkdeveloptool wl 0x6000 trust.img

rkdeveloptool rd

board will boot but it doesnt ahve kerenel, dtb file and rootfs, let us create that 

to use with SD card do the following (RK3399)
------------------------------------

create idblaoder , with two varinats

tools/mkimage -n rk3399 -T rksd -d ..//rkbin/bin/rk33/rk3399_ddr_800MHz_v1.25.bin idbloader.img

cat rk3399_loader_v1.25.126.bin >> idbloader.img

or

tools/mkimage -n rk3399 -T rksd -d tpl/u-boot-tpl.bin idbloader.img

cat spl/u-boot-spl.bin >> idbloader.img

then

dd if=idbloader.img of=sdb seek=64

dd if=uboot.img of=sdb seek=16384

dd if=trust.img of=sdb seek=24576

cd ..

cd rk-kernel

rk-kernel$ 

for RK 3399

make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4   rockchip_linux_defconfig

make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4 Image

make ARCH=arm64 CROSS_COMPILE=..//prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -j4 dtbs

for RK33288



it will create zImage/Image , and dtbs 


