#include <helios_integer.h>
#include <helios_object.h>

helios_object *helios_integer_add(helios_object *self, helios_object *other) {
    helios_object *res = helios_integer_type.constructor();
    TO_HELIOS_INTEGER(res)->value =
        TO_HELIOS_INTEGER(self)->value + TO_HELIOS_INTEGER(other)->value;
    return res;
}

helios_object *helios_integer_subtract(helios_object *self,
                                       helios_object *other) {
    helios_object *res = helios_integer_type.constructor();
    TO_HELIOS_INTEGER(res)->value =
        TO_HELIOS_INTEGER(self)->value - TO_HELIOS_INTEGER(other)->value;
    return res;
}

helios_object *helios_integer_divide(helios_object *self,
                                     helios_object *other) {
    helios_object *res = helios_integer_type.constructor();
    TO_HELIOS_INTEGER(res)->value =
        TO_HELIOS_INTEGER(self)->value / TO_HELIOS_INTEGER(other)->value;
    return res;
}

helios_object *helios_integer_multiply(helios_object *self,
                                       helios_object *other) {
    helios_object *res = helios_integer_type.constructor();
    TO_HELIOS_INTEGER(res)->value =
        TO_HELIOS_INTEGER(self)->value * TO_HELIOS_INTEGER(other)->value;
    return res;
}