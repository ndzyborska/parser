#include "logo.h"

int main(int argc, char** argv) {

  FILE* fp;
  char* turtle_code;
  Program* p;
  printf("reach1\n");


  p = init_program();
  printf("reach2\n");


  turtle_code = argv[1];

  if (argc > 2) {
     if (strcmp(argv[2], "val") == 0 ) {
      p -> valgrind = true;
     }
  }

  if (p -> valgrind == false) {
    Neill_SDL_Init(&(p->sw));
  }

  fp = fopen(turtle_code, "r");

  if (!fp) {
     printf("Can't open file");
     exit(1);
  }
printf("reach\n");
  insert_code(p, fp);
  fclose(fp);

  start(p);

  if (p -> valgrind == false) {
    end_program(&(p->sw));
  }
  free_program(p);

 return 0;
}

/* #####################  INIT PROGRAM ##################### */

Program* init_program()       {

  Program* p;

  p = (Program*)malloc(sizeof(Program));

  p -> cw = 0;
  p -> vars = NULL;
  assert(p -> vars == NULL);
  p -> valgrind = false;
  p -> top = NULL;
  assert(p -> top == NULL);

  init_code(p);
  init_coord(p);

  return p;
}

void init_code(Program* p) {

  int i;

  p -> code = malloc(sizeof *p->code * 3);
  if (p->code == NULL) {
    exit(1);
  }
  for (i=0; i<3; i++ ) {
    p -> code[i] = malloc(sizeof *p->code[0] * MAXLINE);

  }

}

void init_coord(Program* p) {

  p->co = (Coordinates*)malloc(sizeof(Coordinates));

  p -> co -> x_new = CENTRE_X;
  p -> co -> y_new = CENTRE_Y;
  p -> co -> x = 0;
  p -> co -> x = 0;
  p -> co -> angle = M_PI * STRAIGHT;

}

void insert_code(Program* p, FILE* fp) {

  int x = 0;
  int new = 2;
  char** tmp;
  int line = 1;
  int i;
  char c;

  while (fscanf(fp, "%s", p->code[x++])  && x < MAXLEGNTH)  {

    if (x >= ROW) {
      tmp = realloc(p->code, sizeof*p->code*(ROW + new));
      p -> code = tmp;
      p->code[ROW + new] = malloc(sizeof*p -> code[ROW + new] * MAXLINE);

    }



    p -> line_index[x] = line;
    if ((c = fgetc(fp)) == '\n') {
      p->line_index[x] = line;
      line++;
    }
  }


  for (i=0; i<x; i++) {
    printf("%s  %d\n", p->code[i], x);


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
  printf("The error occured at the word %s, which was word number %d.\n", p->code[p->cw], p->cw);
  printf("The line number is %d.\n", p->line_index[p->cw]);
  exit(2);

}

/* #####################  BEEGIN THE PARSE ##################### */


void start(Program* p)    {

  if (string_match(p, "{")) {
    end(p);
  }

  else {
    error("Open brace '{' is missing", p);
  }

  if (p->code[p->cw][0] == '\0') {
    printf("Parse Complete");
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

  if (string_match(p, "CHANGE")) {
    CHANGE(p);
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

  if (string_match(p, "MAKE")) {
    MAKE(p->code[p->cw], p);
    return;
  }

  if (string_match(p, "SPLIT")) {
    SPLIT(p->code[p->cw], p);
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

/*Similar functions, however with intention to correctly define no variable error,
and not making an exit if false*/

bool var_check(char* name, Program* p) {

    Variable* tmp = p-> vars;

    if (p->vars == NULL) {
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

/*########################## INSTRUCTIONS ###########################*/

void FD(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXMOVEMENT)) {
    find_coord(p->co, number);

    if (p->valgrind == false)  {
      art_attack(&(p->sw), p->co);
    }
    return;
  }
  error("FD number not in range\n", p);
}


void RT(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXTURN)) {
    p -> co->angle = p-> co->angle + number * RADIAN_CONV;
    return;
  }
  error("RT number not in range\n", p);
}

void LT(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXTURN)) {
    p -> co->angle = p-> co->angle - number * RADIAN_CONV;
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
  loop_cw = p->cw;

  for (i = loop_start; i <= loop_limit; i++)   {
    change_var(p, string, i);
    start(p);
    save_word = p->cw;
    p -> cw = loop_cw;
  }
  p -> cw = save_word;
}

void SET(char* string, Program* p) {

  Stack s;
  init_stack(&s);

  if (!valid_variable(string)) {
    error("Variable in 'SET' not valid\n", p);
  }

  increment(p);

  if (!string_match(p, ":="))  {
    error("Missing ':=' in 'SET'\n", p);
  }
   create_var(p, string, Polish(p, &s));

  free_stack(&s);
  increment(p);

}

void MAKE(char* string, Program* p) {

  double number = varnum(string, p);

  if (in_range(number, MAXMOVEMENT)) {
    p -> co->radius = number;
    if (p -> valgrind == false)  {
      circle_tastic(&(p->sw), p);
    }
  return;
  }

  error("MAKE number not in range\n", p);

}

void SPLIT(char* string, Program* p) {

    double number = varnum(string, p);
    if (in_range(number, MAXMOVEMENT)) {
      if (p->top == NULL) {
        p->top = init_tree(copy(p->co));
      }

    insert(p->top, p,  number);
    return;
    }

  error("SPLIT number not in range\n", p);
}

void CHANGE(Program* p) {

  double x, y;
  x = varnum(p->code[p->cw], p);

  if (in_range(x, MAXCOORD)) {
    p -> co-> x_new = x;
  }

  y = varnum(p->code[p->cw], p);

  if (in_range(y, MAXCOORD)) {
    p -> co-> y_new = y;
  }

}

/* ############################# SDL ############################## */

void find_coord(Coordinates* co, double number) {

  co->x = co->x_new;
  co->y = co->y_new;

  co->x_new = co->x + (sin(co->angle) * number);
  co->y_new = co->y + (cos(co->angle) * number);

}

void art_attack(SDL_Simplewin* sw, Coordinates* co) {

  if ( (int)co -> x_new ==  (int)co -> x && (int)co -> y_new == (int)co->y) {
    return;
  }

  SDL_Delay(MILLISECONDDELAY);
  Neill_SDL_SetDrawColour(sw, rand()%SDL_8BITCOLOUR, rand()%SDL_8BITCOLOUR, rand()%SDL_8BITCOLOUR);
  SDL_RenderDrawLine(sw->renderer, co->y_new, co->x_new, co->y, co->x);
  Neill_SDL_UpdateScreen(sw);
}

void circle_tastic(SDL_Simplewin* sw, Program* p) {

  SDL_Delay(MILLISECONDDELAY);
  Neill_SDL_SetDrawColour(sw, rand()%SDL_8BITCOLOUR, rand()%SDL_8BITCOLOUR, rand()%SDL_8BITCOLOUR);
  Neill_SDL_RenderDrawCircle(sw->renderer, p->co->y_new, p->co->x_new, p->co->radius);
  Neill_SDL_UpdateScreen(sw);
}
/*############################## POLISH NOTATION STACK ################################*/

double Polish(Program* p, Stack* s)   {

  if(strcmp(p->code[p->cw], ";") == 0) {
    if (s->count == 1 ) {
      s -> count = 0;
      return s->result;
    }
    else {
      error("Incorrect format of Polish notation", p);         /* Else incorrect format */
    }
  }

  if (operator(p->code[p->cw], s)) {
      operate(s);
      increment(p);
  }

  else {
    Push(varnum(p->code[p->cw], p), s);
  }

  s -> result = Polish(p, s);
  return s -> result;
}

void init_stack(Stack* s)    {

  s -> op = 0;
  s -> result = -1;
  s -> count = 0;
  s -> top = (Item*)calloc(1, sizeof(Item));
  s -> top -> prev = NULL;
}

bool operator(char* string, Stack* s)   {

  if (strcmp(string, "*") == 0) {
    s -> op = MUL;
    return true;
  }
  if (strcmp(string, "/") == 0) {
    s -> op = DIV;
    return true;
  }
  if (strcmp(string, "+") == 0) {
    s -> op = ADD;
    return true;
  }
  if (strcmp(string, "-") == 0) {
    s -> op = SUB;
    return true;
  }
  return false;
}

void operate(Stack* s)  {

  double x, y;

    x = Pop(s);
    y = Pop(s);

  if (s -> op == MUL) {
    s -> result = x * y;
  }
  if (s->op == DIV) {
    s->result = y / x;
  }
  if (s->op == ADD) {
    s->result = x + y;
  }
  if (s->op == SUB) {
    s->result = y - x;
  }
  Push(s->result, s);

}

void Push(double value, Stack* s) {

  Item* v;
  v = (Item*)calloc(1, sizeof(Item));
  v -> x = value;
  v -> prev = s -> top;
  s -> top = v;
  s -> result = v -> x;
  s -> count++;
}

double Pop(Stack* s) {

  double val;
  Item* tmp;

  if (s->top == NULL) {
    printf("Nothing to pop- Polish notation\n");
    exit(2);
  }

  val = s -> top -> x;
  tmp = s -> top;
  s->top = s -> top -> prev;
  free(tmp);

  s->count--;
  return val;
}

/* ########################## TREE EXTENSION ###########################*/

Tree* init_tree(Coordinates* co) {

  Tree* t = malloc(sizeof(Tree));
  if (t == NULL) {
    exit(1);
  }
  t -> co = co;

  t->left = NULL;
  t->right = NULL;

  return t;
}

void insert(Tree* t, Program* p, double number) {

  if (t == NULL) {
  error("No tree\n", p);
  }

  if (t -> left == NULL && t -> right == NULL)
   {
       t -> left  = init_tree(copy(t->co));
       t-> left -> co-> angle -= M_PI/8;
       find_coord(t->left->co, number);
       if(p -> valgrind == false) {
         art_attack(&(p->sw), t -> co);
       }

       t -> right = init_tree(copy(t->co));
       t->right->co->angle += M_PI/8;
       find_coord(t->right->co, number);
       if(p -> valgrind == false) {
         art_attack(&(p->sw), t -> co);
       }

     return;
   }

  else {
   insert(t -> right, p, number);
   insert(t -> left, p, number);
   }

}

Coordinates* copy(Coordinates* old) {

  Coordinates* copy = (Coordinates*)malloc(sizeof(Coordinates));

    copy -> x_new = old -> x_new;
    copy -> y_new =   old -> y_new;
    copy -> x =   old -> x;
    copy -> y = old -> y;
    copy -> angle = old -> angle;
    return copy;
}

/*############################ FREE ALL #################################*/

void free_stack(Stack* s) {

  Item* temp = s-> top;

  while (s->top!= NULL){
    temp = s->top;
    s->top = temp -> prev;
    free(temp);
  }
}

void free_program(Program* p) {

Variable *temp;
temp = p ->vars;

  while (p->vars != NULL) {
    temp = p->vars;
    p -> vars = temp -> next;
    free(temp -> name);
    free(temp);
  }

  if (p->top != NULL) {
    free_tree(p->top);
  }
  free(p->co);
  free(p);
}

void free_tree(Tree* t) {

  if (t != NULL) {
    free_tree(t->right);
    if(t->co!= NULL){
      free(t->co);
    }
    free_tree(t->left);
    free(t);
   }
}

void end_program(SDL_Simplewin* sw)   {

  do {
    Neill_SDL_Events(sw);
  } while(!sw->finished);

  SDL_Quit();
}
