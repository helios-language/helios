
#include <helios_float.h>
#include <helios_memory.h>
#include <helios_object.h>
#include <helios_string.h>
#include <math.h>
#include <stdio.h>

// typedef struct {
//     uint64_t mantisa : 52;
//     uint64_t exponent : 11;
//     uint64_t sign : 1;
// } doubleparts;

// typedef union {
//     double f;
//     doubleparts d;
// } doublehelper;

// helios_object *helios_float_from_ife(uint32_t intpart, uint32_t floatpart,
//                                      uint32_t exponent) {

//     doublehelper d;
// }

/**
 * Create a helios_float object from a c double type. Alternate constructor
 * for helios_float.
 *
 * @param value the value contained in the newly created helios_float object
 * @return a new helios_float object.
 */
helios_object *helios_float_from_cdouble(double value) {
    helios_object *res = helios_float_init();
    TO_HELIOS_FLOAT(res)->value = value;
    return res;
}

/**
 * Destructor of a helios_float object.
 *
 * @param obj the object to destruct.
 */
void helios_float_delete(helios_object *self) {
    helios_free_object(self);
}

/**
 * Constructor of a helios_float object.
 * The value is set to HELIOS_float_DEFAULT_VALUE defined in helios_float.h
 *
 * @return a new helios_float object.
 */
helios_object *helios_float_init() {
    helios_float *self =
        (helios_float *)helios_allocate_object(sizeof(helios_float));
    *self = (helios_float){HELIOS_OBJECT_COMMON_BASE_INIT(&helios_float_type),
                           .value = HELIOS_FLOAT_DEFAULT_VALUE};
    helios_set_garbagecollectable(TO_HELIOS_OBJECT(self));

    return TO_HELIOS_OBJECT(self);
}

/**
 * Create a helios_string object which represents the helios_float object's
 * value.
 *
 * @param a helios_float object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_float
 */
helios_object *helios_float_tostring(helios_object *self) {
    uint32_t digits = floor(log10(abs(TO_HELIOS_FLOAT(self)->value))) + 2;
    char str[digits];
    sprintf(str, "%lf", TO_HELIOS_FLOAT(self)->value);
    return helios_string_from_charp(str);
}

/**
 * Alias of helios_float_tostring for completeness.
 *
 * @param a helios_float object to stringify.
 * @return a helios_string object with the string representation of the
 * helios_float
 */
helios_object *helios_float_represent(helios_object *self) {
    return helios_float_tostring(self);
}

/**
 * Definition of the type of any helios_float object.
 */
helios_type helios_float_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_float_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_float_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Float"),
    HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(helios_float_tostring),
    HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(helios_float_tostring),

    HELIOS_OBJECT_BINOPS_FIELDS_ADD(helios_float_add),
    HELIOS_OBJECT_BINOPS_FIELDS_SUBTRACT(helios_float_subtract),
    HELIOS_OBJECT_BINOPS_FIELDS_DIVIDE(helios_float_divide),
    HELIOS_OBJECT_BINOPS_FIELDS_MULTIPLY(helios_float_multiply),
};