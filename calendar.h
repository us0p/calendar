// Copyright 2024 Luan Lopes
#ifndef _CALENDAR_H

#define _CALENDAR_H 1
#define _GNU_SOURCE 1
#define SECONDS_IN_A_DAY 60 * 60 * 24

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void printHeader(struct tm *start);

void padWeek(struct tm *startMonth);
void printMonth(struct tm *startMonth, time_t startMonthTime, struct tm *today);

time_t getToday(struct tm *t);
void printSelectedMonth(struct tm *today, int month);
void printCurrentMonth(struct tm *today);
#endif
