// Copyright 2024 Luan Lopes
#include "./argparse.h"
#include "./calendar.h"

int main(int argc, char *argv[]) {
  char *appName = "Calendar";

  struct command *commands[] = {
      commandFactory("current", "Prints current month.", NULL, 0),
      commandFactory("month", "Prints selected month.", NULL, 0),
  };
  struct cli *commandLine =
      cliFactory(appName, "An app for getting information about the calendar.",
                 commands, 2, NULL, 0);

  struct tm todayStruct;
  time_t today = getToday(&todayStruct);

  printHelp(commandLine, CLI, appName);
}
