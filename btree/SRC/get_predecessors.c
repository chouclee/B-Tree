#include <stdio.h>
#include <string.h>
#include "def.h"
#include "stack.h"

extern int strtolow(char *s);
extern PAGENO treesearch_page(PAGENO, char* key);
extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int FindInsertionPosition(struct KeyRecord *KeyListTraverser, char *Key,
        int *Found, NUMKEYS NumKeys, int Count);
void prede(char *key, int k, char *result[], int *idx, struct PageHdr *PagePtr);
void printPredessors(char **result, int size, int startIdx);
char *copyString(char *str);
//PAGENO treesearch_page(PAGENO PageNo, char *key, stack *s);
int toGetPredecessors = TRUE;
stack s;

int get_predecessors(char *key, int k, char *result[]) {
	//int found = 0;
	if (k <= 0) {
		printf("k should be positive not %d", k);
		return -1;
	}

	/* set flag and empty the stack */
	toGetPredecessors = TRUE;
	if (!stack_isEmpty(&s))
		stack_destory(&s);
	stack_init(&s, sizeof(PAGENO));

    /* turn to lower case, for uniformity */
    strtolow(key);

    PAGENO page = treesearch_page(ROOT, key);
    stack_push(&s, &page);
    //stack_push(&s, &page);
    PAGENO currPg;
    /*while (!stack_isEmpty(&s)) {
    	stack_pop(&s, &currPg);
    	printf("%ld\n", currPg);
    }
    if (stack_isEmpty(&s)) {
    	printf("empty!\n");
    }*/
    struct PageHdr *PagePtr;
    int idx = k - 1;
    while (!stack_isEmpty(&s) && idx >= 0) {
    	stack_pop(&s, &currPg);
    	PagePtr = FetchPage(currPg);
    	if (IsLeaf(PagePtr)) {
    		prede(key, k, result, &idx, PagePtr);
    	} else {
    		page = treesearch_page(currPg, key);
    		stack_push(&s, &page);
    	}
    	FreePage(PagePtr);
    }
    printPredessors(result, k, idx + 1);

    toGetPredecessors = FALSE;
    return 0;
}

void prede(char *key, int k, char *result[], int *idx, struct PageHdr *PagePtr) {
	struct KeyRecord *KeyListTraverser;
	int InsertionPosition; /* Position for insertion */
	int Count, Found, i;

	Count = 0;

	/* Find insertion position */
	KeyListTraverser = PagePtr->KeyListPtr;
	InsertionPosition = FindInsertionPosition(KeyListTraverser, key, &Found,
			PagePtr->NumKeys, Count);
	if (Found)
		InsertionPosition--;
	int skip = 0;
	if (InsertionPosition > *idx) {
		skip = InsertionPosition - 1 - *idx;
		int temp = skip;
		while (temp > 0) {
			KeyListTraverser = KeyListTraverser->Next;
			temp--;
		}
	}
	int startIdx = *idx - InsertionPosition + 1 + skip;
	for (i = startIdx; i <= *idx; i++) {
		result[i] = copyString(KeyListTraverser->StoredKey);
		//printf("%s\n", result[i]);
		KeyListTraverser = KeyListTraverser->Next;
	}
	*idx = startIdx - 1;
}

void printPredessors(char **result, int size, int startIdx) {
	printf("found %d predecessors:\n", size - startIdx);
	int i;
	for (i = startIdx; i < size; i++) {
		printf("%s\n", result[i]);
		free(result[i]);
	}
}

/**
 * Make a copy of the string to be stored (assumes
 * strdup() or similar functionality is not
 * available
 */
char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

