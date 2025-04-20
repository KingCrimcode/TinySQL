%{
#include "parser.hpp"
%}

%option noyywrap
%option nounput
%option noinput

%option yylineno

%%
CREATE                    return CREATE;
DROP                      return DROP;
ALTER                     return ALTER;

SELECT                    return SELECT;
INSERT                    return INSERT;
UPDATE                    return UPDATE;
DELETE                    return DELETE;

FROM                      return FROM;
INTO                      return INTO;
WHERE                     return WHERE;

DATABASE                  return DATABASE;
TABLE                     return TABLE;
INDEX                     return INDEX;

OR                        return OR;
AND                       return AND;
NOT                       return NOT;

[a-zA-Z_][a-zA-Z0-9_]*    { yylval.str = strdup(yytext); return IDENTIFIER; }
[0-9]+                    { yylval.num = atoi(yytext); return NUMBER; }
[ \t\n]+                  /* whitespace */
.                         { return *yytext; }
%%
