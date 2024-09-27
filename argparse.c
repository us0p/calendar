// Copyright 2024 Luan Lopes
struct option {
  char *shortName;
  char *longName;
  char *help;
};

struct command {
  char *command;
  char *help;
  struct option options[];
};
