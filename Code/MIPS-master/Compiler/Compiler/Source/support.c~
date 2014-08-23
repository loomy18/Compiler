/*
=====================================================================
Support
=====================================================================
*/

#include "support.h"

/*
---------------------------------------------------------------------
Hashing
---------------------------------------------------------------------
*/

#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#define ROTL32(x,y) _rotl(x,y)
#define ROTL64(x,y) _rotl64(x,y)
#define BIG_CONSTANT(x) (x)
#else // defined(_MSC_VER)
#define FORCE_INLINE inline __attribute__((always_inline))

FORCE_INLINE uint32_t rotl32(uint32_t x, int8_t r)
{ return (x << r) | (x >> (32 - r)); }

FORCE_INLINE uint64_t rotl64(uint64_t x, int8_t r)
{ return (x << r) | (x >> (64 - r)); }

#define ROTL32(x,y) rotl32(x,y)
#define ROTL64(x,y) rotl64(x,y)
#define BIG_CONSTANT(x) (x##LLU)
#endif // !defined(_MSC_VER)

FORCE_INLINE uint32_t getblock32(const uint32_t * p, int i)
{
	return p[i];
}

FORCE_INLINE uint64_t getblock64(const uint64_t * p, int i)
{
	return p[i];
}

FORCE_INLINE uint32_t fmix32(uint32_t h)
{
	h ^= h >> 16; h *= 0x85ebca6b;
	h ^= h >> 13; h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

FORCE_INLINE uint64_t fmix64(uint64_t k)
{
	k ^= k >> 33; k *= BIG_CONSTANT(0xff51afd7ed558ccd);
	k ^= k >> 33; k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
	k ^= k >> 33;
	return k;
}


/*
---------------
Murmur3
---------------
*/
uint32_t Murmur3(const void * key, int len, uint32_t seed)
{
	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len / 4;
	uint32_t h1 = seed;
	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;
	const uint32_t * blocks = (const uint32_t *)(data + nblocks * 4);
	for (int i = -nblocks; i; i++)
	{
		uint32_t k1 = getblock32(blocks, i);
		k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2;
		h1 ^= k1; h1 = ROTL32(h1, 13); h1 = h1 * 5 + 0xe6546b64;
	}
	const uint8_t * tail = (const uint8_t*)(data + nblocks * 4);
	uint32_t k1 = 0;
	switch (len & 3)
	{
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
		k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
	};
	h1 ^= len;
	h1 = fmix32(h1);
	return h1;
}

/*
---------------------------------------------------------------------
Structures
---------------------------------------------------------------------
*/

/*
---------------
Stack_Create
---------------
*/
int Stack_Create(Stack * self)
{
	Stack temp = (Stack)malloc(sizeof(Stack));
	if (NULL != temp)
	{
		temp->size = 0;
		temp->size_ = 1;
		temp->incr_ = 4;
		temp->data = (void**)malloc(sizeof(void*));
		if (NULL != temp->data)
		{
			*self = temp;
		}
		else {
			free(temp);
			return -2;
		}
	}
	else {
		return -1;
	}
	return 0;
}


/*
---------------
Stack_Destroy
---------------
*/
int Stack_Destroy(Stack * self)
{
	Stack temp = *self;
	free(temp->data);
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Stack_Reserve
---------------
*/
int Stack_Reserve(Stack self, size_t size)
{
	void ** temp = (void**)malloc(sizeof(void*)*size);
	for (int i = 0; i < self->size; ++i)
	{
		temp[i] = self->data[i];
	}
	self->size_ = size;
	self->data = temp;
	if (self->size >= size)
	{
		self->size = size - 1;
	}
	return 0;
}

/*
---------------
Stack_Ensure
---------------
*/
int Stack_Ensure(Stack self, size_t size)
{
	if (self->size_ < size)
	{
		return Stack_Reserve(self, size + self->incr_);
	}
	return 0;
}

/*
---------------
Stack_Push
---------------
*/
int Stack_Push(Stack self, void * data)
{
	if (0 == Stack_Ensure(self, self->size + 1))
	{
		printf("OK\n");
		self->data[self->size++] = data;
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Stack_Pull
---------------
*/
int Stack_Pull(Stack self, void ** data)
{
	if (self->size != 0)
	{
		*data = self->data[--self->size];
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Instruction_Create
---------------
*/
int Instruction_Create(Instruction * self)
{
	Instruction temp = (Instruction)malloc(sizeof(instruction));
	if (NULL != temp)
	{
		temp->label = NULL;
		temp->code = NULL;
		temp->argA = NULL;
		temp->argB = NULL;
		temp->argC = NULL;
		*self = temp;
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Instruction_Destroy
---------------
*/
int Instruction_Destroy(Instruction * self)
{
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Procedure_Create
---------------
*/
int Procedure_Create(Procedure * self)
{
	Procedure temp = (Procedure)malloc(sizeof(procedure));
	if (NULL != temp)
	{
		temp->size = 0;
		temp->size_ = 1;
		temp->incr_ = 32;
		temp->data = (Instruction*)malloc(sizeof(Instruction*));
		if (NULL != temp->data)
		{
			*self = temp;
		}
		else {
			free(temp);
			return -2;
		}
	}
	else {
		return -1;
	}
	return 0;
}


/*
---------------
Procedure_Destroy
---------------
*/
int Procedure_Destroy(Procedure * self)
{
	Procedure temp = *self;
	while (temp->size != 0)
	{
		Instruction instr = NULL;
		Procedure_Pull(temp, &instr);

		if (instr->label)
			free(instr->label);
		if (instr->code)
			free(instr->code);
		if (instr->argA)
			free(instr->argA);
		if (instr->argB)
			free(instr->argB);
		if (instr->argC)
			free(instr->argC);

		Instruction_Destroy(&instr);
	}
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Procedure_Reserve
---------------
*/
int Procedure_Reserve(Procedure self, size_t size)
{
	Instruction * temp = (Instruction*)realloc(self->data, sizeof(Instruction*)*size);
	if (NULL != temp)
	{
		self->size_ = size;
		self->data = temp;
		if (self->size >= size)
		{
			self->size = size - 1;
		}
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Procedure_Ensure
---------------
*/
int Procedure_Ensure(Procedure self, size_t size)
{
	if (self->size_ < size)
	{
		return Procedure_Reserve(self, size + self->incr_);
	}
	return 0;
}

/*
---------------
Procedure_Push
---------------
*/
int Procedure_Push(Procedure self, Instruction instr)
{
	if (0 == Procedure_Ensure(self, self->size + 1))
	{
		self->data[self->size++] = instr;
	}
	else {
		return -1;
	}
	return 0;
}


/*
---------------
Procedure_PushOperands
---------------
*/
int Procedure_PushOperands(Procedure self,
	char * label, char * code, char * argA, char * argB, char * argC)
{
	if (0 == Procedure_Ensure(self, self->size + 1))
	{
		Instruction instr;
		Instruction_Create(&instr);

		if (label)
			instr->label = strdup(label);
		if (code)
			instr->code = strdup(code);
		if (argA)
			instr->argA = strdup(argA);
		if (argB)
			instr->argB = strdup(argB);
		if (argC)
			instr->argC = strdup(argC);

		self->data[self->size++] = instr;
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Procedure_Pull
---------------
*/
int Procedure_Pull(Procedure self, Instruction * instr)
{
	if (self->size != 0)
	{
		*instr = self->data[--self->size];
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Procedure_Merge (TODO: optimize)
---------------
*/
int Procedure_Merge(Procedure self, Procedure other)
{
	if (other->size != 0)
	{
		for (int i = 0; i < other->size; ++i)
		{
			Procedure_PushOperands(self,
				other->data[i]->label,
				other->data[i]->code,
				other->data[i]->argA,
				other->data[i]->argB,
				other->data[i]->argC
				);
		}
	}
	else {
		return 01;
	}
	return 0;
}

/*
---------------
Result_Create
---------------
*/
int Result_Create(Result * self)
{
	Result temp = (Result)malloc(sizeof(result));
	if (NULL != temp)
	{
		temp->type = -1;
		temp->intVal = -1;
		temp->index = -1;
		temp->label = NULL;
		if (0 == Procedure_Create(&temp->procedure))
		{
			*self = temp;
		}
		else {
			return -2;
		}
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Result_Destroy
---------------
*/
int Result_Destroy(Result * self)
{
	Result temp = *self;
	Procedure_Destroy(&temp->procedure);
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Entry_Create
---------------
*/
int Entry_Create(Entry * self)
{
	Entry temp = (Entry)malloc(sizeof(entry));
	if (NULL != temp)
	{
		temp->label = NULL;
		temp->variable = NULL;
		temp->positive = temp;
		temp->negative = temp;
		*self = temp;
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Entry_Destroy
---------------
*/
int Entry_Destroy(Entry * self)
{
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Table_Create
---------------
*/
int Table_Create(Table * self)
{
	Table temp = (Table)malloc(sizeof(table));
	if (NULL != temp)
	{
		temp->data = (Entry*)malloc(sizeof(Entry)*MAX_TABLE_BUCKETS);
		for (int i = 0; i < MAX_TABLE_BUCKETS; ++i)
		{
			Entry_Create(&temp->data[i]);
		}
		*self = temp;
	}
	else {
		return -1;
	}
	return 0;
}

/*
---------------
Table_Destroy
---------------
*/
int Table_Destroy(Table * self)
{
	Table temp = *self;
	for (int i = 0; i < MAX_TABLE_BUCKETS; ++i)
	{
		Entry iter = temp->data[i]->positive;
		while (temp->data[i] != iter)
		{
			Entry next = iter->positive;
			if (iter->label)
			{
				free(iter->label);
			}
			Entry_Destroy(&iter);
			iter = next;
		}
	}
	free(*self);
	*self = NULL;
	return 0;
}

/*
---------------
Table_Set
---------------
*/
int Table_Set(Table self, char * label, Result * refAttribute)
{
	uint32_t index = Murmur3(label, strlen(label), 1) % MAX_TABLE_BUCKETS;

	Entry iter = self->data[index];
	while (self->data[index] != (iter = iter->positive))
	{
		if (0 == strcmp(iter->label, label))
		{
			Result temp = *refAttribute;
			iter->variable = temp;
			*refAttribute = temp;

			return 1;
		}
	}
	Entry temp;
	Entry_Create(&temp);

	temp->label = strdup(label);
	temp->variable = *refAttribute;

	temp->positive = self->data[index];
	temp->negative = self->data[index]->negative;
	self->data[index]->negative = temp;

	return 0;
}

/*
---------------
Table_Get
---------------
*/
int Table_Get(Table self, char * label, Result * refAttribute)
{
	uint32_t index = Murmur3(label, strlen(label), 1) % MAX_TABLE_BUCKETS;

	Entry iter = self->data[index];
	while (self->data[index] != (iter = iter->positive))
	{
		if (0 == strcmp(iter->label, label))
		{
			*refAttribute = iter->variable;
			return 1;
		}
	}
	*refAttribute = NULL;
	return 0;
}


/*
=====================================================================
EOF
=====================================================================
*/


