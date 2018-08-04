#ifndef _INFO
#define _INFO

#define IS_HELLO(s) (strcmp (s, "HELLO") == 0)

struct info_t {
    int  cpu_usage;
    int  volume;
    int  temp;
    long ram_used;          long ram_total;
    long net_up_speed;      long net_down_speed;
    int  day;               int  month;            int  year;
    int  hour;              int  min;              int  sec;
    char media_title[128];  char media_album[128]; char media_artist[64]; int media_track;
    char computer_name[32];
    char sys_info[32];
};

extern info_t INFO;
extern char REQUEST_COMMANDS[][8];

void info_init ();

void parse (char *s);

void print_pretty();

#endif

