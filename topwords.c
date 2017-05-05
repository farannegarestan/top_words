/*
 * topwords.c
 *
 *  Created on: Sep 24, 2014
 *      Author: Faran Negarestan
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

struct Entry* search(char *word);
struct Entry* findTail();
void makeArray(struct Entry* arr);
void toLower(char *p);
void showAll();
int cmpfunc (const void * a, const void * b);
int count();
void usage(char *app_name) {
	printf("%s -i <input file> -n <number of words to show>\n", app_name);
}

struct Entry {
	char word[64];
	int count;
	struct Entry *next;
};

struct Entry head = { NULL, 0, NULL };

int main(int argc, char **argv) {
	int c;
	char *file_name = NULL;
	int number_of_words = 0;
	FILE *input_file;
	while ( (c = getopt(argc, argv, "i:n:")) != -1) {
		switch (c) {
		case 'i':
			file_name = optarg;
			break;
		case 'n':
			number_of_words = atoi(optarg);
			break;
		}
	}
	if (file_name == NULL || number_of_words == 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	input_file = fopen(file_name, "r");
	if (input_file == NULL) {
		fprintf(stderr, "Cannot open file %s\n", file_name);
		exit(EXIT_FAILURE);
	}
	while(!feof(input_file)) {
		char word[64];
		fscanf(input_file, "%s\n", word);
		toLower(word);
		printf("%s\n", word);
		struct Entry *existing = search(word);
		if (existing != NULL) {
			++existing->count;
		} else {
			struct Entry *tail = findTail();
			struct Entry *entry = malloc(sizeof(struct Entry));
			strcpy(entry->word, word);
			entry->count = 1;
			tail->next = entry;
		}
	}
	int entry_count = count();
	printf("Number of element is the list is %d\n", entry_count);
	//showAll();

	struct Entry asArray[entry_count];
	makeArray(asArray);
	qsort(asArray, entry_count, sizeof(struct Entry), cmpfunc);
	for(int i=0 ; i < entry_count ; i++) {
		struct Entry e = asArray[i];
		printf("Word: %s  Count: %d\n", e.word, e.count);
	}

	return EXIT_SUCCESS;
}

int cmpfunc (const void * a, const void * b)
{
   return ( ((struct Entry*)b)->count - ((struct Entry*)a)->count );
}


struct Entry* search(char *word) {
	struct Entry* start = &head;
	while(start != NULL) {
		if (strcmp(word, start->word) == 0) {
			return start;
		} else {
			start = start->next;
		}
	}
	return NULL;
}

void makeArray(struct Entry array[]) {
	struct Entry *start = head.next;
	int index = 0;
	while(start != NULL) {
		array[index++] = *start;
		start = start->next;
	}
}

int count() {
	int count = 0;
	struct Entry *start = &head;
	while(start->next != NULL) {
		start = start->next;
		count++;
	}
	return count;
}


struct Entry* findTail() {
	struct Entry *start = &head;
	while(start->next != NULL) {
		start = start->next;
	}
	return start;
}

void showAll() {
	struct Entry *start = &head;
	while(start != NULL) {
		printf("Word: %s  Count: %d\n", start->word, start->count);
		start = start->next;
	}
}

void toLower(char *p) {
	for ( ; *p; ++p) *p = tolower(*p);
}

