// Copyright 2024 Luan Lopes
#include <stdio.h>
#include <string.h>
#include <time.h>

void printHeader(struct tm start) {
  char *months[12] = {"January",   "February", "March",    "April",
                      "May",       "June",     "June",     "August",
                      "September", "October",  "November", "December"};
  char *month = months[start.tm_mon];
  int len = strlen(month);
  int line_length = 27;
  int pad_count = (line_length - len) / 2;
  for (int i = 0; i < pad_count; i++) {
    printf(" ");
  }
  printf("%s\n", month);
  printf("Sun Mon Tue Wed Thu Fri Sat\n");
}

void printCurrentMonth() {
  time_t epochSecondsToday = time(NULL);
  struct tm today, month;
  localtime_r(&epochSecondsToday, &today);

  const time_t secondsInADay = 60 * 60 * 24;
  time_t daysPassedInSeconds = (today.tm_mday - 1) * secondsInADay;
  time_t epochSecondsMonth = epochSecondsToday - daysPassedInSeconds;

  localtime_r(&epochSecondsMonth, &month);
  printHeader(month);
  while (month.tm_mon == today.tm_mon) {
    if (today.tm_mon != month.tm_mon)
      break;

    if (month.tm_mday == today.tm_mday) {
      printf(" \33[38;5;202m%02d\33[39;49m", month.tm_mday);
    } else {
      printf(" %02d", month.tm_mday);
    }
    if (month.tm_wday == 6) {
      printf("\n");
    } else {
      printf(" ");
    }
    epochSecondsMonth += secondsInADay;
    localtime_r(&epochSecondsMonth, &month);
  }
  printf("\n");
}

int main(int argc, char *argv[]) { printCurrentMonth(); }
