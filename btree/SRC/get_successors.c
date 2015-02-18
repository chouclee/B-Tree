#include "def.h"

extern int iscommon(char *word);
extern int check_word(char *word);
extern int strtolow(char *s);
extern PAGENO treesearch_page(PAGENO, char* key);
extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO Page);
extern int CompareKeys(char *Key, char *Word);
void printSuccessors(char** result, int size);
void findInsertionP(struct KeyRecord **KeyListTraverser, char *Key,
                          int *Found, NUMKEYS NumKeys);
PAGENO FindNumPagesInTree(void);


int get_successors(char *key, int k, char *result[]) {
	int found = 0;
	if (k <= 0) {
		printf("k should be positive not %d", k);
		return -1;
	}

    /* if strlen(key) > MAXWORDSIZE or it is a stop word or contains
     * non-alphanumeric character*/
    //if (strlen(key) > MAXWORDSIZE || iscommon(key) ||
    //		check_word(key) == FALSE) {
    //	return -1;
    //}

    /* turn to lower case, for uniformity */
    strtolow(key);

    const PAGENO page = treesearch_page(ROOT, key);
    struct PageHdr *PagePtr = FetchPage(page);
    struct KeyRecord **KeyListTraverser;
	int hasFound;

	KeyListTraverser = &(PagePtr->KeyListPtr);
	findInsertionP(KeyListTraverser, key, &hasFound,
											  PagePtr->NumKeys);
	if (hasFound)
		*KeyListTraverser = (*KeyListTraverser)->Next;
	else {
		int Result;
		char *Word; /* Key stored in B-Tree */
		/* Compare the possible new key with key stored in B-Tree */
		Word = (*KeyListTraverser)->StoredKey;
		(*(Word + (*KeyListTraverser)->KeyLen)) = '\0';
		Result = CompareKeys(key, Word);
		if (Result == 2)
			*KeyListTraverser = (*KeyListTraverser)->Next;
	}
	while ((*KeyListTraverser) && k > 0) {
		//printf("%s\n", KeyListTraverser->StoredKey);
		result[found] = (*KeyListTraverser)->StoredKey;
		//char *c = result[found];
		//printf("%s\n", c);
		found++;
		k--;
		*KeyListTraverser = (*KeyListTraverser)->Next;
	}
	/* remaining k keys may on continues pages*/
	PAGENO totalPage = FindNumPagesInTree();
	while (k > 0) {
		PAGENO nextPage = PagePtr->PgNumOfNxtLfPg;
		/* check validity of "Page" */
		if ((nextPage < 1) || (nextPage > totalPage)) {
			break;
		}
		FreePage(PagePtr);
		PagePtr = FetchPage(nextPage);
		if (PagePtr) {
			KeyListTraverser = &(PagePtr->KeyListPtr);
			while ((*KeyListTraverser) && k > 0) {
				result[found] = (*KeyListTraverser)->StoredKey;
				found++;
				k--;
				*KeyListTraverser = (*KeyListTraverser)->Next;
			}
		}
	}
	//}
	printSuccessors(result, found);

	FreePage(PagePtr);

    return 0;
}

void printSuccessors(char **result, int size) {
	printf("found %d successors:\n", size);
	int i;
	for (i = 0; i < size; i++) {
		printf("%s\n", result[i]);
	}
}

void findInsertionP(struct KeyRecord **KeyListTraverser, char *Key,
                          int *Found, NUMKEYS NumKeys) {
    int Result;
    int CompareKeys(char *Key, char *Word);

    /* -christos- the next block probably provides for
    insertion in empty list (useful for insertion in root
    for the first time! */

    if (NumKeys == 0) {
        *Found = FALSE;
    }

    /* Compare the the possible new key with the key stored in B-Tree */
    Result = CompareKeys(Key, (*KeyListTraverser)->StoredKey);

    NumKeys = NumKeys - 1;

    if (Result == 0) /* Match found */
    {
        *Found = TRUE;
        return;
    } else              /* No match yet */
        *Found = FALSE;

    if (NumKeys > 0) {
        if (Result == 1)        /* New key < stored key */
            return;
        else                    /* New key > stored key: keep searching */
        {
            (*KeyListTraverser) = (*KeyListTraverser)->Next;
            findInsertionP(KeyListTraverser, Key, Found, NumKeys);
        }
    }
}
