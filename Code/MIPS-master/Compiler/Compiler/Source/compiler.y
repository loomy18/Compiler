/*
=====================================================================
Bison Logic / Main
=====================================================================
*/

/* expose some bison globals */
%{
#include "semantics.h"
#define YYERROR_VERBOSE

extern int     yylex();        /* called to advance lexer       */

extern int     yyparse();      /* called to advance parser      */

extern char *  yytext;         /* current string token          */

extern int     yyleng;         /* current string token length   */

extern int     yylineno;       /* current source line number    */

extern FILE *  yyin;           /* current source stream handle  */
 
void yyerror(const char * s);
%}

%union 
{
  int                  ival;
  float                fval;
  char              *  sval;
  struct Stack_t    *  tuple;
  struct Function_t *  function;
  struct Result_t   *  result;
}

/*
---------------------------------------------------------------------
Types
---------------------------------------------------------------------
*/

%type <result>      expression
%type <result>      procedure
%type <result>      condition
%type <result>      invocation
%type <result>      relation
%type <result>      relationb
%type <result>      arithmetic
%type <result>      variant
%type <result>      print
%type <result>      resolution
%type <result>      assignment
%type <result>      sentence
%type <result>      value
%type <result>      array
%type <result>      literal
%type <fval>        float
%type <ival>        integer
%type <ival>        boolean
%type <sval>        identifier
%type <result>      assignarray

/*
---------------------------------------------------------------------
Tokens
---------------------------------------------------------------------
*/

%token <sval>     TOKEN_ID
%token            TOKEN_ASSIGN

/* literals */
%token <fval>     TOKEN_FLOAT
%token <ival>     TOKEN_INTEGER

%token <ival>     TOKEN_TRUE
%token <ival>     TOKEN_FALSE
%token            TOKEN_PRINT   /* TODO */
%token            TOKEN_PRINTL  /* TODO */
%token            TOKEN_PRINTS  /* TODO */

/* formatting */
%token            TOKEN_TUPLEBEGIN
%token            TOKEN_TUPLEBREAK
%token            TOKEN_ARRAYBEGIN
%token            TOKEN_ARRAYBREAK
%token            TOKEN_RSLNBEGIN
%token            TOKEN_RSLNBREAK

/* condition */
%token            TOKEN_IF
%token            TOKEN_ELSE
%token            TOKEN_WHILE

/* relation */
%token            TOKEN_EQUAL

%token            TOKEN_LESS
%token            TOKEN_LESSOREQUAL
%token            TOKEN_MORE
%token            TOKEN_MOREOREQUAL

/* relationb */
%token            TOKEN_AND
%token            TOKEN_OR
%token            TOKEN_NOT

/* arithmetic */
%token            TOKEN_ADD
%token            TOKEN_SUBTRACT
%token            TOKEN_MULTIPLY
%token            TOKEN_EXPONENTIATE
%token            TOKEN_DIVIDE
%token            TOKEN_MODULO

%token TOKEN_TERMINATE

/*
---------------------------------------------------------------------
Grammar
---------------------------------------------------------------------
*/
%%

program     :       procedure                                     { Action_Accept(); }
			; 

procedure   : sentence procedure         { Action_Procedure($1);        }
			| {}
			;

sentence     : assignment TOKEN_TERMINATE		{ $$ = $1;                          }
	       | assignarray TOKEN_TERMINATE		{ $$ = $1;                          }
	       | print TOKEN_TERMINATE			{ $$ = $1;                          }
	       //| function TOKEN_TERMINATE		{ $$ = $1;                          }
		| invocation TOKEN_TERMINATE		{ $$ = $1;                          }
		| condition  TOKEN_TERMINATE                              { $$ = $1;                          }
;

expression  :      relation                                      { $$ = $1;                          }
			| relationb                                     { $$ = $1;                          }
			| arithmetic                                    { $$ = $1;                          }
			| variant                                       { $$ = $1;                          }
			;

condition   : relation  TOKEN_IF procedure TOKEN_ELSE procedure   { $$ = Action_IfElse($1, $3, $5);   }
			| relation  TOKEN_IF procedure            { $$ = Action_If($1, $3);           }
			| relation  TOKEN_WHILE procedure        { $$ = Action_While($1, $3);        }
			;

relation    : expression TOKEN_EQUAL expression             { $$ = Action_Equal($1, $3);        }
			| expression TOKEN_LESS expression              { $$ = Action_Less($1, $3);         }
			| expression TOKEN_LESSOREQUAL expression       { $$ = Action_LessOrEqual($1, $3);  }
			| expression TOKEN_MORE expression              { $$ = Action_More($1, $3);         }
			| expression TOKEN_MOREOREQUAL expression       { $$ = Action_MoreOrEqual($1, $3);  }
			;

relationb   : expression TOKEN_AND expression               { $$ = Action_And($1, $3);          }
			| expression TOKEN_OR  expression               { $$ = Action_Or($1, $3);           }
			| TOKEN_NOT expression                          { $$ = Action_Not($2);              }
			;

arithmetic  : expression TOKEN_ADD expression               { $$ = Action_Add($1, $3);          }
			| expression TOKEN_SUBTRACT expression          { $$ = Action_Subtract($1, $3);     }
			| expression TOKEN_MULTIPLY expression			{ $$ = Action_Multiply($1, $3);     }
			| expression TOKEN_EXPONENTIATE expression      { $$ = Action_Exponentiate($1, $3); }
			| expression TOKEN_DIVIDE expression            { $$ = Action_Divide($1, $3);       }
			| expression TOKEN_MODULO expression            { $$ = Action_Modulo($1, $3);       }
			| TOKEN_SUBTRACT expression                     { $$ = Action_Negate($2);           }
			;

variant     : resolution                                    { $$ = $1;                          }
			| literal                                       { $$ = $1;                          }
			;

resolution  : TOKEN_RSLNBEGIN expression TOKEN_RSLNBREAK    { $$ = $2;                          }
			;

invocation  : identifier TOKEN_TUPLEBEGIN TOKEN_TUPLEBREAK  { $$ = Action_Invoke($1);           }
			;

assignarray : identifier TOKEN_ARRAYBEGIN integer TOKEN_ARRAYBREAK TOKEN_ASSIGN value       { $$ = Action_AssignToArray($1, $3, $6);}
;
assignment  : identifier TOKEN_ASSIGN value            { $$ = Action_Assign($1, $3);       }
	      |identifier TOKEN_ARRAYBEGIN TOKEN_ARRAYBREAK TOKEN_ASSIGN TOKEN_ARRAYBEGIN integer TOKEN_ARRAYBREAK       { $$ = Action_AssignArray($1, $6);       }
	     |identifier TOKEN_ASSIGN identifier TOKEN_ARRAYBEGIN integer TOKEN_ARRAYBREAK       { $$ = Action_AssignFromArray($1, $3, $5);       }
;

value       : literal		       { $$ = $1; }
;

array : TOKEN_ARRAYBEGIN integer TOKEN_ARRAYBREAK       { $$ = Action_StoreArray($2);}
;


	
print       : TOKEN_PRINT expression                        { $$ = Action_Print($2);            }
;


literal     : 		  float                                         { $$ = Action_StoreFloat($1);       }
			| integer                                       { $$ = Action_StoreInteger($1);     }
			| boolean                                       { $$ = Action_StoreBoolean($1);     }
			;

float       : TOKEN_FLOAT                                   { $$ = atof(yytext);                      }
			;

integer     : TOKEN_INTEGER                                 { $$ = atoi(yytext);                      }
			;

boolean     : TOKEN_TRUE                                    { $$ = 1;                         }
			| TOKEN_FALSE                                   { $$ = 0;                         }
			;

identifier  : TOKEN_ID                                      { $$ = strdup(yytext);                      }
			;
%%

/*
---------------------------------------------------------------------
Parsing Callbacks
---------------------------------------------------------------------
*/
void yyerror(const char * s) 
{
	//TODO: add send this to management
	//Management_LogError((char*)s);

	printf("ERROR ( %i | %s ): %s\n",
		yylineno, yytext, s);
}

/*
*********************************************************************
Main

Arg 1 = Input File
Arg 2 = Output File
Arg 3 = Listing File (Optional)
*********************************************************************
*/
int main(int argc, char ** args) 
{
	int state = 0;
	if(0 == Management_Initiate())
	{
		if(argc > 1)
		{
			FILE * inputFile = fopen(args[1], "r");
			if(inputFile)
			{
				yyin = inputFile;
				if(argc > 2)
				{
					FILE * outputFile = fopen(args[2], "w+");
					if(outputFile)
					{
						FILE * listingFile = NULL;
						if(argc > 3)
						{
							if((listingFile = fopen(args[3], "w+")))
							{
								Management_LogStream(listingFile);
							} else {
								state = -6;
								printf("ERROR - Main[%i]: \
									file IO exception\n", state);
							}
						}

						if(state == 0)
						{
							do { yyparse(); }
								while( !feof(yyin) );

							Management_Serialize(outputFile);
						}
						if(listingFile)
						{
							fclose(listingFile);
						}
					} else {
						state = -5;
						printf("ERROR - Main[%i]: \
							file IO exception\n", state);
					}
					if(outputFile)
					{
						fclose(outputFile);
					}
				} else {
					state = -4;
					printf("ERROR - Main[%i]: \
						missing 2nd argument\n", state);
				}
				if(inputFile)
				{
					fclose(inputFile);
				}
			} else {
				state = -3;
				printf("ERROR - Main[%i]: \
					file IO exception\n", state);
				}
		} else {
			state = -2;
			printf("ERROR - Main[%i]: \
				missing 1st argument\n", state);
		}
		Management_Terminate();
	} else {
		state = -1;
		printf("ERROR - Main[%i]: \
			management failure\n", state);
	}

	return state;
}

/*
=====================================================================
EOF
=====================================================================
*/

