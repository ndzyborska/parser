#include "parse.h"

int main(int argc, char** argv) {

  FILE* fp;
  char* turtle_code;
  Program* p;

  p = init_program();

  turtle_code = argv[1];

  if (argc > 2) {
    if (strcmp(argv[2], "testall") == 0 ) {
     testall(argc, argv);
    }
  }

  fp = fopen(turtle_code, "r");

  if (!fp) {
     printf("Can't open file");
     exit(1);
  }

  insert_code(p, fp);
  fclose(fp);

  start(p);

  free_program(p);

 return 0;
}

/* #####################  INIT PROGRAM ##################### */

Program* init_program()       {

  Program* p;

  p = (Program*)malloc(sizeof(Program));

  p -> cw = 0;
  p-> stack = 0;
  p -> vars = NULL;
  assert(p -> vars == NULL);

  init_code(p);

  return p;
}

void init_code(Program* p) {

  int i;
  for (i = 0; i < MAXLEGNTH; i++)  {
      p -> code[i][0] = '\0';
    }
}

void insert_code(Program* p, FILE* fp) {

  int x = 0;
  int line = 1;
  char c;

  while (fscanf(fp, "%s", p->code[x++]) == 1 && x < MAXLEGNTH)  {
    p->line_index[x] = line;
    if ((c = fgetc(fp)) == '\n') {
      p->line_index[x] = line;
      line++;
    }
  }
}

bool string_match(Program* p, char* s) {

  if (strcmp(p->code[p->cw], s) == 0) {
    increment(p);
    return true;
  }
  return false;
}

void increment(Program* p)   {
  p->cw++;
}

void error(char* message, Program* p) {

  printf("%s", message);
  printf("The error occured at the word %s, which was word number %d.\n", p->code[p->cw-1], p->cw);
  printf("The line number is %d.\n", p->line_index[p->cw]);
  exit(2);

}

/* #####################  BEGIN THE PARSE ##################### */

void start(Program* p)    {

  if (string_match(p, "{")) {
    end(p);
  }
  else {
    error("Open brace '{' is missing\n", p);
  }

  if (p->code[p->cw][0] == '\0') {
    printf("Parse Complete\n");
  }
}

void end(Program* p) {

  if (string_match(p, "}") == true) {
    return;
  }
  statement(p);
  end(p);
}

void statement(Program* p)     {

  if (string_match(p, "DO"))  {
    DO(p->code[p->cw], p);
    return;
  }

  if (string_match(p, "SET")) {
    SET(p->code[p->cw], p);
    return;
  }

  if (string_match(p, "RT")) {
    RT(p->code[p->cw], p);
    return;
  }

  if (string_match(p, "FD"))  {
    FD(p->code[p->cw], p);
    return;
  }

  if (string_match(p, "LT"))  {
    LT(p->code[p->cw], p);
    return;
  }

error("Invalid instruction\n", p);

}

/* #####################  VARIABLE CHECKS  ##################### */


bool valid_variable(char* str) {

  if ((strlen(str) == 1) && (str[0] >= 'A' && str[0] <= 'Z'))  {
    return true;
  }
  return false;
}

bool in_range(double number, double move_or_turn) {

  if ((number >= 0) && (number <= move_or_turn)) {
    return true;
  }
  return false;
}

bool all_numbers(char* str) {

  unsigned int i;

  for (i = 0; i < strlen(str); i++) {
    if ((str[i] < '0' || str[i] > '9') && str[i] != '.' ) {
      return false;
    }
  }
  return true;
}

double varnum(char* str, Program* p) {

  double number;

  increment(p);

  if (all_numbers(str)) {
    (sscanf(str, "%lf ", &number));
    return number;
  }

  if (valid_variable(str)) {
    number = var_retriever(str, p);
    return number;
  }

  error("Variable invalid\n", p);
  return -1;

}

void create_var(Program* p, char* name, double value) {

  Variable* var ;

  if (var_check(name, p) == true) {
    change_var(p, name, value);
    return;
  }

  var = (Variable*)malloc(sizeof(Variable));
  assert(var != NULL);
  var -> name = (char*)calloc(VAR_SZ, sizeof(char));
  assert(var->name != NULL);


    strcpy(var -> name, name);
    var -> val = value;

    var -> next = p -> vars;
    p -> vars = var;
    if (p -> vars == NULL) {
      error("Variable could not be created\n", p);
    }
}

void change_var(Program* p, char* name, double new_value) {

  Variable* tmp;
  tmp = p -> vars;

  while(tmp  != NULL) {

    if(strcmp(tmp->name, name) == 0) {
      tmp-> val = new_value;
      return;
    }
    tmp = tmp -> next;
  }

  error("Variable not found\n", p);
}


double var_retriever(char* name, Program* p) {

  Variable* tmp = p-> vars;

  if (p -> vars == NULL) {
    error("No variables in list\n", p);
    return -1;
  }

  while (tmp  != NULL)   {
    if (strcmp(tmp->name, name) == 0) {
      return tmp->val;
    }
    tmp = tmp -> next;
  }

  error("No variable included\n", p);
  return -1;
}

/* Similar functions, however with intention to correctly define a "no variable" error in var_retriever,
and to check if true or false in var_check for other functions without exiting */

bool var_check(char* name, Program* p) {

    Variable* tmp = p-> vars;

    if (p -> vars == NULL) {
    return false;
    }

    while (tmp  != NULL)   {
      if (strcmp(tmp->name, name) == 0) {
        return true;
      }
      tmp = tmp -> next;
    }
    return false;
}

/* ########################## INSTRUCTIONS ########################### */

void FD(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXMOVEMENT)) {
    return;
  }
  error("FD number not in range\n", p);
}


void RT(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXTURN)) {
    return;
  }
  error("RT number not in range\n", p);
}

void LT(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXTURN)) {
    return;
  }

  error("LT number not in range\n", p);
}

void DO(char* string, Program* p) {

  double i;
  double loop_start;
  double loop_limit;
  double loop_cw, save_word = 0;

  if (!valid_variable(string)) {
    error("Variable in 'DO' not valid\n", p);
  }

  increment(p);

  if (!string_match(p, "FROM")) {
    error("Missing FROM in 'DO' statement\n", p);
  }

  loop_start = varnum(p->code[p->cw], p);

  create_var(p, string, loop_start);

  if (!string_match(p, "TO")) {
    error("Missing 'TO' for loop in 'DO' statement\n", p);
  }

  loop_limit = varnum(p->code[p->cw], p);
  loop_cw = p -> cw;

  for (i = loop_start; i <= loop_limit; i++)   {
    change_var(p, string, i);
    start(p);
    save_word = p->cw;
    p -> cw = loop_cw;
  }
  p -> cw = save_word;

}

void SET(char* string, Program* p) {

  if (!valid_variable(string)) {
    error("Variable in 'SET' not valid\n", p);
  }

  increment(p);

  if (!string_match(p, ":="))  {
    error("Missing ':=' in 'SET'\n", p);
  }

  if (Polish(p) == false) {
    error("Polish notation incorrect\n", p);

  }
  else {
    create_var(p, string, Polish(p));
  }
  increment(p);
}

bool Polish(Program* p)   {

 if(strcmp(p->code[p->cw], ";") == 0) {
   if (p -> stack == 1) {
     p -> stack = 0;
     return true;
   }
   else {
     return false;
   }
 }

  if (operator(p->code[p->cw])) {
    increment(p);
    p -> stack--;
  }

  else {
    varnum(p->code[p->cw], p);
    p -> stack++;
  }

  return Polish(p);

}


bool operator(char* string)   {

  if (strcmp(string, "*") == 0) {
    return true;
  }
  if (strcmp(string, "/") == 0) {
    return true;
  }
  if (strcmp(string, "+") == 0) {
    return true;
  }
  if (strcmp(string, "-") == 0) {
    return true;
  }
  return false;
}

void free_program(Program* p) {

Variable *temp;
temp = p -> vars;

  while (p->vars != NULL) {
    temp = p->vars;
    p -> vars = temp -> next;
    free(temp -> name);
    free(temp);
  }

  free(p);

}
