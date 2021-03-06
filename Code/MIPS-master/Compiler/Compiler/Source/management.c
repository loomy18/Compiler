/*
=====================================================================
Management
=====================================================================
*/

#include "management.h"

/*
---------------------------------------------------------------------
Globals
---------------------------------------------------------------------
*/

#define MAX_DIGITS_LENUM   8  /* max digits in label enumeration */

#define MAX_REG_COUNT      10 /* max register count */

struct Register_t
{
	int    empty;

	char * label;
}

GLOBAL_REGISTERS[MAX_REG_COUNT]
= { { 1, "$t0" }, { 1, "$t1" }, { 1, "$t2" }, { 1, "$t3" },
{ 1, "$t4" }, { 1, "$t5" }, { 1, "$t6" }, { 1, "$t7" },
{ 1, "$t8" }, { 1, "$t9" }
},
GLOBAL_REGISTERSF[MAX_REG_COUNT]
= { { 1, "$f0" }, { 1, "$f1" }, { 1, "$f2" }, { 1, "$f3" },
{ 1, "$f4" }, { 1, "$f5" }, { 1, "$f6" }, { 1, "$f7" },
{ 1, "$f8" }, { 1, "$f9" }
};

char        GLOBAL_LABEL_BUFFER[MAX_DIGITS_LENUM];

int         GLOBAL_LABEL_COUNT;

FILE *      GLOBAL_LOG_STREAM;

Stack       GLOBAL_STACK;

Stack       GLOBAL_NAME_STACK;

Stack       GLOBAL_FRAME_STACK;

Procedure   GLOBAL_CACHED_PROCEDURE;

Procedure   GLOBAL_CACHED_DATA;

/*
---------------------------------------------------------------------
Functions
---------------------------------------------------------------------
*/

/*
---------------
Management_Initiate
---------------
*/
int Management_Initiate()
{
	GLOBAL_LABEL_COUNT = 0;
	GLOBAL_LOG_STREAM = stdout;
	GLOBAL_CACHED_PROCEDURE = NULL;
	GLOBAL_CACHED_DATA = NULL;

	Procedure_Create(&GLOBAL_CACHED_PROCEDURE);
	Procedure_Create(&GLOBAL_CACHED_DATA);

	Stack_Create(&GLOBAL_FRAME_STACK);
	Stack_Create(&GLOBAL_NAME_STACK);
	Stack_Create(&GLOBAL_STACK);

	return 0;
}


/*
---------------
Management_Terminate
---------------
*/
int Management_Terminate()
{
	//TODO: clean up

	return 0;
}

/*
---------------
Management_Finalize
---------------
*/
int Management_Finalize()
{
	return 0;
}

/*
---------------
Management_Serialize
---------------
*/
int Management_Serialize(FILE * file)
{
	if (GLOBAL_CACHED_PROCEDURE)
	{
		/* TEMPORARY */
		fprintf(file, "data:\n");
		for (int i = 0; i < GLOBAL_CACHED_DATA->size; ++i)
		{
			Instruction instr = GLOBAL_CACHED_DATA->data[i];

			if (instr->label)
			{
				fprintf(file, "%s:", instr->label);
			}
			if (instr->code)
			{
				fprintf(file, "\t%s\t", instr->code);
				if (instr->argA)
				{
					fprintf(file, "\t%s", instr->argA);
				}
				if (instr->argB)
				{
					fprintf(file, " %s", instr->argB);
				}
				if (instr->argC)
				{
					fprintf(file, " %s", instr->argC);
				}
			}
			fprintf(file, "\n");
		}


		fprintf(file, "main:\n");

		for (int i = 0; i < GLOBAL_CACHED_PROCEDURE->size; ++i)
		{
			Instruction instr = GLOBAL_CACHED_PROCEDURE->data[i];

			if (instr->label)
			{
				fprintf(file, "%s:", instr->label);
			}
			if (instr->code)
			{
				fprintf(file, "\t%s\t", instr->code);
				if (instr->argA)
				{
					fprintf(file, "\t%s", instr->argA);
				}
				if (instr->argB)
				{
					fprintf(file, " %s", instr->argB);
				}
				if (instr->argC)
				{
					fprintf(file, " %s", instr->argC);
				}
			}
			fprintf(file, "\n");
		}

		/* TEMPORARY */
		fprintf(file, "\t%s\t\t%s %s\n", "li", "$v0", "10");
		fprintf(file, "\t%s\t\n", "syscall");

		/* functions */
		for (int k = 0; k < GLOBAL_STACK->size; ++k)
		{
			Result res = GLOBAL_STACK->data[k];
			fprintf(file, "%s:", res->label);

			for (int j = 0; j < res->procedure->size; ++j)
			{
				Instruction instr1 = res->procedure->data[j];

				if (instr1->label)
				{
					fprintf(file, "%s:\n", instr1->label);
				}
				if (instr1->code)
				{
					fprintf(file, "\t%s\t", instr1->code);
					if (instr1->argA)
					{
						fprintf(file, "\t%s", instr1->argA);
					}
					if (instr1->argB)
					{
						fprintf(file, " %s", instr1->argB);
					}
					if (instr1->argC)
					{
						fprintf(file, " %s", instr1->argC);
					}
				}
				fprintf(file, "\n");
			}
		}
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Management_LogStream
---------------
*/
void Management_LogStream(FILE * file)
{
	GLOBAL_LOG_STREAM = file;
}

/*
---------------
Management_LogError
---------------
*/
void Management_LogError(char * message)
{
	fprintf(GLOBAL_LOG_STREAM, "[ X ] %s\n", message);
}

/*
---------------
Management_LogWarning
---------------
*/
void Management_LogWarning(char * message)
{
	fprintf(GLOBAL_LOG_STREAM, "[ ! ] %s\n", message);
}

/*
---------------
Management_LogAlert
---------------
*/
void Management_LogAlert(char * message)
{
	fprintf(GLOBAL_LOG_STREAM, "[   ] %s\n", message);
}

/*
---------------
Management_LogSemantic
---------------
*/
void Management_LogSemantic(char * message)
{
	fprintf(GLOBAL_LOG_STREAM, "[ > ] %s\n", message);
}

/*
---------------
Management_GetUniqueLabel
---------------
*/
char * Management_GetUniqueLabel()
{
	sprintf(GLOBAL_LABEL_BUFFER, "L%d", GLOBAL_LABEL_COUNT++);
	return GLOBAL_LABEL_BUFFER;
}

/*
---------------
Management_GetRegister
---------------
*/
int Management_GetRegister()
{
	for (int i = 0; i < MAX_REG_COUNT; ++i)
	{
		if (1 == GLOBAL_REGISTERS[i].empty)
		{
			GLOBAL_REGISTERS[i].empty = 0;
			return i;
		}
	}
	Management_LogError("registry overflow");
	return -1;
}

/*
---------------
Management_GetRegisterName
---------------
*/
char * Management_GetRegisterName(int index)
{
	return GLOBAL_REGISTERS[index].label;
}

/*
---------------
Management_GetRegisterf
---------------
*/
int Management_GetRegisterf()
{
	for (int i = 0; i < MAX_REG_COUNT; ++i)
	{
		if (1 == GLOBAL_REGISTERSF[i].empty)
		{
			GLOBAL_REGISTERSF[i].empty = 0;
			return i;
		}
	}
	Management_LogError("registryf overflow");
	return -1;
}

/*
---------------
Management_GetRegisterNamef
---------------
*/
char * Management_GetRegisterNamef(int index)
{
	return GLOBAL_REGISTERSF[index].label;
}

/*
---------------
Management_FreeRegister
---------------
*/
void Management_FreeRegister(int index)
{
	GLOBAL_REGISTERS[index].empty = 1;
}

/*
---------------
Management_FreeRegisterf
---------------
*/
void Management_FreeRegisterf(int index)
{
	GLOBAL_REGISTERSF[index].empty = 1;
}

/*
---------------
Management_PushVariables
---------------
*/
void Management_PushVariables(int index)
{
	Stack_Push(GLOBAL_FRAME_STACK, GLOBAL_NAME_STACK);
	Stack_Create(&GLOBAL_NAME_STACK);
}

/*
---------------
Management_PullVariables
---------------
*/
void Management_PullVariables(int index)
{
	Stack_Destroy(&GLOBAL_NAME_STACK);
	Stack_Pull(GLOBAL_FRAME_STACK, (void**)&GLOBAL_NAME_STACK);
}

/*
---------------
Management_SetVariable
---------------
*/
int Management_SetVariable(char * label, Result resultB)
{
	for (int i = 0; i < GLOBAL_NAME_STACK->size; ++i)
	{
		Pair x = GLOBAL_NAME_STACK->data[i];
		if (0 == strcmp(x->name, label))
		{
			x->data = resultB;
			return 1;
		}
	}

	Pair p = (Pair)malloc(sizeof(PAIR));
	p->name = strdup(label);
	p->data = resultB;

	Stack_Push(GLOBAL_NAME_STACK, p);

	return 0;
}

/*
---------------
Management_GetVariable
---------------
*/
int Management_GetVariable(char * label, Result * resultB)
{
	for (int i = 0; i < GLOBAL_NAME_STACK->size; ++i)
	{
		Pair p = GLOBAL_NAME_STACK->data[i];
		if (0 == strcmp(p->name, label))
		{
			*resultB = p->data;
			return 1;
		}
	}
	return 0;
}

/*
---------------
Management_PushFunction
---------------
*/
int Management_PushFunction(char * label, Result resultA)
{
	resultA->label = strdup(label);
	Stack_Push(GLOBAL_STACK, resultA);
	return 0;
}

/*
---------------
Management_MergeProcedure
---------------
*/
void Management_MergeProcedure(Procedure proc)
{
	Procedure_Merge(GLOBAL_CACHED_PROCEDURE, proc);
}

void Management_MergeData(Procedure proc)
{
	Procedure_Merge(GLOBAL_CACHED_DATA, proc);
}

/*
=====================================================================
EOF
=====================================================================
*/


