#include "def.h"

extern int iscommon(char *word);
extern int check_word(char *word);
extern int strtolow(char *s);
extern PAGENO treesearch_page(PAGENO, char* key);
extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO Page);

int get_successors(char *key, int k, char *result[]) {
	int found = 0;
	if (k < 0) {
		printf("k should be positive not %d\n", k);
		return found;
	}

    /* if strlen(key) > MAXWORDSIZE or it is a stop word or contains
     * non-alphanumeric character*/
    if (strlen(key) > MAXWORDSIZE || iscommon(key) ||
    		check_word(key) == FALSE) {
    	return found;
    }

    /* turn to lower case, for uniformity */
    strtolow(key);

    const PAGENO page = treesearch_page(ROOT, key);
    struct PageHdr *PagePtr = FetchPage(page);
    struct KeyRecord *KeyListTraverser;
	int FindInsertionPosition(struct KeyRecord * KeyListTraverser, char *Key,
							  int *Found, NUMKEYS NumKeys, int Count);
	int Count, hasFound;

	Count = 0;

	/* Find insertion position */
	KeyListTraverser = PagePtr->KeyListPtr;
	FindInsertionPosition(KeyListTraverser, key, &hasFound,
											  PagePtr->NumKeys, Count);
	/* key is in the B-Tree */
	if (hasFound == TRUE) {
		while (KeyListTraverser->Next) {
			KeyListTraverser = KeyListTraverser->Next;
			result[found] = KeyListTraverser->StoredKey;
			found++;
		}
	}
	FreePage(PagePtr);

    return found;
}

void printSuccessors(char **result, int size) {
	printf("found %d predecessors:\n", size);
	for (int i = 0; i < size; i++) {
		printf("%s\n", result[i]);
	}
}
