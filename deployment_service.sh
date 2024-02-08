#!/bin/bash

# Path and name of the new file.
file_path="/etc/systemd/system/"
service_name="uctronics-display.service"
exe_path=$(pwd)/"display"

deploy_function_service() {
    echo Create a new service "$file_path""$service_name".

    # Create a new file and write content into it.
    cat <<EOF |sudo tee  "$file_path""$service_name" >/dev/null
[Unit]
Description=UCtronics Display
After=multi-user.target

[Service]
ExecStart=/bin/sh -c "'$exe_path'"

[Install]
WantedBy=multi-user.target
EOF

    # Check if the file has been successfully created and if the content has been written into it.
    if [ -e "$file_path" ]; then
        echo "New file created and text written successfully."
        return 0
    else
        echo "Failed to create new file or write text."
        return 1
    fi
}


install_service() {
    if [ -e "Makefile" ]; then
        make clean && make
        deploy_function_service
        # Check if the deployment was successful
        if [ $? -eq 0 ]; then
            echo "Reload the systemd daemon to load the new service unit configuration."
            sudo systemctl daemon-reload

            echo "Enable and start the service."
            sudo systemctl enable $service_name
            sudo systemctl start $service_name

            # Check if the service is active
            if sudo systemctl is-active --quiet $service_name; then
                echo "Service $service_name has been successfully enabled and started."
            else
                echo "Failed to start service $service_name."
            fi
        else
            echo "Function service deployment failed. Aborting."
        fi

    else
        echo "Please run the script in the repository folder"
        exit 0
    fi
}

BOOT_CONFIG="/boot/config.txt"

# Verify config file exists, BEGIN
if [ ! -e $BOOT_CONFIG ]; then
    echo "Creating ${BOOT_CONFIG}..."
    sudo bash -c 'touch '${BOOT_CONFIG}
    if [ $? -ne 0 ]; then
        echo "Failed to create file. Exiting."
        exit 1
    else
        echo "Successfully created file."
    fi
fi
# Verify config file exists, END

if [ `grep -c "dtoverlay=gpio-shutdown,gpio_pin=4,active_low=1,gpio_pull=up" $BOOT_CONFIG` -lt '1' ];then
    sudo bash -c 'echo dtoverlay=gpio-shutdown,gpio_pin=4,active_low=1,gpio_pull=up >> /boot/config.txt'
fi
if [ `grep -c "dtparam=i2c_arm=on,i2c_arm_baudrate=400000" $BOOT_CONFIG` -lt '1' ];then
    if [ `grep -c "#dtparam=i2c_arm=on" $BOOT_CONFIG` -ne '0' ];then
        sudo sed -i "s/\(^#dtparam=i2c_arm=on\)/dtparam=i2c_arm=on,i2c_arm_baudrate=400000/" /boot/config.txt
    elif [ `grep -c "dtparam=i2c_arm=on" $BOOT_CONFIG` -ne '0' ]; then
        sudo sed -i "s/\(^dtparam=i2c_arm=on\)/dtparam=i2c_arm=on,i2c_arm_baudrate=400000/" /boot/config.txt
    else 
        sudo bash -c 'echo dtparam=i2c_arm=on,i2c_arm_baudrate=400000 >> /boot/config.txt'
    fi
fi

# Deploy the function service
if [ -e "$file_path""$service_name" ]; then
    echo "Service already exists. Do you want to overwrite? y/n"
    read -r answer
    if [ "$answer" = 'y' ] || [ "$answer" = 'Y' ]; then
        install_service
    else 
        exit 0
    fi
else
    echo "Installing UCtronics display service"
    install_service
fi
