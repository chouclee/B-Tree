#include "stack.h"
#include "def.h"

void stack_init(stack *s,int elemSize)
{
	assert(elemSize > 0);
	s->elemSize = elemSize;
	s->N = 0;
	s->capacity = stackInitialAllocationSize;
	s->data = malloc(stackInitialAllocationSize * elemSize);
	assert(s->data != NULL);
}

void stack_destory(stack *s)
{
	free(s->data);
}

int stack_isEmpty(const stack *s) {
	if (s->N == 0)
		return TRUE;
	else return FALSE;
}
void stack_push(stack *s, const void *elemAddr) {
	void *destAddr;
	if(s->N == s->capacity){
		s->capacity *= 2;
		s->data = realloc(s->data, s->capacity * s->elemSize);
		assert(s->data != NULL);
	}
	destAddr = (char *)s->data + s->N * s->elemSize;
	memcpy(destAddr, elemAddr, s->elemSize);
	s->N++;
}
void stack_pop(stack *s, void *elemAddr) {
	const void *sourceAddr;
	assert(stack_isEmpty(s) == FALSE);
	s->N--;
	sourceAddr = (const char *)s->data + s->N * s->elemSize;
	memcpy(elemAddr, sourceAddr, s->elemSize);
}

