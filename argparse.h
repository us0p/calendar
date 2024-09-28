// Copyright 2024 Luan Lopes

#ifndef _ARGPARSE
#define _ARGPARSE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct option {
  char *shortName;
  char *longName;
  char *help;
};

struct command {
  char *command;
  char *help;
  int optionCount;
  struct option *options[];
};

// should also receive options;
struct cli {
  char *name;
  char *help;
  int commandCount;
  struct command *commands[];
};

enum objectType { CLI, COMMAND, OPTION };

struct option *optionFactory(char *shortName, char *longName, char *help);
struct command *commandFactory(char *command, char *help, struct option *opts[],
                               int opCount);
struct cli *cliFactory(char *name, char *help, struct command *commands[],
                       int commandCount);
void printCli(struct cli *commandLine);
void printHelp(const void *object, enum objectType objType);

#endif
