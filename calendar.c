// Copyright 2024 Luan Lopes
#define _GNU_SOURCE 1
#define SECONDS_IN_A_DAY 60 * 60 * 24

#include "./calendar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printHeader(struct tm *start) {
  char *months[12] = {"January",   "February", "March",    "April",
                      "May",       "June",     "June",     "August",
                      "September", "October",  "November", "December"};
  char *month = months[start->tm_mon];
  int len = strlen(month);
  int line_length = 27;
  int pad_count = (line_length - len) / 2;
  for (int i = 0; i < pad_count; i++) {
    printf(" ");
  }
  printf("%s\n", month);
  printf("Sun Mon Tue Wed Thu Fri Sat\n");
}

void padWeek(struct tm *startMonth) {
  for (int pad = 0; pad < startMonth->tm_wday; pad++) {
    printf("    ");
  }
}

void printMonth(struct tm *startMonth, time_t startMonthTime,
                struct tm *today) {
  int month = startMonth->tm_mon;
  printHeader(startMonth);
  padWeek(startMonth);
  while (month == startMonth->tm_mon) {
    if (month != startMonth->tm_mon)
      break;

    if (startMonth->tm_mday == today->tm_mday) {
      if (startMonth->tm_mon == today->tm_mon) {
        printf("\33[38;5;202m");
      }
    }

    printf(" %02d", startMonth->tm_mday);
    printf("\33[39;49m");

    if (startMonth->tm_wday == 6) {
      printf("\n");
    } else {
      printf(" ");
    }
    startMonthTime += SECONDS_IN_A_DAY;
    localtime_r(&startMonthTime, startMonth);
  }
  printf("\n");
}

time_t getToday(struct tm *t) {
  time_t epochSecondsToday = time(NULL);
  if (epochSecondsToday == -1) {
    fprintf(stderr, "function time() failed");
    exit(1);
  }
  localtime_r(&epochSecondsToday, t);
  return epochSecondsToday;
}

void printSelectedMonth(struct tm *today, int month) {
  char dateStr[20];
  snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d %02d:%02d:%02d",
           today->tm_year + 1900, month + 1, 1, 0, 0, 0);
  struct tm startMonth;
  char *unparsedChar = strptime(dateStr, "%F %H:%M:%S", &startMonth);
  if (!unparsedChar) {
    fprintf(stderr, "Failed to strptime() from string %s, error at: %c, %p\n",
            dateStr, *unparsedChar, unparsedChar);
    exit(1);
  }
  startMonth.tm_isdst = 0;
  time_t startMonthTime = mktime(&startMonth);
  if (startMonthTime == -1) {
    fprintf(stderr,
            "Function mktime() failed to parse broken time structure.\n");
    exit(1);
  }
  printMonth(&startMonth, startMonthTime, today);
}

void printCurrentMonth(struct tm *today) {
  printSelectedMonth(today, today->tm_mon);
}
