#include <stdio.h>

#include "src/hasher.h"

int main(void)
{
	void *hashtable = ht_create(65536);

    int num1 = 1;
    int num2 = 2;

    const char *str1 = "kivanc";
    const char *str2 = "cakmak";

	ht_set(hashtable, "num", (void *) &num1, sizeof(int));
    ht_set(hashtable, "str", (void *) str1, sizeof(str1));

	printf("%d\n", *(int *) ht_get(hashtable, "num"));
	printf("%s\n", (char *) ht_get(hashtable, "str"));

	ht_set(hashtable, "num", (void *) &num2, sizeof(int));
    ht_set(hashtable, "str", (void *) str2, sizeof(str2));

	printf("%d\n", *(int *) ht_get(hashtable, "num"));
	printf("%s\n", (char *) ht_get(hashtable, "str"));

    return 0;
}
