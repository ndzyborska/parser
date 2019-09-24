#include "parse.h"


void test_init_program(FILE* test, Program* tp);
void test_insert_code(FILE* test, Program* tp);
void test_string_match(FILE* report, Program* tp);
void test_create_var(FILE* report, Program* tp);
void test_valid_variable(FILE* report);
void test_varnum(FILE* report, Program* tp);
void test_in_range(FILE* report);
void test_all_numbers(FILE* report);
void test_change_var(FILE* report, Program* tp);
void test_var_retriever(FILE* report, Program* tp);
void test_var_check(FILE* report, Program* tp);
void test_operator(FILE* report);
void test_Polish(FILE* report, FILE* polish, Program* tp);



void testall(int argc, char** argv) {

  Program* tp = init_program();
  FILE* fp;
  char* polish;

  FILE* report = fopen("testfile.txt", "w");
     if (!report){
       printf("Cannot open test file\n");
       exit(1);
     }

  test_init_program(report, tp);
  test_insert_code(report, tp);
  test_string_match(report, tp);
  test_valid_variable(report);
  test_in_range(report);
  test_all_numbers(report);
  test_create_var(report, tp);
  test_varnum(report, tp);
  test_change_var(report, tp);
  test_var_retriever(report, tp);
  test_var_check(report, tp);
  test_operator(report);

  free_program(tp);

  if (argc > 3) {
    polish = argv[3];
      tp = init_program();
      fp = fopen(polish, "r");
           if (!fp){
             printf("Cannot open test file\n");
             exit(1);
           }

      test_Polish(report, fp, tp);
      fclose(fp);
  }

  fclose(report);

}

void test_init_program(FILE* report, Program* tp) {

  if (tp->cw == 0) {
    if (tp->vars == NULL) {
      if (tp->code[5][0] == '\0' &&
          tp->code[0][0] == '\0' &&
          tp->code[MAXLEGNTH-1][0] == '\0') {
            fprintf(report, "init_program check: Complete\n");
            return;
      }
    }
  }

  fprintf(report, "init_program check: Failed\n");
}


void test_insert_code(FILE* report, Program* tp) {

  FILE* test = fopen("test.txt", "r");
     if (!test){
       printf("Cannot open test file\n");
       exit(1);
     }

  insert_code(tp, test);
  fclose(test);

  if (tp->cw == 0) {
    if (string_match(tp, "Shrink") == true) {
      if (string_match(tp, "down") == true) {
        if (string_match(tp, "to") == true) {
          if(tp->cw == 3) {
            tp->cw += 6;
            if (string_match(tp, "ears")==true) {
              fprintf(report, "insert_code check: Complete\n");
              return;
            }
          }
        }
      }
    }
  }
  fprintf(report, "insert_code check: Failed\n");
}

void test_string_match(FILE* report, Program* tp) {

  int n = tp->cw = 0;
   if (string_match(tp, "Shrink") == true
          &&    strcmp(tp->code[0], "Shrink") == 0)  {
     if (n + 1 == tp->cw) {
      fprintf(report, "string_match check: Complete\n");
      return;
     }
   }
  fprintf(report, "string_match check: Failed\n");
}

void test_valid_variable(FILE* report)  {

  if (valid_variable("A")==true) {
    if (valid_variable("Z")==true) {
      if (valid_variable("e")==false)  {
        if (valid_variable(".")==false)  {
          if (valid_variable("AZ")==false)  {
            fprintf(report, "valid_varible check Complete\n");
            return;
          }
        }
      }
    }
  }
  fprintf(report, "valid_varible check Failed\n");

}

void test_in_range(FILE* report) {
  if (in_range(360, MAXTURN)) {
    if (in_range(0, MAXTURN)) {
      if (!in_range(400, MAXTURN)) {
        if (in_range(1000, MAXMOVEMENT)) {
          if (!in_range(-6, MAXTURN)) {
            fprintf(report, "in_range check Complete\n");
            return;
          }
        }
      }
    }
  }
  fprintf(report, "in_range check Failed\n");

}
void test_all_numbers(FILE* report) {
  if(all_numbers("45")) {
    if(!all_numbers("4r")) {
      if(!all_numbers("whr")) {
        if(all_numbers("4568")) {
          if(!all_numbers("-4546513")) {
            if(all_numbers("3")) {
              fprintf(report, "all_numbers check Complete\n");
              return;
            }
          }
        }
      }
    }
  }
  fprintf(report, "all_numbers check Failed\n");

}

void test_create_var(FILE* report, Program* tp) {

  create_var(tp, "A", 65);
   if (strcmp(tp->vars->name, "A") == 0) {
     if ((int)tp->vars->val == 65) {
       create_var(tp, "B", 70);
        if (strcmp(tp->vars->name, "B") == 0) {
          if ((int)tp->vars->val == 70) {
           if (strcmp(tp->vars->next->name, "A") == 0) {
            if ((int)tp->vars->next->val == 65) {
              fprintf(report, "Create_var test Complete\n");
              return;
            }
           }
         }
       }
     }
  }
  fprintf(report, "Create_var test Failed");
}

void test_varnum(FILE* report, Program* tp)  {

  if ((int)varnum("12", tp) == 12) {
    if ((int)varnum("A", tp) == 65) {
      if ((int)varnum("B", tp) == 70) {
        if ((int)varnum("166", tp) == 166) {
          if ((int)varnum("0", tp) == 0) {
            fprintf(report, "varnum test Complete\n");
            return;
          }
        }
      }
    }
  }

  fprintf(report, "varnum test Failed\n");
}

void test_change_var(FILE* report, Program* tp) {

  change_var(tp, "B", 35);
   if (strcmp(tp->vars->name, "B") == 0) {
     if ((int)tp->vars->val == 35) {
       create_var(tp, "B", 70);
        if (strcmp(tp->vars->name, "B") == 0) {
          if ((int)tp->vars->val == 70) {
            create_var(tp, "A", 3);

           if (strcmp(tp->vars->next->name, "A") == 0) {
            if ((int)tp->vars->next->val == 3)
            create_var(tp, "C", 4);

             if (var_check("B", tp)) {
              fprintf(report, "Change_var test Complete\n");
              return;
            }
          }
        }
       }
      }
    }
  fprintf(report, "Change_var test Failed\n");
}

void test_var_retriever(FILE* report, Program* tp) {

  if ((int)var_retriever("A", tp) == 3) {
    if ((int)var_retriever("B", tp) == 70) {
      if ((int)var_retriever("C", tp) == 4) {
        create_var(tp, "F", 64);
        if ((int)var_retriever("F", tp) == 64) {
          fprintf(report, "var_retriever test Complete\n");
          return;
        }
      }
    }
  }
  fprintf(report, "var_retriever test Failed\n");

}

void test_var_check(FILE* report, Program* tp) {

  if (var_check("A", tp))   {
    if (var_check("B", tp))   {
      if (var_check("F", tp))   {
        if (var_check("C", tp))   {
          if (!var_check("Z", tp))   {
            if (!var_check("P", tp))   {
              fprintf(report, "check_var test Complete\n");
              return;
            }
          }
        }
      }
    }
  }
  fprintf(report, "check_var test Failed\n");

}

void test_operator(FILE* report) {

  if (operator("+"))   {
    if (operator("*"))   {
      if (!operator("f"))   {
        if (!operator("&"))   {
          if (operator("/"))   {
            fprintf(report, "operator test Complete\n");
            return;
          }
        }
      }
   }
  }
fprintf(report, "operator test Failed\n");

}

void test_Polish(FILE* report, FILE* polish, Program* tp) {

  insert_code(tp, polish);
  fclose(polish);

  if (Polish(tp) == true) {
    fprintf(report, "Polish test Complete\n");
  }
  else {
    fprintf(report, "Polish test Failed\n");
  }
  free_program(tp);

}
