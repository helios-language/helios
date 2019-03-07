
#ifndef HELIOS_STRING_H
#define HELIOS_STRING_H

/**
 * Defines a helios string object. A helios string can hold strings of any
 * length. A helios string is strictly IMMUTABLE and therefore can only be
 * changed by copying it. TODO: mutablestring (which can be implemented as a
 * helios_list accepting only characters). TODO: unicode support.
 */

#include <helios_object.h>

/**
 * The default size allocated for a helios_string object. Increments in steps of
 * factors of 2 every time the length of the string exeeds the size.
 */
#define HELIOS_STRING_DEFAULT_SIZE 10

/**
 * The definition of the structure of a helios_string object.
 */
typedef struct __helios_string_s {
    HELIOS_OBJECT_COMMON_BASE; //!< always required for a helios object for
                               //!< casting to work
    uint32_t size; //!< the allocated size of the string, grows in steps of
                   //!< powers of 2. Might differ from filled.

    uint32_t filled; //!< the length of the string. Always smaller than
                     //!< allocated size.

    char *value; //!< an array of c characters *NOT* terminated with NULL (use
                 //!< filled) holding the textual data of the helios string.
} helios_string;

helios_type helios_string_type;

//! cast any generic helios object to a helios_string object.
#define TO_HELIOS_STRING(obj) ((struct __helios_string_s *)obj)
//! return 1/true (c int) if obj is an instance of helios_string. Note:
//! subclasses aren't covered.
#define IS_HELIOS_STRING(obj) (obj->class == &helios_string_type)

// global export of string functions

void helios_string_delete(helios_object *obj);
helios_object *helios_string_init();
helios_object *helios_string_from_charp(char *value);
helios_object *helios_string_tostring(helios_object *obj);
helios_object *helios_string_represent(helios_object *obj);
char *helios_string_to_charp(helios_object *obj);
#endif
