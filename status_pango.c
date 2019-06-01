/*
 * A statusbar using pango and powerline symbols
 *
 * Copyright (c) 2019 Gaurav Kumar Yadav <gaurav712@protonmail.com>
 * for license and copyright information, see the LICENSE file that came with the source
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* function to copy num elements from n of a string to another */
static void strNumFromNCpy(char *dest, char *src, short n, short num);

int main(void){

    time_t current_time;
    FILE *up, *down;
    unsigned long up1, up2, down1, down2;
    char time_str[10], date_str[11], bat_stat, wlan_stat;
    short battery_capacity;
    /* Color variables */
    char bat_color[8], time_color[8], date_color[8];

    /* BATTERY STATUS */
    up = fopen("/sys/class/power_supply/BAT1/status", "r");
    fscanf(up, "%c", &bat_stat);
    fclose(up);

    /* BATTERY PERCENTAGE */
    up = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    fscanf(up, "%hd", &battery_capacity);
    fclose(up);

    /* When Discharging */
    if(bat_stat == 'D') {

        /* CHANGE THIS IF YOU WANT DIFFERENT BATTERY COLOR WHEN DISCHARGING */
        strcpy(bat_color, "#dc322f");
        if(battery_capacity <= 20)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf57a %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 40)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf57c %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 60)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf57e %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 80)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf580 %d%%</span>", bat_color, bat_color, battery_capacity);
        else
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf581 %d%%</span>", bat_color, bat_color, battery_capacity);
    }

    /* When Charging */
    else if(bat_stat == 'C') {

        /* CHANGE THIS IF YOU WANT DIFFERENT BATTERY COLOR WHEN CHARGING */
        strcpy(bat_color, "#859900");
        if(battery_capacity <= 20)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf585 %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 40)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf587 %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 60)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf588 %d%%</span>", bat_color, bat_color, battery_capacity);
        else if(battery_capacity <= 80)
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf589 %d%%</span>", bat_color, bat_color, battery_capacity);
        else
            printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf58a %d%%</span>", bat_color, bat_color, battery_capacity);
    }

    /* When Full */
    else {

        /* CHANGE THIS IF YOU WANT DIFFERENT BATTERY COLOR WHEN FULL */
        strcpy(bat_color, "#93a1a1");
        printf("<span color=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf584 %d%%</span>", bat_color, bat_color, battery_capacity);
    }

    /* DATE */
    /* CHANGE THIS IF YOU WANT DIFFERENT COLOR FOR DATE */
    strcpy(date_color, "#6c71c4");
    current_time = time(NULL);
    strNumFromNCpy(date_str, ctime(&current_time), 0, 10);
    printf("<span color=\'%s\' background=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf5ec %s</span>", date_color, bat_color, date_color, date_str);

    /* TIME */
    /* CHANGE THIS IF YOU WANT DIFFERENT COLOR FOR TIME */
    strcpy(time_color, "#2aa198");
    strNumFromNCpy(time_str, ctime(&current_time), 11, 8);
    printf("<span color=\'%s\' background=\'%s\'>\ue0b2</span><span background=\'%s\'> \uf64f %s</span>", time_color, date_color, time_color, time_str);

    /* RFKILL */
    up = fopen("/sys/class/rfkill/rfkill0/state", "r");
    fscanf(up, "%c", &wlan_stat);
    fclose(up);

    /* Wifi is on, let's check if it's connnected */
    if(wlan_stat == '1') {

        /* WIFI STATUS */
        up = fopen("/sys/class/net/wlp3s0/operstate", "r");
        fscanf(up, "%c", &wlan_stat);
        fclose(up);

        /* Wifi is connected, let's calculate speed */
        if(wlan_stat == 'u') {

            /* UPLOAD AND DOWNLOAD SPEED */
            up = fopen("/sys/class/net/wlp3s0/statistics/tx_bytes", "r");
            down = fopen("/sys/class/net/wlp3s0/statistics/rx_bytes", "r");
            fscanf(up, "%ld", &up1);
            fscanf(down, "%ld", &down1);
            sleep(1);
            /* Moving the pointer again to the begining */
            rewind(up);
            rewind(down);
            fscanf(up, "%ld", &up2);
            fscanf(down, "%ld", &down2);
            fclose(up);
            fclose(down);
            printf("<span color='#859900' background=\'%s\'>\ue0b2</span><span background='#859900'> \uf6d9 %.2fkb/s \ufa51 %.2fkb/s </span>", time_color, (down2 - down1)/1024.0, (up2 - up1)/1024.0);
        }

        /* Wifi is disconnected */
        else {
            printf("<span color='#dc322f' background=\'%s\'>\ue0b2</span><span background='#dc322f'> \ufaa8 </span>", time_color);
            sleep(1);
        }
    }

    /* Wifi is off */
    else {
        printf("<span color='#dc322f' background=\'%s\'>\ue0b2</span><span background='#dc322f'> \ufaa9 </span>", time_color);
        sleep(1);
    }

    return 0;
}

void
strNumFromNCpy(char *dest, char *src, short n, short num){

    short count = 0;
    for(; count < num; count++){
        dest[count] = src[n];
        n++;
    }
    dest[count] = '\0';
}
