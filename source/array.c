/// @file array.c
/// @brief This file contains implementations for a dynamic array type.
///
/// @author Ben Potter
/// @date 25 May 2024

#include "array.h"


bool d_array_create(d_array *da, size_t element_size, size_t size_hint) {
    if(da == NULL)
        return false;

    // This array has already been initialized.
    if(da->block != NULL)
        return false;

    // Initialize the structure.
    da->size = 0; // No elements to start.
    da->element_size = element_size;
    da->block_size = element_size * size_hint;

    // Allocate memory according to the element_size and size_hint.
    // TODO Maybe use calloc here?
    da->block = malloc(da->block_size);

    return true;
}


bool d_array_append(d_array *da, void *element) {
    if(da == NULL)
        return false;

    // TODO Handle dynamic resizing.
    
     
    da->size += 1;

    return false;
}


bool d_array_get(d_array *da, int i, void** out) {

    return false;
}


bool d_array_size(d_array *da, size_t *out_size) {
    return false;
}


bool d_array_destroy(d_array *da) {
    if(da == NULL)
        return false;

    // The array contains no allocated memory, return.
    if(da->block == NULL)
        return true;

    // We know the block contains allocated memory. Free it.
    free(da->block);
    da->block = NULL; 

    // Error, the block size has been incorrectly set. 
    if(da->block_size == 0)
        return false;

    da->size = 0;
    da->block_size = 0;

    return true;
}

