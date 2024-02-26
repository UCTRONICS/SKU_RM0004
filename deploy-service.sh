#!/bin/bash

################################################################
SCRIPT_FULLPATH="$(realpath $0)"
SCRIPT_DIR="$(dirname "${SCRIPT_FULLPATH}")"
################################################################
SVC_DIR="/etc/systemd/system/"
SVC_NAME="uctronics-display"
EXE_PATH="${SCRIPT_DIR}/display"
################################################################
promptAnswer="c"
################################################################

## Function: Display custom error messages ##
show-error() {
    echo "" ; echo "Error: $1" ; echo ""
    exit 1 
}

## Function: Prompt user for Y/N answer ##
get-prompt() {
    while true
    do
        read -N1 -t15 -p "$1 (y/N): "
        if [ $? -gt 128 ]; then
            printf "\nTimed out waiting for a user response.\n"
            promptAnswer="c"
            break
        fi
        case $REPLY in
            [yY]) printf "\n"; promptAnswer="${REPLY}" ; break ;;
            [nN]) printf "\n"; promptAnswer="${REPLY}" ; break ;;
            *) printf "\nPlease type y or n to continue.\n" ;;
        esac
    done
}

## Function: Get the Linux OS distribution ##
get-os() {
    local unameResult=$(uname | tr "[:upper:]" "[:lower:]")

    if [ "$unameResult" == "linux" ]; then
        # Determine specific Linux distribution
        if [ -f /etc/lsb-release -o -d /etc/lsb-release.d ]; then
            # Use LSB method if available
            export OS_DISTRO=$(lsb_release --id | awk -F"\t" '{print tolower($2)}')
            export OS_RELEASE=$(lsb_release --release | awk -F"\t" '{print $2}' | sed 's/\.//g')
        else
            # Otherwise use OS-release file 
            export OS_DISTRO=$(awk -F= 'BEGIN{IGNORECASE=1}/^name/{print tolower($2)}' /etc/os-release | sed 's/\"//g')
            export OS_RELEASE=$(awk -F= 'BEGIN{IGNORECASE=1}/^version_id/{print $2}' /etc/os-release | sed 's/[\"\.]//g')
        fi
    else
        show-error "Linux OS not detected. Terminating installation."
    fi
}

## Function: Create the boot.config file
new-config() {
    if [ "$OS_DISTRO" == "ubuntu" ] && [ "$OS_RELEASE" -ge 2204 ]; then
        # TESTING: Uncomment after testing 
        #export BOOT_CONFIG="/boot/firmware/config.txt"

        # TESTING: Remove after testing
        export BOOT_CONFIG="~/config.txt"
    else
        export BOOT_CONFIG="/boot/config.txt"
    fi

    if ! [ -f "${BOOT_CONFIG}" ]; then 
        sudo bash -c 'touch '${BOOT_CONFIG} || show-error "Failed to create file. Exiting."
    fi
}

## Function: Add entries to the appropriate config file
set-config() {
    local dtParam='dtparam=i2c_arm=on,i2c_arm_baudrate=400000'
    local dtOverlay='dtoverlay=gpio-shutdown,gpio_pin=4,active_low=1,gpio_pull=up'

    # Write dtParam entry to config.txt
    if [ `grep -c "${dtParam}" $BOOT_CONFIG` -eq '0' ]; then
        if [ `grep -c "#dtparam=i2c_arm=on" $BOOT_CONFIG` -ne '0' ]; then
            sudo sed -i "s/\(^#dtparam=i2c_arm=on\)/${dtParam}/" ${BOOT_CONFIG}
        elif [ `grep -c "dtparam=i2c_arm=on" $BOOT_CONFIG` -ne '0' ]; then
            sudo sed -i "s/\(^dtparam=i2c_arm=on\)/${dtParam}/" ${BOOT_CONFIG}
        else 
            sudo bash -c 'echo '${dtParam}' >> '${BOOT_CONFIG}
        fi
    fi

    # Write dtOverlay entry to config.txt
    if [ "$OS_DISTRO" == "ubuntu" ] && [ "$OS_RELEASE" -ge 2204 ]; then
        # /boot/firmware/config.txt -- insert after dtParam entry
        if [ `grep -c "${dtOverlay}" $BOOT_CONFIG` -eq '0' ]; then
            sudo sed -i "/\(^${dtParam}\)/a${dtOverlay}" ${BOOT_CONFIG}
        fi
    else
        # /boot/config.txt -- write entry anywhere
        if [ `grep -c "${dtOverlay}" $BOOT_CONFIG` -eq '0' ]; then
            sudo bash -c 'echo '${dtOverlay}' >> '${BOOT_CONFIG}
        fi
    fi
}

## Function: Create the service unit file ##
new-service() {
    cat <<EOF | sudo tee "${SVC_DIR}${SVC_NAME}.service" &> /dev/null
[Unit]
Description=UCtronics Display
After=multi-user.target

[Service]
Type=simple
ExecStart=/bin/sh -c ${EXE_PATH}

[Install]
WantedBy=multi-user.target
EOF
    # Check if unit file was successfully created
    if ! [ -f "${SVC_DIR}${SVC_NAME}.service" ]; then
        show-error "Failed to create the service unit file."
    fi
}

## Function: Stop and remove the service ##
remove-service() {
    if systemctl is-active --quiet ${SVC_NAME}.service; then
        systemctl --quiet stop ${SVC_NAME}.service &> /dev/null || show-error "Unable to stop the service."
        systemctl --quiet disable ${SVC_NAME}.service &> /dev/null || show-error "Unable to disable the service."
    fi

    if [ -f "${SVC_DIR}${SVC_NAME}.service" ]; then
        rm ${SVC_DIR}${SVC_NAME}.service &> /dev/null || show-error "Failed to remove service unit file."
    fi
}

## Function: Load and start the service ##
set-service() {
    if ! [ -f "${EXE_PATH}" ]; then 
        printf "Compiling display application: "
        
        if [ -e "./Makefile" ]; then
            make clean && make || show-error "Failed to compile display application. Verify 'make' libraries are installed."
        else
            show-error "No 'Makefile' file found. Exiting"
        fi

        printf "DONE\n"
    fi

    # Reload and reset systemd
    printf "Reloading systemd daemon: "
    systemctl --quiet daemon-reload &> /dev/null || show-error "Unable to reload systemd."
    printf "DONE\n"

    # Enable the service
    printf "Enabling service %s: " ${SVC_NAME}
    systemctl --quiet enable ${SVC_NAME} &> /dev/null || show-error "Unable to enable the service."
    printf "DONE\n"

    # Start the service
    printf "Starting service %s: " ${SVC_NAME}
    systemctl --quiet start ${SVC_NAME} &> /dev/null || show-error "Unable to start the service."
    printf "DONE\n"

}

# Detect previous service installation #
printf "\nFinding previous %s installation: " ${SVC_NAME}
if [ -f "${SVC_DIR}${SVC_NAME}.service" ]; then
    # Overwrite instance of service
    printf "FOUND\n"
    get-prompt "Do you want to overwrite?"
    if [ ${promptAnswer} == 'y' ] || [ ${promptAnswer} == 'Y' ]; then
        remove-service
    else
        exit 0
    fi
else
    # Install new instance of service
    printf "NOT FOUND\n"
    get-prompt "Proceed with new service installation?"
    if [ ${promptAnswer} != 'y' ] && [ ${promptAnswer} != 'Y' ]; then
        exit 0
    fi
fi

# Install a new instance of service #
printf "\nScanning for a Linux operating system: " ; get-os ; printf "%s %s\n" ${OS_DISTRO} ${OS_RELEASE}
printf "Creating the config.txt file: " ; new-config ; printf "%s\n" ${BOOT_CONFIG}

# TESTING: Remove after testing
sudo chown moebius:moebius ${BOOT_CONFIG} 

printf "Writing i2c entries to config file..." ; set-config ; printf "DONE\n\n"

printf "Creating a new service unit: " ; new-service ; printf "%s%s.service\n" ${SVC_DIR} ${SVC_NAME}
printf "Loading and starting the service...\n" ; set-service

if systemctl is-active --quiet $SVC_NAME; then
    echo "Service ${SVC_NAME} successfully enabled and started!"
    exit 0
else
    show-error "Failed to complete service deployment. Aborting."
fi
