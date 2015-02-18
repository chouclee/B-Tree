#include "stack.h"
#include "def.h"

/* Initialize the stack with default allocation size*/
void stack_init(stack *s,int elemSize)
{
	assert(elemSize > 0);
	s->elemSize = elemSize;
	s->N = 0;
	s->capacity = StackInitialAllocationSize;
	s->data = malloc(StackInitialAllocationSize * elemSize);
	assert(s->data != NULL);
}

/* Check whether stack is empty or not*/
int stack_isEmpty(const stack *s) {
	if (s->N == 0)
		return TRUE;
	else return FALSE;
}

/* Push an element into stack*/
void stack_push(stack *s, const void *src) {
	if(s->N == s->capacity){
		s->capacity *= 2;
		s->data = realloc(s->data, s->capacity * s->elemSize);
		assert(s->data != NULL);
	}
	void *dest = (char *)s->data + s->N * s->elemSize;
	memcpy(dest, src, s->elemSize);
	s->N++;
}

/* Pop an element*/
void stack_pop(stack *s, void *dst) {
	const void *src;
	assert(stack_isEmpty(s) == FALSE);
	s->N--;
	src = (const char *)s->data + s->N * s->elemSize;
	memcpy(dst, src, s->elemSize);
}

/* Free memory of the stack*/
void stack_destory(stack *s)
{
	free(s->data);
}

