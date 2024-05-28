/// @file array.h
/// @brief This file contains definitions for a dynamic array type.
/// https://www.davidpriver.com/ctemplates.html 
///
/// @warning Not complete. Might not even compile.
///
/// @author Ben Potter
/// @date 22 May 2024

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/// @struct d_array
/// @brief This structure stores information about a dynamic array.
typedef struct d_array {
    
    /// @brief The number of elements in the array.
    size_t size;

    /// @brief The size of the currently allocated block of memory.
    size_t block_size;

    /// @brief The size in bytes of a single element of the array.
    size_t element_size;

    /// @brief A pointer to the currently allocated block of memory.
    void* block;

} d_array;


/// @brief Create a dynamic array with initial size.
///
/// @param da A reference to the state of the dynamic array.
/// @param element_size The size of a single element of memory in bytes.
/// @param size_hint The initial size of the dynamic array in elements.
/// @return True on success, otherwise false.
bool d_array_create(d_array *da, size_t element_size, size_t size_hint);


/// @brief Add an element to a dynamic array, copying the element.
///
/// @param da A reference to the state of the dynamic array.
/// @param element A pointer to the element that will be added to the array.
/// @return True on success, otherwise false.
bool d_array_append(d_array *da, void *element);


// @brief Get the ith element of a dynamic array.
//
/// @param da A reference to the state of the dynamic array.
/// @param i The index of the element.
/// @param void** A reference to a void pointer to write the data to.
/// @return True on success, otherwise false.
bool d_array_get(d_array *da, int i, void** out);


/// @brief Get the size of a dynamic array.
///
/// @param da A reference to the state of the dynamic array.
/// @param out_size A pointer to the output size.
/// @return True on success, otherwise false.
bool d_array_size(d_array *da, size_t *out_size);


/// @brief Destroy a dynamic array, deallocating all memory.
///
/// @param da A reference to the state of the dynamic array.
/// @return True on success, otherwise false.
bool d_array_destroy(d_array *da);

