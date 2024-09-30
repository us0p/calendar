// Copyright 2024 Luan Lopes
#include <stdlib.h>
#include <string.h>

#include "./argparse.h"
#include "./calendar.h"

int main(int argc, char *argv[]) {
  char *appName = "Calendar";

  struct command *commands[] = {
      commandFactory("current", "Prints current month.", NULL, 0, NULL),
      commandFactory("month", "Prints selected month.", NULL, 0,
                     vcFactory(INT, "month-number", NULL, 1)),
  };
  struct cli *commandLine =
      cliFactory(appName, "An app for getting information about the calendar.",
                 commands, 2, NULL, 0);

  matchValues(appName, commandLine, argc, argv);

  struct tm todayStruct;
  time_t today = getToday(&todayStruct);

  for (int i = 0; i < commandLine->commandCount; i++) {
    struct command *cmd = commandLine->commands[i];
    if (cmd->exec) {
      if (strcmp(cmd->command, "current") == 0) {
        printCurrentMonth(&todayStruct);
        return 0;
      }
      if (strcmp(cmd->command, "month") == 0) {
        int month = atoi(&cmd->vc->value[1]);
        printSelectedMonth(&todayStruct, month - 1);
      }
    }
  }
}
