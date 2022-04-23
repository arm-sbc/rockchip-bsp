# rockchip-bsp
Rockchip vendor BSP, kernel version 4.4 and uboot version 2017

to compile install follwing packages on Ubuntu/Debian host machine

sudo apt install flex bison gcc-arm-linux-gnueabihf gcc-aarch64-linux-gnu build-essential git debootstrap u-boot-tools device-tree-compiler libssl-dev libncurses-dev binfmt-support qemu-user-static pkg-config automake sudo ibudev-dev libusb-1.0-0-dev dh-autoreconf 

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


