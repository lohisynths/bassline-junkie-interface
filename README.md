
1. Download latest STM32CubeIDE from following web page
```
https://www.st.com/en/development-tools/stm32cubeide.html
```

2. Unzip and run downloaded binary (versions may differ)
```
unzip en.st-stm32cubeide_1.0.2_3566_20190716_0927_amd64.deb_bundle.sh.zip 
chmod +x st-stm32cubeide_1.0.2_3566_20190716_0927_amd64.deb_bundle.sh 
sudo ./st-stm32cubeide_1.0.2_3566_20190716_0927_amd64.deb_bundle.sh
```

2. Install toolchain- arm gcc 8
```
cd ~/
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz
tar -xjf gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz
cd gcc-arm-none-eabi-8-2019-q3-update/bin/

add following line to the bottom of the ~/.bashrc file
export PATH="$PATH:$HOME/gcc-arm-none-eabi-8-2019-q3-update/bin"

```

3. Clone, checkout and clean mbed-os repository
```
cd git
git clone --recurse-submodules https://github.com/ARMmbed/mbed-os.git
cd ~/git/mbed-os
git checkout mbed-os-5.9 # later versions causes pips segfault on ubuntu 18.04 LTS
```

```
git clean -xfd
git submodule foreach --recursive git clean -xfd
git reset --hard
git submodule foreach --recursive git reset --hard
git submodule update --init --recursive
```

4. Install requirements
```
cd ~/git/mbed-os
pip install -r requirements.txt 
```

5. Build mbed-os

```

cd ~/git/mbed-os
python tools/build.py --mcu NUCLEO_F411RE --tool GCC_ARM -j8
```

6. Clone bassline junkie interface repo
```
cd ~/git
git clone --recurse-submodules https://github.com/lohisynths/bassline-junkie-interface.git
```

7. Build bassline junkie interface project
```
cd ~/git/bassline-junkie-interface
./build.sh

```
