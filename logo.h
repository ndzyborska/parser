#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "neillsdl2.h"

#define MAXLINE 30
#define MAXLEGNTH 100
#define MAXMOVEMENT 1000
#define MILLISECONDDELAY 5
#define MAXTURN 360
#define STRAIGHT 3/2
#define CENTRE_X 300
#define CENTRE_Y 400
#define VAR_SZ 2
#define RADIAN_CONV M_PI/180
#define MAXCOORD 800
#define ROW 3


typedef enum bool {false, true} bool;

enum operator {MUL = 1, DIV = 2, ADD = 3, SUB = 4};

typedef struct Variable {

  char* name;
  double val;
  struct Variable* next;

} Variable;

typedef struct Coordinates {

  double x_new;
  double y_new;
  double x;
  double y;
  double angle;
  double radius;

} Coordinates;

typedef struct Item {

  double x;
  struct Item* prev;

} Item;

typedef struct stack {

  Item* top;
  int count;
  double result;
  int op;

} Stack;

typedef struct Tree {

  Coordinates* co;
  struct Tree* left;
  struct Tree* right;

} Tree;

typedef struct Program {

  int cw;
  char** code;
  int line_index[MAXLEGNTH];
  Coordinates* co;
  Variable* vars;
  SDL_Simplewin sw;
  bool valgrind;
  Tree* top;

} Program;


void testall();
Program* init_program();
void init_code(Program* p);
void init_coord(Program* p);
void insert_code(Program* p, FILE* fp);
void increment(Program* p);
void error(char* message, Program* p);
bool string_match(Program* p, char* s);
void start(Program* p);
void end(Program* p);
void statement(Program* p);
void FD(char* string, Program* p);
void RT(char* string, Program* p);
void LT(char* string, Program* p);
void DO(char* string, Program* p);
void SET(char* string, Program* p);
void MAKE(char* string, Program* p);
void SPLIT(char* string, Program* p);
void CHANGE(Program* p);


bool all_numbers(char* str);
double varnum(char* str, Program* p);
void create_var(Program* p, char* name, double value);
bool var_check(char* name, Program* p);
double var_retriever(char* name, Program* p);
void change_var(Program* p, char* name, double new_value);
bool valid_variable(char* str);
bool in_range(double number, double move_or_turn);

void init_stack(Stack* s);
double Polish(Program* p, Stack* s);
bool operator(char* string, Stack* s);
void operate(Stack* s);
void Push(double value, Stack* s);
double Pop(Stack* s);

void find_coord(Coordinates* co, double number);
void art_attack(SDL_Simplewin* sw, Coordinates* co);
void circle_tastic(SDL_Simplewin* sw, Program* p);
void end_program(SDL_Simplewin* sw);

Tree* init_tree(Coordinates* co);
void insert(Tree* t, Program* p, double number);
Coordinates* copy(Coordinates* old);

void free_program(Program* p);
void free_stack(Stack* s);
void free_tree(Tree* t);
