#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"dynamic_array.h"

static int DEFAULT_SIZE = 8;
static Type DEFAULT_TYPE = STRING;

static element* new_element(const void* value, Type type) {
    element* el = malloc(sizeof(element));
    switch (type) {
        case INTEGER: el->integer = *((int*)value); break;
        case STRING: el->string = strdup((char*)value); break;
    }
    return el;
}

static dy_array* new_array(int base_size, Type value_type) {
    dy_array* da = malloc(sizeof(dy_array));
    da->size = base_size;
    da->type = value_type;
    da->count = 0;

    da->arr = calloc((size_t)da->size+1, sizeof(element*));

    return da;
}

dy_array* init_dy(Type type) {
    printf("Creating array of size %d\n", DEFAULT_SIZE);
    return new_array(DEFAULT_SIZE, type);
}

static void delete_element(element* el, Type type) {
    if (type == STRING) {
        free(el->string);
    }
    free(el);
}

void delete_dy(dy_array* da) {
    for (int i = 0; i < da->count; i++) {
        delete_element(da->arr[i], da->type);
    }
    free(da->arr);
    free(da);
}


static void dy_resize(dy_array* da, int new_size) {
    dy_array* new_da = new_array(new_size, da->type);
        
    switch(new_da->type) {
        case INTEGER :
            for (int i = 0; i < da->size; i++) {
                dy_append(new_da, &(da->arr[i]->integer));
            }
            break;
        case STRING :
            for (int i = 0; i < da->size; i++) {
                dy_append(new_da, da->arr[i]->string);
            }
            break;
    }
    const int temp_size = new_da->size;
    new_da->size = da->size;
    da->size = temp_size;

    element** temp_arr = new_da->arr;
    new_da->arr = da->arr;
    da->arr = temp_arr;

    delete_dy(new_da);
}


static void dy_size_up(dy_array* da) {
    dy_resize(da, da->size * 2);
    printf("Resizing array to %d\n", da->size);
}


void dy_append(dy_array* da, const void* c) {
    dy_insert(da, c, da->count);
}


void dy_insert(dy_array* da, const void* c, int index) {
    if (index > da->count) {
        puts("Out of bounds exception");
        return;
    }

    if (da->count >= da->size) {
        dy_size_up(da);
    }
    element* el = new_element(c, da->type);
    for (int i = da->count; i >= index; i--) {
        da->arr[i+1] = da->arr[i];
    }
    da->arr[index] = el;
    da->count++;
}


void* dy_remove(dy_array* da, const int index) {
    if (index > da->count) {
        puts("Out of bounds exception");
        return NULL;
    }
    void* ret;
    int temp;
    switch(da->type) {
        case INTEGER:
            temp = da->arr[index]->integer;
            ret = malloc(sizeof(int));
            *((int*)ret) = temp;
            break;
        case STRING:
            ret = strdup(da->arr[index]->string);
            break;
    }
    delete_element(da->arr[index], da->type);
    for (int i = index+1; i <= da->count; i++) {
        da->arr[i-1] = da->arr[i];
    }
    da->count--;
    return ret;
}


void* dy_pop(dy_array* da) {
    return dy_remove(da, da->count-1);
}


void* dy_get(dy_array* da, const int index) {
    if (index > da->count) {
        puts("Out of bounds exception");
        return NULL;
    }
    switch(da->type) {
        case INTEGER:
            return &(da->arr[index]->integer);
        case STRING:
            return da->arr[index]->string;
    }
}
