%{
#include <string>
#include "parser.hpp"
%}

%option noyywrap
%option nounput
%option noinput

/* For line number in error message */
/* Could remove, no need to yet though */
%option yylineno

%%
ALTER                     return ALTER;
AND                       return AND;
CREATE                    return CREATE;
DATABASE                  return DATABASE;
DELETE                    return DELETE;
DROP                      return DROP;
FROM                      return FROM;
INDEX                     return INDEX;
INSERT                    return INSERT;
INT                       return INT;
INTO                      return INTO;
NOT                       return NOT;
OR                        return OR;
SELECT                    return SELECT;
TABLE                     return TABLE;
UPDATE                    return UPDATE;
VALUES                    return VALUES;
VARCHAR                   return VARCHAR;
WHERE                     return WHERE;

\'([^\'\n]*)\'            { yylval.string = new std::string(yytext, yyleng); return STRING; }
\"([^\"\n]*)\"            { yylval.string = new std::string(yytext, yyleng); return STRING; }

[0-9]+                    { yylval.string = new std::string(yytext, yyleng); return INTEGER; }
[0-9]+"."[0-9]+           { yylval.string = new std::string(yytext, yyleng); return FLOAT; }

[a-zA-Z_][a-zA-Z0-9_]*    { yylval.string = new std::string(yytext, yyleng); return NAME; }

#.*                       {}
"--"[ \t].*               {}

[ \t\n]+                  /* whitespace */
.                         { return *yytext; }
%%
