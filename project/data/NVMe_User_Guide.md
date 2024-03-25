# How to burn imge to NVMe and boot from NVMe
### Hardware connection 
<div align=left><img width="500" height="400" src=hardware.png/></div>

### How to burn image to NVMe disk?
We provide two ways to burning image to the NVMe disk.
- Method 1: Using a USB disk reader/writer
Uisng this method, you need a disk reader such as the below image shown. Just put the NVME disk to your USB disk reader and connect the  USB disk reader to your PC. Then using the RPI imager to install Raspberry Pi OS.

  <div align=left><img width="500" height="200" src=disk_reader.png/></div>

- Method 2: Using the RPI directly 
       Please refer to Enable NVMe chapter to enable the NVMe driver firstly.  
  - 2.1. Check if the SSD controller exist.
    <div align=left><img width="500" height="80" src=check_disk.png/></div>
  -  2.2. Using the rpi-imager burning image to the NVMe disk
   <div align=left><img width="500" height="500" src=imager.png/></div>

*Note: Please ensure the image has been burned to the MVNe before doing the following operations.*
- Updating your Pi
    ```bash
    sudo apt-get update && sudo apt-get upgrade
    ```
- Enabling NVMe
1. Edit the EEPROM on the Raspberry Pi 5. **sudo rpi-eeprom-config --edit**
2. Change the **BOOT_ORDER** line to the following:**BOOT_ORDER=0xf416**
3. Add **PCIE_PROBE** to enumerate over the PCIe bus: **PCIE_PROBE=1**
4. Press **Ctrl-O**, then enter, to write the change to the file.
5. Press **Ctrl-X** to exit nano (the editor).
6. Reboot your Pi with: **sudo reboot**