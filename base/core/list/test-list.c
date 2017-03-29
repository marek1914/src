#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "list_sort.h"

struct string {
	char word[16];
	struct list_head unsort;
	struct list_head sort;
};

LIST_HEAD(sort_head);
LIST_HEAD(unsort_head);

int main (int argc, char *argv[])
{
	FILE *fp;
	char tmp[16];
	int i;
	

	struct string *string;
	struct list_head *pos;
	
	fp = fopen("db.txt", "r");
	
	if (fp==NULL) {
		printf("db.txt open fail\n");
		return -1;
	}
	
	while ( fgets(tmp, 16, fp) ) {	
		string = malloc(sizeof(*string));
		strcpy(string->word, tmp);
		list_add_tail(&string->unsort, &unsort_head);
		list_add(&string->sort, &sort_head);
	}
	
	list_for_each(pos, &unsort_head) {
		string = list_entry(pos, struct string, unsort);
		printf("%s", string->word);
	}
	
	list_for_each(pos, &sort_head) {
		string = list_entry(pos, struct string, sort);
		printf("%s", string->word);
	}
	
	return 0;
}


