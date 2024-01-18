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

##  Clone SKU_RM0004 library 
```bash
git clone https://github.com/UCTRONICS/SKU_RM0004.git
```
## Compile 
```bash
cd SKU_RM0004
make
```
## Run 
```
./display
```

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

# Install Instructions for SKU_RM0004 on Ubuntu Server 22.04

The following steps would be the adapted instructions for your environment (Ubuntu Server 22.04 with user `serveradmin`, adjust for your own environment):

1. Enable i2c and set the baud rate by editing the `/boot/firmware/config.txt` file. You can do this using a text editor such as `nano`. You will need to use `sudo` to edit this file because it requires root permissions:

    ```bash
    sudo nano /boot/firmware/config.txt
    ```

    Then, add the following lines to the file:

    ```bash
    dtparam=i2c_arm=on
    dtparam=i2c_arm_baudrate=400000
    dtoverlay=gpio-shutdown
    dtoverlay=gpio_pin=4
    dtoverlay=active_low=1
    dtoverlay=gpio_pull=up
    ```

    Save the file and exit the editor.

2. Install the necessary i2c packages with the following command:

    ```bash
    sudo apt install -y python3-pip python3-dev python3-pil python3-setuptools python3-rpi.gpio i2c-tools
    ```

3. Add your user to the i2c group with the following command:

    ```bash
    sudo usermod -aG i2c serveradmin
    ```

4. Clone the SKU_RM0004 library and compile the display driver:

    ```bash
    git clone https://github.com/UCTRONICS/SKU_RM0004.git
    cd SKU_RM0004
    make
    ```

5. To start the display driver automatically at boot, you can either use the systemd service or the `/etc/rc.local` file.

    - To use the systemd service, you will need to create a systemd service. Open a new service file in a text editor:

        ```bash
        sudo nano /etc/systemd/system/sku_rm0004.service
        ```

        Then, add the following lines to the file:

        ```bash
        [Unit]
        Description=Start SKU_RM0004 display
        After=network.target

        [Service]
        ExecStartPre=/usr/bin/make -C /home/serveradmin/SKU_RM0004 clean
        ExecStartPre=/usr/bin/make -C /home/serveradmin/SKU_RM0004
        ExecStart=/home/serveradmin/SKU_RM0004/display
        WorkingDirectory=/home/serveradmin/SKU_RM0004
        User=serveradmin
        Group=serveradmin
        Restart=always

        [Install]
        WantedBy=multi-user.target
        ```

        Save and close the file.

        Then, enable the service with the following command:

        ```bash
        sudo systemctl enable sku_rm0004
        sudo systemctl start sku_rm0004
        ```

    - To use the rc.local file method, you will need to add commands to the `/etc/rc.local` file. Note that this file may not exist or be executable by default on Ubuntu 22.04, so you may need to create it and make it executable:

        ```bash
        sudo touch /etc/rc.local
        sudo chmod +x /etc/rc.local
        sudo nano /etc/rc.local
        ```

        Then, add the following commands to the file:

        ```bash
        #!/bin/bash
        cd /home/serveradmin/SKU_RM0004
        make clean 
        make 
        ./display &
        ```

        Save the file and exit the editor.

        Remember to reboot your system for changes to take effect:

        ```bash
        sudo reboot
        ```





