# Blink
## overview

LEDを点滅するアプリケーション

---
## Host
### Build
```
mkdir build
cd build
cmake ..
make -j4
```
### Execution
```
./blink
```
---
## Target
### Build
```
mkdir target_build
cd target_build
cmake -DCMAKE_TOOLCHAIN_FILE=../raspi.cmake ..
make -j4
```
### Execution
```
scp blink pi@raspberrypi.local:/home/pi/
ssh pi@raspberrypi.local
sudo ./blink

# もしinitInitialise: Can't lock /var/run/pigpio.pidが出たら
sudo killall -9 pigpiod
sudo rm /var/run/pigpio.pid
```