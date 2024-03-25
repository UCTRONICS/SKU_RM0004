# SKU_RM0004
The project supports running on RaspberryPi, Ubuntu, [HomeAssistant](https://github.com/UCTRONICS/UCTRONICS_RM0004_HA),You can also use Python to call compiled DLLs on these platforms.
# RaspberryPi

## Deployment service
>  Clone SKU_RM0004 library 
```bash
git clone https://github.com/UCTRONICS/SKU_RM0004.git
```
> Compile 
```bash
cd SKU_RM0004
make
```
## Add automatic start script
```bash
./deployment_service.sh   
```
**reboot your system**
```bash
sudo reboot
```
## How to uninstall the uctronics-display.service

```bash
sudo systemctl disable uctronics-display.service
sudo rm /etc/systemd/system/uctronics-display.service
sudo systemctl daemon-reload
```
## How to use NVMe 
https://github.com/UCTRONICS/SKU_RM0004/blob/main/data/NVMe_User_Guide.md





