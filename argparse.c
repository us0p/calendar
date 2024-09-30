// Copyright 2024 Luan Lopes
#include "./argparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HELP_SPACES 10
#define DEFAULT_VALUE_SEPARATOR ","
#define SPACE_LEN 1

struct valueContainer *vcFactory(enum type t, char *name, char *value,
                                 int valCount) {
  struct valueContainer *vc = malloc(sizeof(struct valueContainer));
  if (vc == NULL) {
    fprintf(stderr, "Failed to malloc vcFactory().\n");
    exit(1);
  }
  vc->t = t;
  vc->name = name;
  vc->value = value;
  vc->valCount = valCount;
  return vc;
}

struct option *optionFactory(char *shortName, char *longName, char *help,
                             struct valueContainer *vc) {
  struct option *op = malloc(sizeof(struct option));
  if (op == NULL) {
    fprintf(stderr, "Failed to malloc() optionFactory().\n");
    exit(1);
  }
  op->shortName = shortName;
  op->longName = longName;
  op->help = help;
  op->vc = vc;
  op->exec = false;
  return op;
}

struct command *commandFactory(char *command, char *help, struct option **opts,
                               int opCount, struct valueContainer *vc) {
  struct command *cm = malloc(sizeof(struct command));
  if (cm == NULL) {
    fprintf(stderr, "Failed to malloc commandFactory().\n");
    exit(1);
  }
  cm->command = command;
  cm->help = help;
  cm->optionCount = opCount;
  cm->options = opts;
  cm->vc = vc;
  cm->exec = false;
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
    struct command *cmd = cmds[i];
    int cmdLen = strlen(cmd->command);
    if (cmd->vc != NULL) {
      cmdLen += SPACE_LEN + strlen(cmd->vc->name);
    }
    if (cmdLen > max)
      max = cmdLen;
  }
  return max;
}

int getBiggestOptsLen(struct option *opts[], int optCount) {
  int max = 0;
  for (int i = 0; i < optCount; i++) {
    struct option *opt = opts[i];
    int sLen = strlen(opt->shortName);
    int lLen = strlen(opt->longName);
    int commaSpaceLen = 2;
    int totalLen = sLen + commaSpaceLen + lLen;
    if (opt->vc != NULL) {
      totalLen += SPACE_LEN + strlen(opt->vc->name);
    }
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
    printf("   %s, %s", opt->shortName, opt->longName);
    if (opt->vc != NULL) {
      printf(" %s", opt->vc->name);
      totalLen += SPACE_LEN + strlen(opt->vc->name);
    }
    int extraSpaces = biggestOptLen - totalLen;
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
  if (cmd->vc != NULL) {
    printf("usage: %s [OPTIONS] %s <%s> [OPTIONS]\n", appName, cmd->command,
           cmd->vc->name);
  } else {
    printf("usage: %s [OPTIONS] %s [OPTIONS]\n", appName, cmd->command);
  }
  printf("\n");

  printf("%s\n", cmd->help);
  printf("\n");

  printf("positional arguments:\n");
  if (cmd->vc != NULL) {
    printf("    %s\n", cmd->vc->name);
  }
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

void matchValues(char *appName, struct cli *cmdLine, int argc, char *argv[]) {
  int commandsPointer = 0;
  int optionPointer = 0;
  // skipping program name.
  int valuesPointer = 1;
  while (valuesPointer < argc) {
    char *value = argv[valuesPointer++];
    if (value[0] == '-') {
      if (strcmp("-h", value) == 0 || strcmp("--help", value) == 0) {
        printHelp(cmdLine, CLI, appName);
        return;
      }
      if (cmdLine->options != NULL) {
        while (optionPointer < cmdLine->optionCount) {
          if (optionPointer >= cmdLine->optionCount) {
            printf("unexpected cli option %s.\n", value);
            printHelp(cmdLine, CLI, appName);
            return;
          }
          struct option *cmdLineOpt = cmdLine->options[optionPointer];
          if (strcmp(cmdLineOpt->shortName, value) == 0 ||
              strcmp(cmdLineOpt->longName, value) == 0) {
            optionPointer = 0;
            cmdLineOpt->exec = !cmdLineOpt->exec;
            if (cmdLineOpt->vc) {
              struct valueContainer *vc = cmdLineOpt->vc;
              for (int i = 0; i < vc->valCount; i++) {
                if (valuesPointer >= argc) {
                  fprintf(stderr, "insuficient parameters for cli option %s.\n",
                          cmdLineOpt->longName);
                  exit(1);
                }
                value = argv[valuesPointer++];
                char *tmpValue = vc->value;
                int optLen = tmpValue ? strlen(tmpValue) : 1;
                int currentLen = strlen(value);
                // making space for comma
                int totalLen = optLen + currentLen + 1;
                char *tmpBuf = malloc(sizeof(char) * totalLen);
                snprintf(tmpBuf, totalLen, "%s,%s", tmpValue, value);
                vc->value = tmpBuf;
                free(tmpValue);
              }
              break;
            }
          }
          optionPointer++;
        }
        continue;
      }
      printf("unexpected cli option %s.\n", value);
      printHelp(cmdLine, CLI, appName);
      return;
    }
    if (cmdLine->commands != NULL) {
      while (commandsPointer < cmdLine->commandCount) {
        if (commandsPointer >= cmdLine->commandCount) {
          fprintf(stderr, "unexpected positional argument %s for cli.\n",
                  value);
          exit(1);
        }
        struct command *cmd = cmdLine->commands[commandsPointer];
        if (strcmp(cmd->command, value) == 0) {
          cmd->exec = !cmd->exec;
          if (cmd->vc) {
            for (int i = 0; i < cmd->vc->valCount; i++) {
              if (valuesPointer >= argc) {
                fprintf(stderr, "insuficient parameters for command %s.\n",
                        cmd->command);
                exit(1);
              }
              value = argv[valuesPointer++];
              if (strcmp("-h", value) == 0 || strcmp("--help", value) == 0) {
                printHelp(cmd, COMMAND, appName);
                return;
              }
              char *tmpValue = cmd->vc->value;
              int positionalLen = strlen(value);
              int currentLen = tmpValue ? strlen(tmpValue) : 1;
              // making space for aditional comma
              int totalLen = positionalLen + currentLen + 1;
              char *tmpBuf = malloc(sizeof(char) * totalLen);
              snprintf(tmpBuf, totalLen, "%s,%s", tmpValue ? tmpValue : "",
                       value);
              cmd->vc->value = tmpBuf;
              free(tmpValue);
            }
          }
          while (valuesPointer < argc) {
            value = argv[valuesPointer++];
            if (strcmp("-h", value) == 0 || strcmp("--help", value) == 0) {
              printHelp(cmd, COMMAND, appName);
              return;
            }
            if (cmd->options != NULL) {
              optionPointer = 0;
              while (optionPointer < cmd->optionCount) {
                if (optionPointer >= cmd->optionCount) {
                  printf("unexpected %s option %s.\n", cmd->command, value);
                  printHelp(cmd, COMMAND, appName);
                  return;
                }
                struct option *cmdOpt = cmd->options[optionPointer];
                if (strcmp(cmdOpt->shortName, value) == 0 ||
                    strcmp(cmdOpt->longName, value) == 0) {
                  optionPointer = 0;
                  cmdOpt->exec = !cmdOpt->exec;
                  if (cmdOpt->vc) {
                    struct valueContainer *vc = cmdOpt->vc;
                    for (int i = 0; i < vc->valCount; i++) {
                      if (valuesPointer >= argc) {
                        fprintf(stderr,
                                "insuficient parameters for %s option %s.\n",
                                cmd->command, cmdOpt->longName);
                        exit(1);
                      }
                      value = argv[valuesPointer++];
                      char *tmpValue = vc->value;
                      int optLen = tmpValue ? strlen(tmpValue) : 1;
                      int currentLen = strlen(value);
                      // making space for comma
                      int totalLen = optLen + currentLen + 1;
                      char *tmpBuf = malloc(sizeof(char) * totalLen);
                      snprintf(tmpBuf, totalLen, "%s,%s", tmpValue, value);
                      vc->value = tmpBuf;
                      free(tmpValue);
                    }
                    break;
                  }
                }
                optionPointer++;
              }
              continue;
            }
            printf("unexpected %s option %s.\n", cmd->command, value);
            printHelp(cmd, COMMAND, appName);
            return;
          }
        }
        commandsPointer++;
      }
      continue;
    }
    printf("unexpected cli positional argument %s.\n", value);
    printHelp(cmdLine, CLI, appName);
    return;
  }
}
