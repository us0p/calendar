// Copyright 2024 Luan Lopes
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

struct command *commandFactory(char *command, char *help, struct option *opts[],
                               int opCount) {
  struct command *cm =
      malloc(sizeof(struct command) + opCount * sizeof(struct option));
  if (cm == NULL) {
    fprintf(stderr, "Failed to malloc commandFactory().\n");
    exit(1);
  }
  cm->command = command;
  cm->help = help;
  memcpy(cm->options, opts, opCount);
  return cm;
}

struct cli *cliFactory(char *name, char *help, struct command *commands[],
                       int commandCount) {
  struct cli *commandLine =
      malloc(sizeof(struct cli) + commandCount * sizeof(struct command));
  if (commandLine == NULL) {
    fprintf(stderr, "Failed to malloc cliFactory().\n");
    exit(1);
  }
  commandLine->name = name;
  commandLine->help = help;
  memcpy(commandLine->commands, commands, commandCount);
  return commandLine;
}

enum objectType { CLI, COMMAND, OPTION };

void printCli(struct cli *commandLine) {
  char *cliCommands[commandLine->commandCount];
  char *commandDescription[commandLine->commandCount];

  for (int i = 0; i < commandLine->commandCount; i++) {
    struct command *cmd = commandLine->commands[i];
    cliCommands[i] = cmd->command;
    commandDescription[i] = cmd->help;
  }

  printf("usage: %s [-h] ", commandLine->name);

  printf("{");
  for (int i = 0; i < commandLine->commandCount; i++) {
    printf("%s", cliCommands[i]);
    if (i + 1 < commandLine->commandCount) {
      printf(",");
    }
  }
  printf("} ...\n");
  printf("\n");
  printf("%s\n", commandLine->help);
  printf("\n");
  printf("positional arguments:\n");
  for (int i = 0; i < commandLine->commandCount; i++) {
    printf("    %s\n", cliCommands[i]);
  }
}

void printHelp(const void *object, enum objectType objType) {
  switch (objType) {
  case CLI: {
    struct cli *commandLine = (struct cli *)object;
    break;
  }
  case COMMAND: {
    struct command *cmd = (struct command *)object;
    break;
  }
  case OPTION: {
    struct option *opt = (struct option *)object;
    break;
  }
  default: {
    fprintf(stderr, "Invalid object type.\n");
    exit(1);
  }
  }
}
