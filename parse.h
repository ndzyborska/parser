#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAXLINE 10
#define MAXLEGNTH 100
#define MAXMOVEMENT 1000
#define MILLISECONDDELAY 5
#define MAXTURN 360
#define STRAIGHT 3/2
#define CENTRE_X 300
#define CENTRE_Y 400
#define VAR_SZ 2
#define RADIAN_CONV M_PI/180

typedef enum bool {false, true} bool;

typedef struct Variable {

  char* name;
  double val;
  struct Variable* next;

} Variable;


typedef struct Program {

  int cw;
  int stack;
  bool polish;
  char code[MAXLEGNTH][MAXLINE];
  int line_index[MAXLEGNTH];
  Variable* vars;

} Program;


void testall();
Program* init_program();
void init_code(Program* p);
void insert_code(Program* p, FILE* fp);
void increment(Program* p);
bool string_match(Program* p, char* s);
void start(Program* p);
void end(Program* p);
void statement(Program* p);
void error(char* message, Program* p);
void FD(char* string, Program* p);
void RT(char* string, Program* p);
void LT(char* string, Program* p);
void DO(char* string, Program* p);
void SET(char* string, Program* p);
bool Polish(Program* p);
bool operator(char* string);
bool all_numbers(char* str);
double varnum(char* str, Program* p);
void create_var(Program* p, char* name, double value);
bool var_check(char* name, Program* p);
double var_retriever(char* name, Program* p);
void change_var(Program* p, char* name, double new_value);
bool valid_variable(char* str);
bool in_range(double number, double move_or_turn);
void free_program(Program* p);
