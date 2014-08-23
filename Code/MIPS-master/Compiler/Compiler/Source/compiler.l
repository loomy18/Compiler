/*
=====================================================================
Flex Logic
=====================================================================
*/

%{
#include <stdio.h>
#include "compiler.tab.h"

extern void Management_LogError(char * message);
%}

%option noyywrap
%option nounput
%option noinput
%option yylineno

%%

[ \t\r\n]               ;

\#[^\n]*\n              ;

[a-z][a-zA-Z]*          {  return TOKEN_ID;            }

\@                      {  return TOKEN_ASSIGN;        }

-?[0-9]+\.[0-9]+        {  return TOKEN_FLOAT;         }

-?[0-9]+                {  return TOKEN_INTEGER;       }

\;                      {  return TOKEN_TERMINATE;     }

TRUE                    {  return TOKEN_TRUE;          }

FALSE                   {  return TOKEN_FALSE;         }

PRINT                   {  return TOKEN_PRINT;         }

PRINTL                  {  return TOKEN_PRINTL;        }

PRINTS                  {  return TOKEN_PRINTS;        }


\[                      {  return TOKEN_ARRAYBEGIN;    }

\]                      {  return TOKEN_ARRAYBREAK;    }
\(                      {  return TOKEN_TUPLEBEGIN;    }

\)                      {  return TOKEN_TUPLEBREAK;    }

\{                      {  return TOKEN_RSLNBEGIN;     }

\}                      {  return TOKEN_RSLNBREAK;     }


\?                      {  return TOKEN_IF;            }

\:                      {  return TOKEN_ELSE;          }

\$                      {  return TOKEN_WHILE;         }


\=                      {  return TOKEN_EQUAL;         }

\<                      {  return TOKEN_LESS;          }

\<\=                    {  return TOKEN_LESSOREQUAL;   }

\>                      {  return TOKEN_MORE;          }

\>\=                    {  return TOKEN_MOREOREQUAL;   }


\&                      {  return TOKEN_AND;           }

\|                      {  return TOKEN_OR;            }

\~                      {  return TOKEN_NOT;           }


\+                      {  return TOKEN_ADD;           }

\-                      {  return TOKEN_SUBTRACT;      }

\*                      {  return TOKEN_MULTIPLY;      }

\^                      {  return TOKEN_EXPONENTIATE;  }

\/                      {  return TOKEN_DIVIDE;        }

\%                      {  return TOKEN_MODULO;        }

.                       { Management_LogError("illegal character"); }

%%

/*
=====================================================================
EOF
=====================================================================
*/
