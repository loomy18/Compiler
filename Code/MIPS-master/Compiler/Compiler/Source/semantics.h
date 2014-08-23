/*
=====================================================================
Semantics
=====================================================================
*/

#ifndef SEMANTICS_H_
#define SEMANTICS_H_

#include "management.h"

void      Action_Accept();

void      Action_Function(char * resultA, Result resultB);

void      Action_Procedure(Result resultA);

Result    Action_Print(Result resultA);

Result    Action_Routine(Result resultA);

Result    Action_IfElse(Result resultA, Result resultB, Result resultC);

Result    Action_If(Result resultA, Result resultB);

Result    Action_While(Result resultA, Result resultB);


Result    Action_Equal(Result resultA, Result resultB);

Result    Action_Less(Result resultA, Result resultB);

Result    Action_LessOrEqual(Result resultA, Result resultB);

Result    Action_More(Result resultA, Result resultB);

Result    Action_MoreOrEqual(Result resultA, Result resultB);


Result    Action_And(Result resultA, Result resultB);

Result    Action_Or(Result resultA, Result resultB);

Result    Action_Not(Result resultA);


Result    Action_Add(Result resultA, Result resultB);

Result    Action_Subtract(Result resultA, Result resultB);

Result    Action_Multiply(Result resultA, Result resultB);

Result    Action_Exponentiate(Result resultA, Result resultB);

Result    Action_Divide(Result resultA, Result resultB);

Result    Action_Modulo(Result resultA, Result resultB);

Result    Action_Negate(Result resultA);


Result    Action_Invoke(char * resultA);

Result    Action_Assign(char * resultA, Result resultB);

Result    Action_SelectIndex(char * resultA, Result resultB);

Result    Action_Select(char * resultA);


Result    Action_StoreFunction(Function resultA);

Result    Action_StoreTuple(Stack resultA);

Result    Action_StoreFloat(float resultA);

Result    Action_StoreInteger(int resultA);

Result    Action_StoreBoolean(int resultA);


Function  Action_ParseFunction(Stack resultA, Result resultB);

Stack     Action_ParseTuple(Result resultA); /*TODO: figure out args */


#endif

/*
=====================================================================
EOF
=====================================================================
*/


