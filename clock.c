/*

 This program is managing simple system clock for Ubuntu. User can set/clear/get currently set alarm.


 * 
 * int alarm_hour;          // Stores the hour of the alarm
 * int alarm_minute;        // Stores the minute of the alarm
 * int stop;                // Flag to stop the timer thread
 * int alarm_stop;          // Flag to indicate if the alarm has been triggered
 * const char* FILENAME;    // File name to store alarm settings
 * pthread_mutex_t lock;    // Mutex lock for thread synchronization
 * 
 * void save_alarm();       // Function to save alarm settings to a file
 * void load_alarm();       // Function to load alarm settings from a file
 * void *timer(void *arg);  // Timer function to check for alarm trigger
 * int main(void);          // Main function to control alarm setting and interaction
 * 
 * Conditionals:
 * 
 * set <hour>:<minute>      // Set the alarm time to the specified hour and minute
 * clear                    // Clear the currently set alarm
 * get                      // Get the currently set alarm time
 * exit                     // Exit the program

*********************************************************************************************
*/


#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int alarm_hour;
int alarm_minute;
int stop;
int alarm_stop;
const char* FILENAME = "alarm.txt";

pthread_mutex_t lock;

void save_alarm(){
    FILE *f = fopen(FILENAME, "w");
    if (!f){
        fprintf(stderr, "Failed to open file %s\n", FILENAME);
        return;                         //void
    }
    fprintf(f, "%d:%d:%d", alarm_hour, alarm_minute, alarm_stop);
    fclose(f);
}

void load_alarm(){
    FILE *f = fopen(FILENAME, "r");
    if (!f) return;                         //void
    fscanf(f, "%d:%d:%d", &alarm_hour, &alarm_minute, &alarm_stop);
    fclose(f);
}


void *timer(void *arg){
    while(1){
        sleep(1);

        pthread_mutex_lock(&lock);
        int tmp = stop;
        pthread_mutex_unlock(&lock);
        if (tmp) break;

        time_t current_time = time(NULL);
        struct tm tm;
        localtime_r(&current_time, &tm);
        int current_hour = tm.tm_hour;
        int current_minute = tm.tm_min;

        pthread_mutex_lock(&lock);
        if(current_hour == alarm_hour && current_minute == alarm_minute)
        {
            printf("<<ALARM>>\n");
            alarm_stop = 1;
            alarm_hour = 100;
            alarm_minute = 100;
            save_alarm();
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void){
    int ret = pthread_mutex_init(&lock, NULL);
    if (ret) return 1;

    load_alarm();

    pthread_t thread;
    ret = pthread_create(&thread, NULL, timer, NULL);
    if (ret) {
        pthread_mutex_destroy(&lock);
        return 1;
    }
    while (1) {
        printf(">>> ");

        char line[80];
        if (!fgets(line, sizeof(line), stdin)) break;
        
        int h, m;
        if (sscanf(line, "set %d:%d\n", &h, &m) == 2){
            pthread_mutex_lock(&lock);
            alarm_hour = h;
            alarm_minute = m;
            alarm_stop = 0;
            save_alarm();
            pthread_mutex_unlock(&lock);
        }
        if (strcmp("clear\n", line) == 0) {
            pthread_mutex_lock(&lock);
            alarm_hour = 100;
            alarm_minute = 100;
            save_alarm();
            pthread_mutex_unlock(&lock);
        }
        if (strcmp("get\n", line) == 0) {
            pthread_mutex_lock(&lock);
            if((alarm_hour <= 24 && alarm_hour >= 0) && (alarm_minute <= 59 && alarm_minute >= 0) && !alarm_stop) printf("alarm is set to %02d:%02d\n", alarm_hour, alarm_minute);
            else printf("alarm is not set\n");
            pthread_mutex_unlock(&lock);
        }
        if (strcmp("exit\n", line) == 0) break;

        pthread_mutex_lock(&lock);
        if (alarm_stop){
            
            alarm_hour = 100;
            alarm_minute = 100;
            save_alarm();
               
        }
        pthread_mutex_unlock(&lock);
    }

    pthread_mutex_lock(&lock);
    stop = 1;
    pthread_mutex_unlock(&lock);
    pthread_join(thread, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}


