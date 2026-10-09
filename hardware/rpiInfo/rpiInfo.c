#include "rpiInfo.h"
#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include "st7735.h"
#include <stdlib.h>
#include <sys/time.h>
#include <mntent.h>

/*
* Get the IP address of wlan0 or eth0
*/

char* get_ip_address(void)
{
    int fd;
    struct ifreq ifr;
    int symbol=0;
    if (IPADDRESS_TYPE == ETH0_ADDRESS)
    {
      fd = socket(AF_INET, SOCK_DGRAM, 0);
      /* I want to get an IPv4 IP address */
      ifr.ifr_addr.sa_family = AF_INET;
      /* I want IP address attached to "eth0" */
      strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
      symbol=ioctl(fd, SIOCGIFADDR, &ifr);
      close(fd);
      if(symbol==0)
      {
        return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
      }
      else
      {
        char* buffer="xxx.xxx.xxx.xxx";
        return buffer;
      }
    }
    else if (IPADDRESS_TYPE == WLAN0_ADDRESS)
    {
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        /* I want to get an IPv4 IP address */
        ifr.ifr_addr.sa_family = AF_INET;
        /* I want IP address attached to "wlan0" */
        strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
        symbol=ioctl(fd, SIOCGIFADDR, &ifr);
        close(fd);    
        if(symbol==0)
        {
          return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);   
        }
        else
        {
          char* buffer="xxx.xxx.xxx.xxx";
          return buffer;
        }
    }
    else
    {
      char* buffer="xxx.xxx.xxx.xxx";
      return buffer;
    }
}

char* get_ip_address_new(void)
{
    int fd;
    struct ifreq ifr;
    int symbol=0;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    symbol=ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    if(symbol==0)
    {
      return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    }
    else
    {
      fd = socket(AF_INET, SOCK_DGRAM, 0);
      /* I want to get an IPv4 IP address */
      ifr.ifr_addr.sa_family = AF_INET;
      /* I want IP address attached to "wlan0" */
      strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
      symbol=ioctl(fd, SIOCGIFADDR, &ifr);
      close(fd);    
      if(symbol==0)
      {
        return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);   
      }
      else
      {
        char* buffer="xxx.xxx.xxx.xxx";
        return buffer;
      }
    }
}



/*
* get ram memory
*/
void get_cpu_memory(float *Totalram, float *freeram)
{
    unsigned long mem_total = 0;
    unsigned long mem_available = 0;
    unsigned long mem_free = 0;
    char buffer[128];
    char key[64];
    unsigned long value = 0;

    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        return;
    }
    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (sscanf(buffer, "%63s %lu", key, &value) == 2)
        {
            if (strcmp(key, "MemTotal:") == 0)
            {
                mem_total = value;
            }
            else if (strcmp(key, "MemAvailable:") == 0)
            {
                mem_available = value;
            }
            else if (strcmp(key, "MemFree:") == 0)
            {
                mem_free = value;
            }
        }
    }
    fclose(fp);

    unsigned long effective_free = (mem_available > 0) ? mem_available : mem_free;
    *Totalram = mem_total / 1000.0 / 1000.0;
    *freeram = effective_free / 1000.0 / 1000.0;
}

/*
* get sd memory
*/
void get_sd_memory(uint32_t *MemSize, uint32_t *freesize)
{
    struct statfs diskInfo;
    statfs("/",&diskInfo);
    unsigned long long blocksize = diskInfo.f_bsize;// The number of bytes per block
    unsigned long long totalsize = blocksize*diskInfo.f_blocks;//Total number of bytes	
    *MemSize=(unsigned int)(totalsize>>30);


    unsigned long long size = blocksize*diskInfo.f_bfree; //Now let's figure out how much space we have left
    *freesize=size>>30;
    *freesize=*MemSize-*freesize;
}


/*
* get hard disk memory
*/
uint8_t get_hard_disk_memory(uint16_t *diskMemSize, uint16_t *useMemSize)
{
    *diskMemSize = 0;
    *useMemSize = 0;

    FILE *fp = setmntent("/proc/mounts", "r");
    if (!fp)
    {
        return 0;
    }

    struct mntent *mnt;
    unsigned long total_gb = 0;
    unsigned long used_gb = 0;

    while ((mnt = getmntent(fp)) != NULL)
    {
        // Support NVMe (/dev/nvme*), SATA/USB (/dev/sd*), and virtio (/dev/vd*)
        if ((strncmp(mnt->mnt_fsname, "/dev/sd", 7) == 0 ||
             strncmp(mnt->mnt_fsname, "/dev/nvme", 9) == 0 ||
             strncmp(mnt->mnt_fsname, "/dev/vd", 7) == 0) &&
            strcmp(mnt->mnt_dir, "/") != 0)
        {
            struct statfs s;
            if (statfs(mnt->mnt_dir, &s) == 0)
            {
                unsigned long long blocksize = s.f_bsize;
                unsigned long long total = (blocksize * s.f_blocks) >> 30;
                unsigned long long free = (blocksize * s.f_bfree) >> 30;
                total_gb += total;
                used_gb += (total - free);
            }
        }
    }
    endmntent(fp);

    *diskMemSize = (uint16_t)total_gb;
    *useMemSize  = (uint16_t)used_gb;
    return 0;
}

/*
* get temperature
*/

uint8_t get_temperature(void)
{
    FILE *fd;
    unsigned int temp;
    char buff[10] = {0};
    fd = fopen("/sys/class/thermal/thermal_zone0/temp","r");
    if (fd == NULL)
    {
        return 0;
    }
    fgets(buff,sizeof(buff),fd);
    sscanf(buff, "%d", &temp);
    fclose(fd);
    return TEMPERATURE_TYPE == FAHRENHEIT ? temp/1000*1.8+32 : temp/1000;    
}

/*
* Get cpu usage
*/
uint8_t get_cpu_message(void)
{
    static unsigned long long prev_user = 0, prev_nice = 0, prev_sys = 0, prev_idle = 0;
    static unsigned long long prev_iowait = 0, prev_irq = 0, prev_softirq = 0, prev_steal = 0;
    static int initialized = 0;

    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
    {
        return 0;
    }

    char line[128];
    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    unsigned long long user = 0, nice = 0, sys = 0, idle = 0;
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0;
    int n = sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                   &user, &nice, &sys, &idle, &iowait, &irq, &softirq, &steal);
    if (n < 4)
    {
        return 0;
    }

    if (!initialized)
    {
        prev_user = user;
        prev_nice = nice;
        prev_sys = sys;
        prev_idle = idle;
        prev_iowait = iowait;
        prev_irq = irq;
        prev_softirq = softirq;
        prev_steal = steal;
        initialized = 1;
        return 0;
    }

    unsigned long long prev_idle_all = prev_idle + prev_iowait;
    unsigned long long idle_all = idle + iowait;

    unsigned long long prev_busy = prev_user + prev_nice + prev_sys + prev_irq + prev_softirq + prev_steal;
    unsigned long long busy = user + nice + sys + irq + softirq + steal;

    unsigned long long prev_total = prev_idle_all + prev_busy;
    unsigned long long total = idle_all + busy;

    prev_user = user;
    prev_nice = nice;
    prev_sys = sys;
    prev_idle = idle;
    prev_iowait = iowait;
    prev_irq = irq;
    prev_softirq = softirq;
    prev_steal = steal;

    if (total <= prev_total)
    {
        return 0;
    }

    unsigned long long total_diff = total - prev_total;
    unsigned long long idle_diff = idle_all - prev_idle_all;

    double cpu_usage = ((double)(total_diff - idle_diff) * 100.0) / (double)total_diff;
    if (cpu_usage < 0.0) cpu_usage = 0.0;
    if (cpu_usage > 100.0) cpu_usage = 100.0;

    return (uint8_t)(cpu_usage + 0.5);
}

/*
* Format network rate with auto-shifting units (bps, Kbps, Mbps, Gbps)
*/
static void format_network_rate(double bps, char *out, size_t max_len)
{
    if (bps >= 1000000000.0)
    {
        snprintf(out, max_len, "%.2f Gbps", bps / 1000000000.0);
    }
    else if (bps >= 1000000.0)
    {
        snprintf(out, max_len, "%.2f Mbps", bps / 1000000.0);
    }
    else if (bps >= 1000.0)
    {
        snprintf(out, max_len, "%.1f Kbps", bps / 1000.0);
    }
    else
    {
        snprintf(out, max_len, "%.0f bps", bps);
    }
}

/*
* Get physical network download and upload throughput
*/
void get_network_speed(char *rx_str, char *tx_str)
{
    static unsigned long long prev_rx = 0;
    static unsigned long long prev_tx = 0;
    static struct timeval prev_time = {0, 0};
    static int net_initialized = 0;

    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp)
    {
        snprintf(rx_str, 16, "0.0 Kbps");
        snprintf(tx_str, 16, "0.0 Kbps");
        return;
    }

    char line[256];
    unsigned long long total_rx = 0;
    unsigned long long total_tx = 0;

    // Skip first two header lines
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return; }
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return; }

    while (fgets(line, sizeof(line), fp))
    {
        char *colon = strchr(line, ':');
        if (!colon) continue;

        *colon = '\0';
        char *iface = line;
        while (*iface == ' ') iface++;

        // Only measure physical interfaces: eth*, en*, wlan*, wl*
        if (strncmp(iface, "eth", 3) == 0 ||
            strncmp(iface, "en", 2) == 0 ||
            strncmp(iface, "wlan", 4) == 0 ||
            strncmp(iface, "wl", 2) == 0)
        {
            unsigned long long rx = 0, tx = 0;
            unsigned long long dummy;
            if (sscanf(colon + 1, "%llu %llu %llu %llu %llu %llu %llu %llu %llu",
                       &rx, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &tx) >= 9)
            {
                total_rx += rx;
                total_tx += tx;
            }
        }
    }
    fclose(fp);

    struct timeval now;
    gettimeofday(&now, NULL);

    if (!net_initialized)
    {
        prev_rx = total_rx;
        prev_tx = total_tx;
        prev_time = now;
        net_initialized = 1;
        snprintf(rx_str, 16, "0.0 Kbps");
        snprintf(tx_str, 16, "0.0 Kbps");
        return;
    }

    double dt = (now.tv_sec - prev_time.tv_sec) + (now.tv_usec - prev_time.tv_usec) / 1000000.0;
    if (dt <= 0.05)
    {
        return;
    }

    double rx_bps = ((double)(total_rx >= prev_rx ? (total_rx - prev_rx) : 0) * 8.0) / dt;
    double tx_bps = ((double)(total_tx >= prev_tx ? (total_tx - prev_tx) : 0) * 8.0) / dt;

    format_network_rate(rx_bps, rx_str, 16);
    format_network_rate(tx_bps, tx_str, 16);

    prev_rx = total_rx;
    prev_tx = total_tx;
    prev_time = now;
}