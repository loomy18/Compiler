/*
=====================================================================
Semantics
=====================================================================
*/

#include "semantics.h"

/*
---------------
CastFloatToInteger
---------------
*/
int ConvertFloatToInteger(Result resultf)
{
	//Management_LogError("(float->integer) cast not implemented");

	int resultIndex = Management_GetRegister();

	/* append instructions to result */
	Procedure_PushOperands(resultf->procedure,
		/* label */ NULL,
		/* code  */ "cvt.w.s",
		/* argA  */ Management_GetRegisterNamef(resultf->index),
		/* argB  */ Management_GetRegisterNamef(resultf->index),
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultf->procedure,
		/* label */ NULL,
		/* code  */ "mfc1",
		/* argA  */ Management_GetRegisterName(resultIndex),
		/* argB  */ Management_GetRegisterNamef(resultf->index),
		/* argC  */ NULL
		);

	Management_FreeRegisterf(resultf->index);
	resultf->index = resultIndex;
	resultf->type = TYPE_INTEGER;

	return 0;;
}

/*
---------------
CastIntegerToFloat
---------------
*/
int ConvertIntegerToFloat(Result resulti)
{
	//Management_LogError("(integer->float) cast not implemented");

	int resultIndex = Management_GetRegister();

	/* append instructions to result */
	Procedure_PushOperands(resulti->procedure,
		/* label */ NULL,
		/* code  */ "mfc1",
		/* argA  */ Management_GetRegisterName(resultIndex),
		/* argB  */ Management_GetRegisterName(resulti->index),
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resulti->procedure,
		/* label */ NULL,
		/* code  */ "cvt.s.w",
		/* argA  */ Management_GetRegisterNamef(resulti->index),
		/* argB  */ Management_GetRegisterName(resulti->index),
		/* argC  */ NULL
		);

	Management_FreeRegister(resulti->index);
	resulti->index = resultIndex;
	resulti->type = TYPE_FLOAT;

	return 0;
}

/*
---------------
ImplicitCast

resultA->type = resultA->type
resultB->type = (resultA->type) resultB->type
---------------
*/
int ImplicitCast(Result resultA, Result resultB)
{
	if (resultA && resultB)
	{
		switch (resultA->type)
		{
		case TYPE_FUNCTION: {
			switch (resultB->type)
			{
				/* function = function */
			case TYPE_FUNCTION: {
				/* OK */
				return 0;
			} break;

				/* function = tuple */
			case TYPE_TUPLE: {
				Management_LogError("cannot cast tuple to function");
			} break;

				/* function = float */
			case TYPE_FLOAT: {
				Management_LogError("cannot cast float to function");
			} break;

				/* function = integer */
			case TYPE_INTEGER: {
				Management_LogError("cannot cast integer to function");
			} break;

				/* function = boolean */
			case TYPE_BOOLEAN: {
				Management_LogError("cannot cast boolean to function");
			} break;
			}

		} break;

		case TYPE_TUPLE: {
			switch (resultB->type)
			{
				/* tuple = function */
			case TYPE_FUNCTION: {
				Management_LogError("cannot cast function to tuple");
			} break;

				/* tuple = tuple */
			case TYPE_TUPLE: {
				/* OK */
				return 0;
			} break;

				/* tuple = float */
			case TYPE_FLOAT: {
				Management_LogError("cannot cast float to tuple");
			} break;

				/* tuple = integer */
			case TYPE_INTEGER: {
				Management_LogError("cannot cast integer to tuple");
			} break;

				/* tuple = boolean */
			case TYPE_BOOLEAN: {
				Management_LogError("cannot cast boolean to tuple");
			} break;
			}

		} break;

		case TYPE_FLOAT: {
			switch (resultB->type)
			{
				/* float = function */
			case TYPE_FUNCTION: {
				Management_LogError("cannot cast function to float");
			} break;

				/* float = tuple */
			case TYPE_TUPLE: {
				Management_LogError("cannot cast tuple to float");
			} break;

				/* float = float */
			case TYPE_FLOAT: {
				/* OK */
				return 0;
			} break;

				/* float = integer */
			case TYPE_INTEGER: {
				return ConvertIntegerToFloat(resultB);
			} break;

				/* float = boolean */
			case TYPE_BOOLEAN: {
				Management_LogError("cannot cast boolean to float");
			} break;
			}

		} break;

		case TYPE_INTEGER: {
			switch (resultB->type)
			{
				/* integer = function */
			case TYPE_FUNCTION: {
				Management_LogError("cannot cast function to integer");
			} break;

				/* integer = tuple */
			case TYPE_TUPLE: {
				Management_LogError("cannot cast tuple to integer");
			} break;

				/* integer = float */
			case TYPE_FLOAT: {
				Management_LogWarning("possible data loss from cast");
				return ConvertFloatToInteger(resultB);
			} break;

				/* integer = integer */
			case TYPE_INTEGER: {
				/* OK */
				return 0;
			} break;

				/* integer = boolean */
			case TYPE_BOOLEAN: {
				Management_LogError("cannot cast boolean to integer");
			} break;
			}

		} break;

		case TYPE_BOOLEAN: {
			switch (resultB->type)
			{
				/* boolean = function */
			case TYPE_FUNCTION: {
				Management_LogError("cannot cast function to boolean");
			} break;

				/* boolean = tuple */
			case TYPE_TUPLE: {
				Management_LogError("cannot cast tuple to boolean");
			} break;

				/* boolean = float */
			case TYPE_FLOAT: {
				Management_LogError("cannot cast float to boolean");
			} break;

				/* boolean = integer */
			case TYPE_INTEGER: {
				Management_LogError("cannot cast integer to boolean");
			} break;

				/* boolean = boolean */
			case TYPE_BOOLEAN: {
				/* OK */
				return 0;
			} break;
			}
		} break;
		}
		return -2;
	}
	return -1;
}

/*
---------------
Action_Accept
---------------
*/
void Action_Accept()
{
	Management_LogSemantic("accept");

	/* inline, optimize, cache */
	Management_Finalize();
}

/*
---------------
Action_Function
---------------
*/
void Action_Function(char * resultA, Result resultB)
{
	Management_LogSemantic("function");

	/* append instructions to result */
	Procedure_PushOperands(resultB->procedure,
		/* label */ NULL,
		/* code  */ "jr",
		/* argA  */ "$ra",
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	Management_PushFunction(resultA, resultB);
}

/*
---------------
Action_Routine
---------------
*/
Result Action_Routine(Result resultA)
{
	return resultA;
}


/*
---------------
Action_Procedure
---------------
*/
void Action_Procedure(Result resultA)
{
	/* append procedure to program */
	Management_LogSemantic("procedure");
	if(resultA->procedure)Management_MergeProcedure(resultA->procedure);

	/* clean up */
	//TODO: delete result
}

Result Action_Print(Result resultA)
{
	///* append instructions to result */
	//Procedure_PushOperands(resultA->procedure,
	//	/* label */ NULL,
	//	/* code  */ "li",
	//	/* argA  */ "$v0",
	//	/* argB  */ "1",
	//	/* argC  */ NULL
	//	);

	///* append instructions to result */
	//Procedure_PushOperands(resultA->procedure,
	//	/* label */ NULL,
	//	/* code  */ "move",
	//	/* argA  */ "$a0",
	//	/* argB  */ Management_GetRegisterName(resultA->index),
	//	/* argC  */ NULL
	//	);

	///* append instructions to result */
	//Procedure_PushOperands(resultA->procedure,
	//	/* label */ NULL,
	//	/* code  */ "syscall",
	//	/* argA  */ NULL,
	//	/* argB  */ NULL,
	//	/* argC  */ NULL
	//	);

	///* append instructions to result */
	//Procedure_PushOperands(resultA->procedure,
	//	/* label */ NULL,
	//	/* code  */ "syscall",
	//	/* argA  */ NULL,
	//	/* argB  */ NULL,
	//	/* argC  */ NULL
	//	);

	///* append instructions to result */
	//Procedure_PushOperands(resultB->procedure,
	//	/* label */ NULL,
	//	/* code  */ "syscall",
	//	/* argA  */ NULL,
	//	/* argB  */ NULL,
	//	/* argC  */ NULL
	//	);

	///* append instructions to result */
	//Procedure_PushOperands(resultB->procedure,
	//	/* label */ NULL,
	//	/* code  */ "syscall",
	//	/* argA  */ NULL,
	//	/* argB  */ NULL,
	//	/* argC  */ NULL
	//	);



	//AppendSeq(code, GenInstr(NULL, "li", "$v0", "4", NULL));
	//AppendSeq(code, GenInstr(NULL, "la", "$a0", "_nl", NULL));
	//AppendSeq(code, GenInstr(NULL, "syscall", NULL, NULL, NULL));

	//return resultA;
}

/*
---------------------------------------------------------------------
Condition
---------------------------------------------------------------------
*/

/*
---------------
Action_IfElse
---------------
*/
Result Action_IfElse(Result resultA, Result resultB, Result resultC)
{
	Management_LogSemantic("ifelse");

	int resultIndex = Management_GetRegister();
	int tmpreg = Management_GetRegister();
	char * jmplbla = strdup(Management_GetUniqueLabel());
	char * jmplblb = strdup(Management_GetUniqueLabel());

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "li",
		/* argA  */ Management_GetRegisterName(tmpreg),
		/* argB  */ "1",
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "bne",
		/* argA  */ Management_GetRegisterName(resultA->index),
		/* argB  */ Management_GetRegisterName(tmpreg),
		/* argC  */ jmplbla
		);

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultB->procedure
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "j",
		/* argA  */ jmplblb,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ jmplbla,
		/* code  */ NULL,
		/* argA  */ NULL,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultC->procedure
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ jmplblb,
		/* code  */ NULL,
		/* argA  */ NULL,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* clean up */
	Management_FreeRegister(tmpreg);
	free(jmplbla);
	free(jmplblb);

	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegister(resultB->index);
	Result_Destroy(&resultB);

	Management_FreeRegister(resultC->index);
	Result_Destroy(&resultC);

	return resultA;
}

/*
---------------
Action_If
---------------
*/
Result Action_If(Result resultA, Result resultB)
{
	Management_LogSemantic("if");

	int resultIndex = Management_GetRegister();
	int tmpreg = Management_GetRegister();
	char * jmplbl = strdup(Management_GetUniqueLabel());

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "li",
		/* argA  */ Management_GetRegisterName(tmpreg),
		/* argB  */ "1",
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "bne",
		/* argA  */ Management_GetRegisterName(resultA->index),
		/* argB  */ Management_GetRegisterName(tmpreg),
		/* argC  */ jmplbl
		);

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultB->procedure
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ jmplbl,
		/* code  */ NULL,
		/* argA  */ NULL,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* clean up */
	Management_FreeRegister(tmpreg);
	free(jmplbl);

	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegister(resultB->index);
	Result_Destroy(&resultB);

	return resultA;
}

/*
---------------
Action_While
---------------
*/
Result Action_While(Result resultA, Result resultB)
{
	Management_LogSemantic("while");

	int resultIndex = Management_GetRegister();
	int tmpreg = Management_GetRegister();
	char * jmplbla = strdup(Management_GetUniqueLabel());
	char * jmplblb = strdup(Management_GetUniqueLabel());

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "li",
		/* argA  */ Management_GetRegisterName(tmpreg),
		/* argB  */ "1",
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ jmplbla,
		/* code  */ NULL,
		/* argA  */ NULL,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "bne",
		/* argA  */ Management_GetRegisterName(resultA->index),
		/* argB  */ Management_GetRegisterName(tmpreg),
		/* argC  */ jmplblb
		);

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultB->procedure
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ "j",
		/* argA  */ jmplbla,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ jmplblb,
		/* code  */ NULL,
		/* argA  */ NULL,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	/* clean up */
	Management_FreeRegister(tmpreg);
	free(jmplbla);
	free(jmplblb);

	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegister(resultB->index);
	Result_Destroy(&resultB);

	return resultA;
}


/*
---------------------------------------------------------------------
Relation
---------------------------------------------------------------------
*/

/*
---------------
Action_Relation
---------------
*/
Result Action_Relation_(Result resultA, const char * op, Result resultC)
{
	char opcode[8] = "";

	int resultIndex = Management_GetRegister();

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultC->procedure
		);

	/* handle operation type */
	switch (op[0])
	{
		/* if equal... */
	case '=': {
		strcpy(opcode, "seq");
	} break;

	case '>': {
		/* if more or equal... */
		if (op[1] == '=')
		{
			strcpy(opcode, "sge");
		}
		/* if more... */
		else {
			strcpy(opcode, "sgt");
		}
	} break;
	case '<': {
		/* if less or equal... */
		if (op[1] == '=')
		{
			strcpy(opcode, "sle");
		}
		/* if less... */
		else {
			strcpy(opcode, "slt");
		}
	} break;
	}

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ opcode,
		/* argA  */ Management_GetRegisterName(resultIndex),
		/* argB  */ Management_GetRegisterName(resultA->index),
		/* argC  */ Management_GetRegisterName(resultC->index)
		);

	/* clean up */
	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegister(resultC->index);
	Result_Destroy(&resultC);

	return resultA;
}

Result Action_Relationf(Result resultA, const char * op, Result resultC)
{
	char opcode[8] = "";

	int resultIndex = Management_GetRegister();

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultC->procedure
		);

	/* handle operation type */
	switch (op[0])
	{
		/* if equal... */
	case '=': {
		strcpy(opcode, "seq.s");
	} break;

	case '>': {
		/* if more or equal... */
		if (op[1] == '=')
		{
			strcpy(opcode, "sge.s");
		}
		/* if more... */
		else {
			strcpy(opcode, "sgt.s");
		}
	} break;
	case '<': {
		/* if less or equal... */
		if (op[1] == '=')
		{
			strcpy(opcode, "sle.s");
		}
		/* if less... */
		else {
			strcpy(opcode, "slt.s");
		}
	} break;
	}

	/* append instructions to result */
	Procedure_PushOperands(resultA->procedure,
		/* label */ NULL,
		/* code  */ opcode,
		/* argA  */ Management_GetRegisterNamef(resultIndex),
		/* argB  */ Management_GetRegisterNamef(resultA->index),
		/* argC  */ Management_GetRegisterNamef(resultC->index)
		);

	/* clean up */
	Management_FreeRegisterf(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegisterf(resultC->index);
	Result_Destroy(&resultC);

	return resultA;
}

Result Action_Relation(Result resultA, const char * op, Result resultC)
{
	/* verify leftwise type for semantic */
	switch (resultA->type)
	{
	case TYPE_FLOAT: {
		if (0 == ImplicitCast(resultA, resultC))
		{
			return Action_Relationf(resultA, op, resultC);
		}
	} break;

	case TYPE_INTEGER: {
		if (0 == ImplicitCast(resultA, resultC))
		{
			return Action_Relation_(resultA, op, resultC);
		}
	} break;

	case TYPE_BOOLEAN: {
		if (0 == ImplicitCast(resultA, resultC))
		{
			return Action_Relation_(resultA, op, resultC);
		}
	} break;

	default: {
		Management_LogError("invalid leftwise type for relation");
	} break;
	}

	return NULL;
}

/*
---------------
Action_Equal
---------------
*/
Result Action_Equal(Result resultA, Result resultB)
{
	Management_LogSemantic("equal");
	return Action_Relation(resultA, "=", resultB);
}

/*
---------------
Action_Less
---------------
*/
Result Action_Less(Result resultA, Result resultB)
{
	Management_LogSemantic("less");
	return Action_Relation(resultA, "< ", resultB);
}

/*
---------------
Action_LessOrEqual
---------------
*/
Result Action_LessOrEqual(Result resultA, Result resultB)
{
	Management_LogSemantic("lessorequal");
	return Action_Relation(resultA, "<=", resultB);
}

/*
---------------
Action_More
---------------
*/
Result Action_More(Result resultA, Result resultB)
{
	Management_LogSemantic("more");
	return Action_Relation(resultA, "> ", resultB);
}

/*
---------------
Action_MoreOrEqual
---------------
*/
Result Action_MoreOrEqual(Result resultA, Result resultB)
{
	Management_LogSemantic("moreorequal");
	return Action_Relation(resultA, ">=", resultB);
}

/*
---------------------------------------------------------------------
Relation Bitwise
---------------------------------------------------------------------
*/

Result Action_Relationb_(Result resultA, char op, Result resultC)
{
	char opcode[8] = "";

	int resultIndex = Management_GetRegister();

	/* assume binary operation*/
	if (NULL != resultC)
	{
		/* leftwise merge of instructions */
		Procedure_Merge(
			resultA->procedure,
			resultC->procedure
			);

		/* handle operation type */
		switch (op)
		{
			/* if and... */
		case '&': {
			strcpy(opcode, "and");
		} break;

			/* if or... */
		case '|': {
			strcpy(opcode, "or");
		} break;
		}

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ opcode,
			/* argA  */ Management_GetRegisterName(resultIndex),
			/* argB  */ Management_GetRegisterName(resultA->index),
			/* argC  */ Management_GetRegisterName(resultC->index)
			);

		/* clean up */
		Management_FreeRegister(resultC->index);
		Result_Destroy(&resultC);
	}

	/* assume unary operation*/
	else
	{
		/* handle operation type */
		switch (op)
		{
			/* if not... */
		case '~': {
			strcpy(opcode, "not");
		} break;
		}

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ opcode,
			/* argA  */ Management_GetRegisterName(resultIndex),
			/* argB  */ Management_GetRegisterName(resultA->index),
			/* argC  */ NULL
			);
	}

	/* clean up */
	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	return resultA;
}

Result Action_Relationb(Result resultA, char op, Result resultC)
{
	/* verify leftwise type for semantic */
	switch (resultA->type)
	{
	case TYPE_BOOLEAN: {
		int state = ImplicitCast(resultA, resultC);
		if (0 == state || -1 == state)
		{
			return Action_Relationb_(resultA, op, resultC);
		}
	} break;

	default: {
		Management_LogError("invalid leftwise type for bitwise relation");
	} break;
	}

	return NULL;
}

/*
---------------
Action_And
---------------
*/
Result Action_And(Result resultA, Result resultB)
{
	Management_LogSemantic("and");
	return Action_Relationb(resultA, '&', resultB);
}

/*
---------------
Action_Or
---------------
*/
Result Action_Or(Result resultA, Result resultB)
{
	Management_LogSemantic("or");
	return Action_Relationb(resultA, '|', resultB);
}

/*
---------------
Action_Not
---------------
*/
Result Action_Not(Result resultA)
{
	Management_LogSemantic("not");
	return Action_Relationb(resultA, '~', NULL);
}

/*
---------------------------------------------------------------------
Arithmetic
---------------------------------------------------------------------
*/

/*
---------------
Action_Arithmetic
---------------
*/
Result Action_Arithmetic_(Result resultA, char op, Result resultC)
{
	int state = 0;

	char opcode[8] = "";

	int resultIndex = Management_GetRegister();

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultC->procedure
		);

	/* handle operation type */
	switch (op)
	{
		/* if addition... */
	case '+': {
		strcpy(opcode, "add");
	} break;

		/* if subtraction... */
	case '-': {
		strcpy(opcode, "sub");
	} break;

		/* if multiplication... */
	case '*': {
		strcpy(opcode, "mul");
	} break;

		/* if division... */
	case '/': {
		strcpy(opcode, "div");
	} break;

		/* if modulo... */
	case '%': {
		strcpy(opcode, "rem");
	} break;

		/* if exponential... */
	case '^': { state = 1;
		//TODO

	} break;

		/* if negation... */
	case '~': { state = 1;

		int reg = Management_GetRegister();

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ "li",
			/* argA  */ Management_GetRegisterName(reg),
			/* argB  */ "-1",
			/* argC  */ NULL
			);

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ "mul",
			/* argA  */ Management_GetRegisterName(resultIndex),
			/* argB  */ Management_GetRegisterName(resultA->index),
			/* argC  */ Management_GetRegisterName(reg)
			);

		Management_FreeRegister(reg);

	} break;
	}

	if (0 == state)
	{
		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ opcode,
			/* argA  */ Management_GetRegisterName(resultIndex),
			/* argB  */ Management_GetRegisterName(resultA->index),
			/* argC  */ Management_GetRegisterName(resultC->index)
			);
	}

	/* clean up */
	Management_FreeRegister(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegister(resultC->index);
	Result_Destroy(&resultC);

	return resultA;
}

Result Action_Arithmeticf(Result resultA, char op, Result resultC)
{
	int state = 0;

	char opcode[8] = "";

	int resultIndex = Management_GetRegisterf();

	/* leftwise merge of instructions */
	Procedure_Merge(
		resultA->procedure,
		resultC->procedure
		);

	/* handle operation type */
	switch (op)
	{
		/* if addition... */
	case '+': {
		strcpy(opcode, "add.s");
	} break;

		/* if subtraction... */
	case '-': {
		strcpy(opcode, "sub.s");
	} break;

		/* if multiplication... */
	case '*': {
		strcpy(opcode, "mul.s");
	} break;

		/* if division... */
	case '/': {
		strcpy(opcode, "div.s");
	} break;

		/* if modulus... */
	case '%': { state = 1;
		//TODO
	} break;

		/* if exponential... */
	case '^': { state = 1;
		//TODO

	} break;
		/* if negation... */
	case '~': { state = 1;

		int reg = Management_GetRegister();

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ "li.s",
			/* argA  */ Management_GetRegisterNamef(reg),
			/* argB  */ "-1",
			/* argC  */ NULL
			);

		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ "mul.s",
			/* argA  */ Management_GetRegisterNamef(resultIndex),
			/* argB  */ Management_GetRegisterNamef(resultA->index),
			/* argC  */ Management_GetRegisterNamef(reg)
			);

		Management_FreeRegisterf(reg);

	} break;
	}

	if (0 == state)
	{
		/* append instructions to result */
		Procedure_PushOperands(resultA->procedure,
			/* label */ NULL,
			/* code  */ opcode,
			/* argA  */ Management_GetRegisterNamef(resultIndex),
			/* argB  */ Management_GetRegisterNamef(resultA->index),
			/* argC  */ Management_GetRegisterNamef(resultC->index)
			);
	}

	/* clean up */
	Management_FreeRegisterf(resultA->index);
	resultA->index = resultIndex;

	Management_FreeRegisterf(resultC->index);
	Result_Destroy(&resultC);

	return resultA;
}

Result Action_Arithmetic(Result resultA, char op, Result resultC)
{
	/* verify leftwise type for semantic */
	switch (resultA->type)
	{
	case TYPE_FLOAT: {
		if (0 == ImplicitCast(resultA, resultC))
		{
			return Action_Arithmeticf(resultA, op, resultC);
		}
	} break;

	case TYPE_INTEGER: {
		if (0 == ImplicitCast(resultA, resultC))
		{
			return Action_Arithmetic_(resultA, op, resultC);
		}
	} break;

	default: {
		Management_LogError("invalid leftwise type for arithmetic");
	} break;
	}

	return NULL;
}

/*
---------------
Action_Add
---------------
*/
Result Action_Add(Result resultA, Result resultB)
{
	Management_LogSemantic("add");
	return Action_Arithmetic(resultA, '+', resultB);
}

/*
---------------
Action_Subtract
---------------
*/
Result Action_Subtract(Result resultA, Result resultB)
{
	Management_LogSemantic("subtract");
	return Action_Arithmetic(resultA, '-', resultB);
}

/*
---------------
Action_Multiply
---------------
*/
Result Action_Multiply(Result resultA, Result resultB)
{
	Management_LogSemantic("multiply");
	return Action_Arithmetic(resultA, '*', resultB);
}

/*
---------------
Action_Exponentiate
---------------
*/
Result Action_Exponentiate(Result resultA, Result resultB)
{
	Management_LogSemantic("exponentiate");
	return Action_Arithmetic(resultA, '^', resultB);
}

/*
---------------
Action_Divide
---------------
*/
Result Action_Divide(Result resultA, Result resultB)
{
	Management_LogSemantic("divide");
	return Action_Arithmetic(resultA, '/', resultB);
}

/*
---------------
Action_Modulo
---------------
*/
Result Action_Modulo(Result resultA, Result resultB)
{
	Management_LogSemantic("modulo");
	return Action_Arithmetic(resultA, '%', resultB);
}

/*
---------------
Action_Negate
---------------
*/
Result Action_Negate(Result resultA)
{
	Management_LogSemantic("negate");
	return Action_Arithmetic(resultA, '~', NULL);
}

/*
---------------------------------------------------------------------
Variant Productions
---------------------------------------------------------------------
*/

/*
---------------
Action_Invoke
---------------
*/
Result Action_Invoke(char * resultA)
{
	Management_LogSemantic("invoke");

	Result resultStructure;
	Result_Create(&resultStructure);

	/* append instructions to result */
	Procedure_PushOperands(resultStructure->procedure,
		/* label */ NULL,
		/* code  */ "jal",
		/* argA  */ resultA,
		/* argB  */ NULL,
		/* argC  */ NULL
		);

	return resultStructure;
}

/*
---------------
Action_Assign
---------------
*/
Result Action_Assign(char * resultA, Result resultB)
{
	Management_LogSemantic("assign");
	if (resultB == NULL) {
		char str[1024];
		sprintf(str, "%i", resultB->intVal);
		Procedure_PushOperands(resultB->procedure,
		/* label */ resultA ,
		/* code  */ "  .space",
		/* argA  */ str,
		/* argB  */ NULL,
		/* argC  */ NULL
		);
		Management_MergeData(resultB->procedure);
		resultB->procedure = NULL;
	}
	Management_SetVariable(resultA, resultB);

	return resultB;
}

Result Action_AssignArray(char * resultA, int size)
{
	Result resultStructure;
	Result_Create(&resultStructure);
	Management_LogSemantic("assign");
	char str[1024];
	sprintf(str, "%i", size);
	Procedure_PushOperands(resultStructure->procedure,
		/* label */ resultA ,
		/* code  */ "  .space",
		/* argA  */ str,
		/* argB  */ NULL,
		/* argC  */ NULL
	);
	Management_MergeData(resultStructure->procedure);
	resultStructure->procedure = NULL;
	return resultStructure;
}

Result Action_AssignFromArray(char* resultA, char* arrayName, int index)
{
	Result resultStructure;
	Result_Create(&resultStructure);
	resultStructure->index = Management_GetRegister();
	Management_LogSemantic("assign from array");
	Management_SetVariable(resultA, resultStructure);
	index = index * 4;
	char str[1024];
	sprintf(str, "%i", index);
	char temp[1024] = {'('};
	char temp2[1024] = {')'};
	strcat(temp, str);
	strcat(temp, temp2);
	strcat(arrayName, temp);

	Procedure_PushOperands(resultStructure->procedure,
		/* label */ NULL ,
		/* code  */ "lw",
		/* argA  */ arrayName,
		/* argB  */ Management_GetRegisterName(resultStructure->index),
		/* argC  */ NULL
		);
		
	return resultStructure;

}

Result Action_AssignToArray(char* resultA, int index, Result resultB)
{
	Management_LogSemantic("assign array");
	index = index * 4;
	char str[1024];
	sprintf(str, "%i", index);
	char temp[1024] = {'('};
	char temp2[1024] = {')'};
	strcat(temp, str);
	strcat(temp, temp2);
	strcat(resultA, temp);

	Procedure_PushOperands(resultB->procedure,
		/* label */ NULL ,
		/* code  */ "sw",
		/* argA  */ Management_GetRegisterName(resultB->index),
		/* argB  */ resultA,
		/* argC  */ NULL
		);
		
	return resultB;
}

/*
---------------
Action_SelectIndex
---------------
*/
Result Action_SelectIndex(char * resultA, Result resultB)
{
	Management_LogSemantic("selecti");

	/*TODO*/
	return NULL;
}

/*
---------------
Action_Select
---------------
*/
Result Action_Select(char * resultA)
{
	Management_LogSemantic("select");

	Result var;
	Management_GetVariable(resultA, &var);

	return var;
}

/*
---------------------------------------------------------------------
Literals
---------------------------------------------------------------------
*/

/*
---------------
Action_StoreFunction
---------------
*/
Result Action_StoreFunction(Function resultA)
{
	Management_LogSemantic("function");
	/* TODO */
	return NULL;
}

/*
---------------
Action_StoreTuple
---------------
*/
Result Action_StoreTuple(Stack resultA)
{
	Management_LogSemantic("tuple");
	/* TODO */
	return NULL;
}

Result Action_StoreArray(int resultA)
{
	Management_LogSemantic("array");

	Result resultStructure;
	Result_Create(&resultStructure);
	resultStructure->type = TYPE_ARRAY;
	resultStructure->index = Management_GetRegister();
	resultStructure->intVal = resultA;

	return resultStructure;
}

/*
---------------
Action_StoreFloat
---------------
*/
Result Action_StoreFloat(float resultA)
{
	Management_LogSemantic("float");

	char str[1024];
	sprintf(str, "%f", resultA);

	Result resultStructure;
	Result_Create(&resultStructure);
	resultStructure->type = TYPE_FLOAT;
	resultStructure->index = Management_GetRegisterf();

	/* append instructions to result */
	Procedure_PushOperands(resultStructure->procedure,
		/* label */ NULL,
		/* code  */ "li.s",
		/* argA  */ Management_GetRegisterNamef(resultStructure->index),
		/* argB  */ str,
		/* argC  */ NULL
		);

	return resultStructure;
}

/*
---------------
Action_StoreInteger
---------------
*/
Result Action_StoreInteger(int resultA)
{
	Management_LogSemantic("integer");

	char str[1024];
	sprintf(str, "%i", resultA);

	Result resultStructure;
	Result_Create(&resultStructure);
	resultStructure->type = TYPE_INTEGER;
	resultStructure->index = Management_GetRegister();

	/* append instructions to result */
	Procedure_PushOperands(resultStructure->procedure,
		/* label */ NULL,
		/* code  */ "li",
		/* argA  */ Management_GetRegisterName(resultStructure->index),
		/* argB  */ str,
		/* argC  */ NULL
		);

	return resultStructure;
}

/*
---------------
Action_StoreBoolean
---------------
*/
Result Action_StoreBoolean(int resultA)
{
	Management_LogSemantic("boolean");

	char str[4];
	sprintf(str, "%i", resultA);

	Result resultStructure;
	Result_Create(&resultStructure);
	resultStructure->type = TYPE_BOOLEAN;
	resultStructure->index = Management_GetRegister();

	/* append instructions to result */
	Procedure_PushOperands(resultStructure->procedure,
		/* label */ NULL,
		/* code  */ "li",
		/* argA  */ Management_GetRegisterName(resultStructure->index),
		/* argB  */ str,
		/* argC  */ NULL
		);

	return resultStructure;
}

/*
---------------------------------------------------------------------
Parsing
---------------------------------------------------------------------
*/

/*
---------------
Action_ParseFunction
---------------
*/
Function Action_ParseFunction(Stack resultA, Result resultB)
{
	/* TODO */
	return NULL;
}

/*
---------------
Action_ParseTuple
---------------
*/
Stack Action_ParseTuple(Result resultA)
{
	/* TODO */
	return NULL;
}

/*
=====================================================================
EOF
=====================================================================
*/
