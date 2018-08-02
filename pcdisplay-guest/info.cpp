#include "info.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CMM_CASE(s) (strcmp (command, s) == 0)

static void parse_media (char *args);

info_t INFO;

char REQUEST_COMMANDS[][8] = {"CPU",
                              "RAM",
                              "NAME",
                              "NET",
                              "TIME",
                              "VOLUME",
                              "TEMP",
                              "SYSINFO",
                              "MEDIA"};

void info_init () {
    INFO.cpu_usage    = 0;
    INFO.volume       = 0;
    INFO.temp         = 0;
    INFO.ram_used     = 0; INFO.ram_total      = 1;
    INFO.net_up_speed = 0; INFO.net_down_speed = 0;
    INFO.day          = 0; INFO.month          = 0; INFO.year         = 0;
    INFO.hour         = 0; INFO.min            = 0; INFO.sec          = 0;
    INFO.media_track  = -1;

    *INFO.computer_name = '\0';
    *INFO.sys_info      = '\0';
    *INFO.media_title   = '\0';
    *INFO.media_album   = '\0';
    *INFO.media_artist  = '\0';
}

void parse (char *s) {
    if (s == NULL) return;
    char *s2 = (char *) malloc ((strlen (s) + 1) * sizeof (char));
    strcpy (s2, s);

    char *p = strtok (s2, " ");
    char *command = (char *) malloc ((strlen (p) + 1) * sizeof (char));
    strcpy (command, p);

    free (s2);

    char *args = s + strlen (command) + 1;

    if      (CMM_CASE ("CPU"))     sscanf (args, "%d",           &INFO.cpu_usage);
    else if (CMM_CASE ("RAM"))     sscanf (args, "%ld%ld",       &INFO.ram_used, &INFO.ram_total);
    else if (CMM_CASE ("NAME"))    sscanf (args, "%s",           INFO.computer_name);
    else if (CMM_CASE ("NET"))     sscanf (args, "%ld%ld",       &INFO.net_down_speed, &INFO.net_up_speed);
    else if (CMM_CASE ("TIME"))    sscanf (args, "%d%d%d%d%d%d", &INFO.day, &INFO.month, &INFO.year, &INFO.hour, &INFO.min, &INFO.sec);
    else if (CMM_CASE ("VOLUME"))  sscanf (args, "%d",           &INFO.volume);
    else if (CMM_CASE ("TEMP"))    sscanf (args, "%d",           &INFO.temp);
    else if (CMM_CASE ("SYSINFO")) sscanf (args, "%s",           INFO.sys_info);
    else if (CMM_CASE ("MEDIA"))   parse_media (args);

    free (command);
}

void print_pretty () {
    printf ("CPU:  %d\n",                 INFO.cpu_usage);
    printf ("RAM:  %f\n",                 (float) INFO.ram_used / INFO.ram_total);
    printf ("NET:  %ld %ld\n",            INFO.net_up_speed, INFO.net_down_speed);
    printf ("DATE: %d/%d/%d\n",           INFO.day, INFO.month, INFO.year);
    printf ("TIME: %d:%d:%d\n",           INFO.hour, INFO.min, INFO.sec);
    printf ("VOL:  %d\n",                 INFO.volume);
    printf ("TEMP: %d\n",                 INFO.temp);
    printf ("NAME: %s\n",                 INFO.computer_name);
    printf ("INFO: %s\n",                 INFO.sys_info);
    printf ("MEDIA: %d - %s (%s) - %s\n", INFO.media_track, INFO.media_title, INFO.media_album, INFO.media_artist);

    putchar ('\n');
}

static void parse_media (char *args) {
    char buffer[128];
    char *dest[] = {INFO.media_title, INFO.media_album, INFO.media_artist};
    int backslash = 0;
    int dest_index = 0;
    char *c, *b = buffer;

    for (c = args; *c != '\0' && dest_index < 3; c++) {
        // Unescaping
        if (backslash) {
            backslash = 0;

            switch (*c) {
            case 'n':  *b = '\n'; break;
            case '\\': *b = '\\'; break;
            case 't':  *b = '\t'; break;
            case '\'': *b = '\''; break;
            default:   *b = *c;
            }

            b++;
            continue;
        }

        switch (*c) {
        case'\\':
            backslash = 1;
            break;

        case ' ':
            *b = '\0';
            strcpy (dest[dest_index++], buffer);
            b = buffer;
            break;

        default:
            *b = *c;
            b++;
        }
    }

    sscanf (c, "%d", &INFO.media_track);
}
