/*
** Praised be GNU magic
*/
%debug

%code requires{
  #include <memory>
  #include <string>
  #include <vector>

  #include "ast.hpp"
}

%{
  #include <iostream>

  #include "ast.hpp"

  AST::ASTNode* astRoot;

  // extern int column;
  extern int yylineno;

  extern int yylex();
  void yyerror(const char *err);
%}

%union {
  std::string* string;

  AST::Statement* statement;
  AST::Expr* expr;
  AST::SelectStatement* select_statement;
  std::vector<std::unique_ptr<AST::SelectExpr>>* select_expr_list;
  AST::SelectExpr* select_expr;
  std::vector<std::unique_ptr<AST::TableReference>>* table_references;
  AST::TableReference* table_reference;
  AST::TableFactor* table_factor;
}

%token <string> NAME STRING INTEGER FLOAT

%token ALTER
%token AND
%token CREATE
%token DATABASE
%token DELETE
%token DROP
%token FROM
%token INDEX
%token INSERT
%token INT
%token INTO
%token NOT
%token OR
%token SELECT
%token TABLE
%token UPDATE
%token VALUES
%token VARCHAR
%token WHERE


// Precedence rules
%left OR
%left AND
%right NOT

// Type definitions
// %type <str_list>        select_expr_list
// %type <select_query>    select_stmt

%type <statement> statement
%type <expr> expr
%type <select_statement> select_statement
%type <select_expr_list> select_expr_list
%type <select_expr> select_expr
%type <table_references> table_references
%type <table_reference> table_reference
%type <table_factor> table_factor


%%
statement
  : select_statement { 
    astRoot = $1;
  }
  ;

select_statement
  : SELECT select_expr_list FROM table_references {
    $$ = new AST::SelectStatement(std::move(*$2), std::move(*$4));
    delete $2;
    delete $4;
  }
  ;

select_expr_list
  : select_expr
  {
    $$ = new std::vector<std::unique_ptr<AST::SelectExpr>>();
    $$->push_back(std::unique_ptr<AST::SelectExpr>($1));
  }
  | select_expr_list ',' select_expr
  {
    $$ = $1;
    $$->push_back(std::unique_ptr<AST::SelectExpr>($3));
  }
  //| '*'
  //{
  //}
  ;

select_expr
  : expr // opt_as_alias (will implement, maybe)
  {
    $$ = new AST::SelectExpr(std::unique_ptr<AST::Expr>($1));
  }
  ;

table_references
  : table_reference
  {
    $$ = new std::vector<std::unique_ptr<AST::TableReference>>();
    $$->push_back(std::unique_ptr<AST::TableReference>($1));
  }
  | table_references ',' table_reference
  {
    $$ = $1;
    $$->push_back(std::unique_ptr<AST::TableReference>($3));
  }
  ;

table_reference
  : table_factor
  {
    $$ = $1;
  }
  // | join table (will implement, maybe)
  ;

table_factor
  : NAME
  {
    $$ = new AST::TableFactor(std::make_unique<AST::Identifier>(*$1));
    delete $1;
  }
  ;

expr
  : NAME {
    $$ = new AST::Identifier(*$1);
    delete $1;
  }
  // | NAME '.' NAME {}
  | STRING {
    $$ = new AST::StringLiteral(*$1);
    delete $1;
  }
  | INTEGER {
    $$ = new AST::IntegerLiteral(std::stoi(*$1));
    delete $1;
  }
  | FLOAT {
    $$ = new AST::FloatLiteral(std::stof(*$1));
    delete $1;
  }
  ;

%%

void yyerror(const char *err) {
  std::cerr << '\n' << err << " at line " << yylineno; // << ':' << column;
  // std::cerr << "Parsing error: " << err << '\n';
}
