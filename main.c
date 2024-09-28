// Copyright 2024 Luan Lopes
#include "./argparse.h"
#include "./calendar.h"

int main(int argc, char *argv[]) {
  struct tm todayStruct;
  time_t today = getToday(&todayStruct);
  // printCurrentMonth(&todayStruct);
  struct option **opt;
  struct command *cmds[1] = {
      commandFactory("test", "a wounderful description for test.", opt, 0)};
  struct cli *CLI =
      cliFactory("Super test", "a wounderful description for cli.", cmds, 1);
  printCli(CLI);
}
