#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "utils.h"

struct entry_s {
    char *key;
    void *value;
    struct entry_s *next;
};
typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};
typedef struct hashtable_s hashtable_t;

static int ht_hash(hashtable_t *hashtable, char *key);
static entry_t *ht_newpair(char *key, void *value, size_t size);

/**
 * @brief Create a new hashtable
 *
 * @param[in] size
 *
 * @return
 */
void *ht_create(int size) 
{
    int i;
    hashtable_t *hashtable = NULL;

    if (size < 1) {
        return NULL;
    }

    /* Allocate the table itself. */
    hashtable = (hashtable_t *) malloc(sizeof(hashtable_t));
    if (hashtable == NULL) {
        goto bail;
    }

    /* Allocate pointers to the head nodes. */
    hashtable->table = (entry_t**) malloc(sizeof(entry_t) * size);
    if (hashtable->table == NULL) {
        return NULL;
    }

    for( i = 0; i < size; i++ ) {
        hashtable->table[i] = NULL;
    }

    hashtable->size = size;

    return (void *) hashtable;
bail:
    sfree(hashtable->table);
    sfree(hashtable);
    return NULL;
}

/**
 * @brief Insert a key-value pair into a hash table
 *
 * @param[in][out] context
 * @param[in] key
 * @param[in] value
 * @param[in] size
 */
void ht_set(void *context, char *key, void *value, size_t size)
{
    hashtable_t *hashtable;
    int bin = 0;
    entry_t *newpair = NULL;
    entry_t *next = NULL;
    entry_t *last = NULL;

    hashtable = (hashtable_t *) context;
    bin = ht_hash(hashtable, key);
    next = hashtable->table[bin];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next;
    }

    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        free(next->value);
        next->value = (void *) malloc(size + 1);
        memcpy(next->value, value, size);
    } else {
        newpair = ht_newpair(key, value, size);
        if (next == hashtable->table[bin]) {
            newpair->next = next;
            hashtable->table[ bin ] = newpair;
        } else if (next == NULL) {
            last->next = newpair;
        } else  {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/**
 * @brief Retrieve a key-value pair from a hash table
 *
 * @param[in] context
 * @param[in] key
 *
 * @return 
 */
void *ht_get(void *context, char *key)
{
    hashtable_t *hashtable;
    int bin = 0;
    entry_t *pair;

    hashtable = (hashtable_t *) context;

    bin = ht_hash(hashtable, key);

    pair = hashtable->table[bin];
    while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
        pair = pair->next;
    }

    if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
        return NULL;
    } else {
        return pair->value;
    }
}

/**
 * @brief Hash a string for a particular hash table
 *
 * @param[in] hashtable
 * @param[in] key
 *
 * @return 
 */
static int ht_hash(hashtable_t *hashtable, char *key)
{
    unsigned long int hashval;
    int i = 0;

    /* Convert our string to an integer */
    while (hashval < ULONG_MAX && i < strlen(key)) {
        hashval = hashval << 8;
        hashval += key[i];
        i++;
    }

    return hashval % hashtable->size;
}

/**
 * @brief Create a key-value pair
 *
 * @param[in] key
 * @param[in] value
 * @param[in] size
 *
 * @return 
 */
static entry_t *ht_newpair(char *key, void *value, size_t size)
{
    entry_t *newpair;

    newpair = (entry_t *) malloc(sizeof(entry_t));
    if (newpair == NULL) {
        goto bail;
    }

    newpair->key = strdup(key);
    if (newpair->key == NULL) {
        goto bail;
    }

    newpair->value = (void *) malloc(size + 1);
    if (newpair->value == NULL) {
        goto bail;
    }

    memcpy(newpair->value, value, size);
    newpair->next = NULL;

    return newpair;
bail:
    return NULL;
}

#ifdef TEST_HASH
int main(int argc, char **argv)
{
    void *hashtable = ht_create(65536);

    ht_set(hashtable, "key1", (void *) "inky", sizeof("inky"));
    ht_set(hashtable, "key2", (void *) "pinky", sizeof("pinky"));
    ht_set(hashtable, "key3", (void *) "blinky", sizeof("blinky"));
    ht_set(hashtable, "key4", (void *) "floyd", sizeof("floyd"));

    printf("%s\n", (char *) ht_get(hashtable, "key1"));
    printf("%s\n", (char *) ht_get(hashtable, "key2"));
    printf("%s\n", (char *) ht_get(hashtable, "key3"));
    printf("%s\n", (char *) ht_get(hashtable, "key4"));

    return 0;
}
#endif
