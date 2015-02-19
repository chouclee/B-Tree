typedef struct{
	void *data;
	int elemSize;
	int N;
	int capacity;
} stack;

#define StackInitialAllocationSize (10)
void stack_init(stack *s,int elemSize);
int stack_isEmpty(const stack *s);
void stack_push(stack *s, const void *src);
void stack_pop(stack *s, void *dst);
void stack_destory(stack *s);
