// Copyright 2024 Luan Lopes
#include "./argparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct option *optionFactory(char *shortName, char *longName, char *help) {
  struct option *op = malloc(sizeof(struct option));
  if (op == NULL) {
    fprintf(stderr, "Failed to malloc() optionFactory().\n");
    exit(1);
  }
  op->shortName = shortName;
  op->longName = longName;
  op->help = help;
  return op;
}

struct command *commandFactory(char *command, char *help, struct option **opts,
                               int opCount) {
  struct command *cm = malloc(sizeof(struct command));
  if (cm == NULL) {
    fprintf(stderr, "Failed to malloc commandFactory().\n");
    exit(1);
  }
  cm->command = command;
  cm->help = help;
  cm->optionCount = opCount;
  cm->options = opts;
  return cm;
}

struct cli *cliFactory(char *name, char *help, struct command **commands,
                       int commandCount, struct option **opts, int optCount) {
  struct cli *commandLine = malloc(sizeof(struct cli));
  if (commandLine == NULL) {
    fprintf(stderr, "Failed to malloc cliFactory().\n");
    exit(1);
  }
  commandLine->name = name;
  commandLine->help = help;
  commandLine->commandCount = commandCount;
  commandLine->commands = commands;
  commandLine->options = opts;
  commandLine->optionCount = optCount;
  return commandLine;
}

int getBiggestArgumentLen(struct command *cmds[], int cmdCount) {
  int max = 0;
  for (int i = 0; i < cmdCount; i++) {
    int cmdLen = strlen(cmds[i]->command);
    if (cmdLen > max)
      max = cmdLen;
  }
  return max;
}

int getBiggestOptsLen(struct option *opts[], int optCount) {
  int max = 0;
  for (int i = 0; i < optCount; i++) {
    int sLen = strlen(opts[i]->shortName);
    int lLen = strlen(opts[i]->longName);
    int commaSpaceLen = 2;
    int totalLen = sLen + commaSpaceLen + lLen;
    if (totalLen > max)
      max = totalLen;
  }
  return max;
}

void printSpaces(int times) {
  for (int j = 0; j < times; j++) {
    printf(" ");
  }
}

void printOptions(struct option *opts[], int optCount) {
  puts("options:");
  int biggestOptLen = getBiggestOptsLen(opts, optCount);
  biggestOptLen = biggestOptLen > 10 ? biggestOptLen : 10;
  for (int i = 0; i < optCount; i++) {
    struct option *opt = opts[i];
    int lLen = strlen(opt->longName);
    int sLen = strlen(opt->shortName);
    int commaSpaceLen = 2;
    int totalLen = sLen + commaSpaceLen + lLen;
    int extraSpaces = biggestOptLen - totalLen;
    printf("   %s, %s", opt->shortName, opt->longName);
    printSpaces(extraSpaces + DEFAULT_HELP_SPACES);
    printf("%s\n", opt->help);
  }
  printf("   -h, --help");
  printSpaces(biggestOptLen - 10 + DEFAULT_HELP_SPACES);
  printf("Prints this help message.\n");
}

void printCli(struct cli *commandLine) {
  printf("usage: %s [OPTIONS] [ARGUMENTS]\n", commandLine->name);
  printf("\n");

  printf("%s\n", commandLine->help);
  printf("\n");

  printf("positional arguments:\n");
  int biggestArgLen =
      getBiggestArgumentLen(commandLine->commands, commandLine->commandCount);
  for (int i = 0; i < commandLine->commandCount; i++) {
    struct command *cmd = commandLine->commands[i];
    int extraSpaces = biggestArgLen - strlen(cmd->command);
    printf("    %s", cmd->command);
    printSpaces(extraSpaces + DEFAULT_HELP_SPACES);
    printf("%s\n", cmd->help);
  }
  printf("\n");

  printOptions(commandLine->options, commandLine->optionCount);
}

void printCommand(struct command *cmd, char *appName) {
  printf("usage: %s [OPTIONS] %s ... [OPTIONS]\n", appName, cmd->command);
  printf("\n");

  printf("%s\n", cmd->help);
  printf("\n");

  printOptions(cmd->options, cmd->optionCount);
}

void printHelp(const void *object, enum objectType objType, char *appName) {
  switch (objType) {
  case CLI: {
    struct cli *commandLine = (struct cli *)object;
    printCli(commandLine);
    break;
  }
  case COMMAND: {
    struct command *cmd = (struct command *)object;
    printCommand(cmd, appName);
    break;
  }
  default: {
    fprintf(stderr, "Invalid object type.\n");
    exit(1);
  }
  }
}
