#ifndef _HASHER_H
#define _HASHER_H

void *ht_create(int size);

void ht_set(void *hashtable, char *key, void *value, size_t size);

void *ht_get(void *hashtable, char *key);

#endif
