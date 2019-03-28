
#include <helios_integer.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <math.h>
#include <stdio.h>

/**
 * Create a helios_integer object from a c integer type. Alternate constructor
 * for helios_integer.
 *
 * @param value the value contained in the newly created helios_integer object
 * @return a new helios_integer object.
 */
helios_object *helios_integer_from_cint(int64_t value) {
    helios_object *res = helios_integer_init();
    TO_HELIOS_INTEGER(res)->value = value;
    return res;
}

/**
 * Extract the c integer (long) from a helios object.
 *
 * @param obj the object to extract the int outof
 * @return the cint
 */
int64_t helios_integer_to_cint(helios_object *obj) {
    if (!IS_HELIOS_INTEGER(obj)) {
        printf("has to be helios integer\n");
        exit(-1);
    }
    return TO_HELIOS_INTEGER(obj)->value;
}

/**
 * Destructor of a helios_integer object.
 *
 * @param self the object to destruct.
 */
void helios_integer_delete(helios_object *self) {
    helios_free_object(self);
}

/**
 * Constructor of a helios_integer object.
 * The value is set to HELIOS_INTEGER_DEFAULT_VALUE defined in helios_integer.h
 *
 * @return a new helios_integer object.
 */
helios_object *helios_integer_init() {
    helios_integer *self =
        (helios_integer *)helios_allocate_object(sizeof(helios_integer));
    *self =
        (helios_integer){HELIOS_OBJECT_COMMON_BASE_INIT(&helios_integer_type),
                         .value = HELIOS_INTEGER_DEFAULT_VALUE};
    helios_set_garbagecollectable(TO_HELIOS_OBJECT(self));

    return TO_HELIOS_OBJECT(self);
}

/**
 * Create a helios_string object which represents the helios_integer object's
 * value.
 *
 * @param self helios_integer object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_integer
 */
helios_object *helios_integer_tostring(helios_object *self) {
    int64_t digits = floor(log10(abs(TO_HELIOS_INTEGER(self)->value))) + 2;
    char str[digits];
    sprintf(str, "%ld", TO_HELIOS_INTEGER(self)->value);
    return helios_string_from_charp(str);
}

/**
 * Alias of helios_integer_tostring for completeness.
 *
 * @param self helios_integer object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_integer
 */
helios_object *helios_integer_represent(helios_object *self) {
    return helios_integer_tostring(self);
}

/**
 * Copy a helios integer object.
 *
 * @param obj the object to copy
 * @return a new helios_integer with the same value
 */
helios_object *helios_integer_copy(helios_object *obj) {
    helios_integer *old = TO_HELIOS_INTEGER(obj);
    helios_integer *new = TO_HELIOS_INTEGER(helios_integer_init());
    new->value = old->value;
    return TO_HELIOS_OBJECT(new);
}

/**
 * Definition of the type of any helios_integer object.
 */
helios_type helios_integer_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_integer_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_integer_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Integer"),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_integer_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_integer_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_COPY(helios_integer_copy),

    HELIOS_OBJECT_BINOPS_FIELDS_ADD(helios_integer_add),
    HELIOS_OBJECT_BINOPS_FIELDS_SUBTRACT(helios_integer_subtract),
    HELIOS_OBJECT_BINOPS_FIELDS_DIVIDE(helios_integer_divide),
    HELIOS_OBJECT_BINOPS_FIELDS_MULTIPLY(helios_integer_multiply),

};