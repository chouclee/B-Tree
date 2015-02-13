void printSuccessors(char **result, int size) {
	printf("found %d predecessors:\n", size);
	for (int i = 0; i < size; i++) {
		printf("%s\n", result[i]);
	}
}
