// Copyright 2024 Luan Lopes

#ifndef _ARGPARSE
#define _ARGPARSE 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum objectType { CLI, COMMAND, OPTION };
enum type { INT, STR, BOOL };

struct valueContainer {
  enum type t;
  char *name;
  char *value;
  int valCount;
};

struct option {
  char *shortName;
  char *longName;
  char *help;
  bool exec;
  struct valueContainer *vc;
};

struct command {
  char *command;
  char *help;
  int optionCount;
  bool exec;
  struct option **options;
  struct valueContainer *vc;
};

struct cli {
  char *name;
  char *help;
  int commandCount;
  int optionCount;
  struct option **options;
  struct command **commands;
};

struct valueContainer *vcFactory(enum type t, char *name, char *value,
                                 int valCount);
struct option *optionFactory(char *shortName, char *longName, char *help,
                             struct valueContainer *vc);
struct command *commandFactory(char *command, char *help, struct option *opts[],
                               int opCount, struct valueContainer *vc);
struct cli *cliFactory(char *name, char *help, struct command **commands,
                       int commandCount, struct option **opts, int optCount);
void printHelp(const void *object, enum objectType objType, char *appName);
void matchValues(char *appName, struct cli *cmdLine, int argc, char *argv[]);

#endif
