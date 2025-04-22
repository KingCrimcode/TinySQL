/*
** Praised be GNU magic
*/
%debug

%code requires{
#include <vector>
#include <string>

#include "ast.hpp"
}

%{
#include <iostream>

// extern int column;
extern int yylineno;

extern int yylex();
void yyerror(const char *err);
%}

%union {
int num;
char* str;
std::vector<std::string>* str_list;
SelectQuery* select_query;
}

// Token definitions
%token DATABASE
%token OR AND NOT //IS ISNOT MATCH BETWEEN IN ISNULL NOTNULL
%token CREATE DROP ALTER SELECT INSERT UPDATE DELETE FROM INTO WHERE TABLE INDEX
%token <str> IDENTIFIER
%token <num> NUMBER

// Precedence rules
%left OR
%left AND
%right NOT

// Type definitions
%type <str_list>        column_list
%type <select_query>    select_stmt


%%
cmd
  : select_stmt { std::cout << "Placeholder SELECT query text\n"; }
  ;

column_list
  : IDENTIFIER
  {
    $$ = new std::vector<std::string>();
    $$->push_back(std::string($1));
  }
  // list before identifier to easily build the vector with push backs
  // identifier before list means prepending, not appending, which is slower
  | column_list ',' IDENTIFIER
  {
    $1->push_back(std::string($3));
    $$ = $1;
  }
  ;

select_stmt
  : SELECT column_list FROM IDENTIFIER 
  {
    $$ = new SelectQuery(*$2, std::string($4));
    delete $2;
    delete $4;
  }
  ;   
%%

void yyerror(const char *err) {
  std::cerr << '\n' << err << " at line " << yylineno; // << ':' << column;
  // std::cerr << "Parsing error: " << err << '\n';
}
