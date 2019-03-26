
#include <helios_boolean.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * Create a helios_boolean object from a c integer type. Alternate constructor
 * for helios_boolean.
 *
 * @param value the value contained in the newly created helios_boolean object
 * @return a new helios_boolean object.
 */
helios_object *helios_boolean_from_cint(int32_t value) {
    if (value) {
        return helios_boolean_true;
    }
    return helios_boolean_false;
}

/**
 * Create a helios_boolean object from a c boolean type. Alternate constructor
 * for helios_boolean.
 *
 * @param value the value contained in the newly created helios_boolean object
 * @return a new helios_boolean object.
 */
helios_object *helios_boolean_from_cbool(bool value) {
    return helios_boolean_from_cint((int32_t)value);
}

/**
 * Destructor of a helios_boolean object. This is only here for compatibility.
 * It doesnt do anything as there are only 2 booleans in existence, True and
 * False.
 *
 * @param self the object to destruct.
 */
void helios_boolean_delete(helios_object *self) {
}

/**
 * Constructor of a helios_boolean object.
 * The value is set to HELIOS_BOOLEAN_DEFAULT_VALUE defined in helios_boolean.h
 *
 * @return a new helios_boolean object.
 */
helios_object *helios_boolean_init() {
    return HELIOS_BOOLEAN_DEFAULT_VALUE;
}

/**
 * Create a helios_string object which represents the helios_boolean object's
 * value.
 *
 * @param self helios_boolean object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_boolean
 */
helios_object *helios_boolean_tostring(helios_object *self) {
    if (HELIOS_BOOLEAN_IS_TRUE(self)) {
        return helios_string_from_charp("True");
    }
    return helios_string_from_charp("False");
}

/**
 * Alias of helios_boolean_tostring for completeness.
 *
 * @param self helios_boolean object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_boolean
 */
helios_object *helios_boolean_represent(helios_object *self) {
    return helios_boolean_tostring(self);
}

/**
 * Copy a helios integer object.
 *
 * @param obj the object to copy
 * @return a new helios_boolean with the same value
 */
helios_object *helios_boolean_copy(helios_object *obj) {
    return obj;
}

/**
 * Definition of the type of any helios_boolean object.
 */
helios_type helios_boolean_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_boolean_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_boolean_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Boolean"),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_boolean_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_boolean_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_COPY(helios_boolean_copy),

    HELIOS_OBJECT_BINOPS_FIELDS_NULL(),

    HELIOS_OBJECT_STATIC(),
};

helios_boolean helios_boolean_false_real = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_boolean_type),
    .value = false,
};

helios_boolean helios_boolean_true_real = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_boolean_type),
    .value = true,
};

helios_object *helios_boolean_false =
    (helios_object *)&helios_boolean_false_real;
helios_object *helios_boolean_true = (helios_object *)&helios_boolean_true_real;
