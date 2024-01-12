# SKU_RM0004
The project supports running on RaspberryPi, Ubuntu, [HomeAssistant](https://github.com/UCTRONICS/UCTRONICS_RM0004_HA),You can also use Python to call compiled DLLs on these platforms.
# RaspberryPi

## Deployment service
> Run in the SKU_RM0004 folder
```bash
./deployment_service.sh   
```
>You can also manually deploy according to the following steps
## Turn on i2c and set the speed
**Add the following to the /boot/config.txt file**
```bash
dtparam=i2c_arm=on,i2c_arm_baudrate=400000
```

## Turn on the button to control the shutdown function
**Add the following to the /boot/config.txt file**
```bash
dtoverlay=gpio-shutdown,gpio_pin=4,active_low=1,gpio_pull=up
```

**reboot your system**
```bash
sudo reboot
```
**Wait for the system to restart**

<br>

##  Pre-Requisites
### Ubuntu
Install necessary dependencies to compile the project:
```bash
sudo apt install make gcc
```
### Home Assistant
```bash
$ apk add make gcc musl-dev i2c-tools i2c-tools-dev linux-headers
```

##  Building
###  Clone SKU_RM0004 library
```bash
git clone https://github.com/UCTRONICS/SKU_RM0004.git
```
### Compile 
```bash
cd SKU_RM0004
make
```
### Run 
```
./display
```

## Installing

### Ubuntu
1. Create file `/etc/systemd/system/rpi-display.service`
    ```
    [Unit]
    After=network.target

    [Service]
    ExecStart=/home/user/git/SKU_RM0004/display

    [Install]
    WantedBy=default.target
    ```
1. Create folder (if it doesn't exist):
    ```
    $ mkdir /etc/systemd/system/default.target.wants
    ```
1. Create soft link to service file:
    ```
    $ ln -s /etc/systemd/system/rpi-display.service /etc/systemd/system/default.target.wants/default.target.wants
    ```

### Others
## Add automatic start script
**Open the rc.local file**
```bash
sudo nano /etc/rc.local
```
**Add command to the rc.local file**
```bash
cd /home/pi/SKU_RM0004
make clean 
make 
./display &
```
**reboot your system**






