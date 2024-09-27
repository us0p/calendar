// Copyright 2024 Luan Lopes
#include "./calendar.h"

int main(int argc, char *argv[]) {
  struct tm todayStruct;
  time_t today = getToday(&todayStruct);
  printCurrentMonth(&todayStruct);
}
