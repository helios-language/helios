
#include <helios_object.h>
#include <helios_string.h>
#include <string.h>

/**
 * Add a character to a helios_string object.
 * WARNING: only for internal use. Helios strings are immutable.
 *
 * @param obj the string object to add a char to.
 * @param c the character to add.
 */
static void helios_string_append_char(helios_object *obj, char c) {
    helios_string *strobj = TO_HELIOS_STRING(obj);
    strobj->value[strobj->filled++] = c;
    if (strobj->filled >= strobj->size) {
        strobj->size <<= 1;
        strobj->value = realloc(strobj->value, strobj->size * sizeof(char));
    }
}

/**
 * Create a helios_string object from a c string. Alternate constructor for
 * helios_string.
 *
 * @param value the value the string must be initialized to. NOTE: this value is
 * copied into the new helios_string object and can/must be freed without the
 * helios_string object breaking. Similarly the helios_string object can be
 * destructed without the c string to break.
 *
 * @return a new helios_string object with in it the c string value.
 */
helios_object *helios_string_from_charp(char *value) {
    helios_object *res = helios_string_init();
    uint32_t length = strlen(value); // precalculating to increase speed

    for (uint32_t i = 0; i < length; i++) {
        helios_string_append_char(res, value[i]);
    }
    return res;
}

/**
 * Create a cstring from a helios_string object.
 *
 * @param obj the object to convert to a cstring.
 * @return a cstring which represents the contents of the helios_string. NOTE:
 * the pointer returned is still tied to the helios_string object and can be
 * used for as long as the helios_string object is not destructed. When the
 * helios_string is destructed the c string will automatically be freed. Freeing
 * the returned string results in undefined behaviour and should not be
 * attempted.
 */
char *helios_string_to_charp(helios_object *obj) {
    return TO_HELIOS_STRING(obj)->value;
}

/**
 * Destructor of a helios_string object.
 *
 * Frees both the helios_object and the internal cstring
 *
 * @param obj the object to destruct.
 */
void helios_string_delete(helios_object *obj) {
    free(TO_HELIOS_STRING(obj)->value);
    free(TO_HELIOS_STRING(obj));
}

/**
 * Constructor of a helios_string.
 *
 * @return a newly constructed string object with no length and no contents.
 */
helios_object *helios_string_init() {
    helios_string *self = malloc(sizeof(helios_string));
    *self = (helios_string){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_string_type),
        .size = HELIOS_STRING_DEFAULT_SIZE, .filled = 0,
        .value = malloc(HELIOS_STRING_DEFAULT_SIZE * sizeof(char))};
    return TO_HELIOS_OBJECT(self);
}

/**
 * Converts a helios_string object to a string.
 *
 * Kinda redundant but we gotta stay consistent... Returns itself.
 * @param obj the object to convert to a string.
 * @return the string representation (self). NOTE: needs not be freed as it's
 * litterally a pointer to the same object.
 */
helios_object *helios_string_tostring(helios_object *obj) {
    return obj;
}

/**
 * Represents a helios_string in a way that could theoretically be
 * reinterpreted. Includes quotation marks.
 *
 * @param obj the object to represent
 * @return a representation of the helios_string. NOTE: has to be freed.
 */
helios_object *helios_string_represent(helios_object *obj) {
    helios_object *res = helios_string_init();
    helios_string_append_char(res, '\'');
    for (uint32_t i = 0; i < TO_HELIOS_STRING(obj)->filled; i++) {
        helios_string_append_char(res, TO_HELIOS_STRING(obj)->value[i]);
    }
    helios_string_append_char(res, '\'');

    return res;
}

/**
 * Definition of the type of any helios_string object.
 */
helios_type helios_string_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_string_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_string_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("string"),

    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_string_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_string_represent),
};
