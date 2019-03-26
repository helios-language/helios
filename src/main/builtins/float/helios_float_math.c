#include <helios_float.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <stdio.h>

helios_object *helios_float_add(helios_object *self, helios_object *other) {
    if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value =
            TO_HELIOS_FLOAT(self)->value + TO_HELIOS_FLOAT(other)->value;
        return res;
    } else if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = TO_HELIOS_FLOAT(self)->value +
                                      (double)TO_HELIOS_INTEGER(other)->value;
        return res;
    } else {
        // ERROR system
        printf("can only add number to float\n");
        exit(-1);
    }
}

helios_object *helios_float_subtract(helios_object *self,
                                     helios_object *other) {
    if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value =
            TO_HELIOS_FLOAT(self)->value - TO_HELIOS_FLOAT(other)->value;
        return res;
    } else if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = TO_HELIOS_FLOAT(self)->value -
                                      (double)TO_HELIOS_INTEGER(other)->value;
        return res;
    } else {
        // ERROR system
        printf("can only subtract number from float\n");
        exit(-1);
    }
}

helios_object *helios_float_divide(helios_object *self, helios_object *other) {
    if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value =
            TO_HELIOS_FLOAT(self)->value / TO_HELIOS_FLOAT(other)->value;
        return res;
    } else if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = TO_HELIOS_FLOAT(self)->value /
                                      (double)TO_HELIOS_INTEGER(other)->value;
        return res;
    } else {
        // ERROR system
        printf("can only divide float by a number\n");
        exit(-1);
    }
}

helios_object *helios_float_multiply(helios_object *self,
                                     helios_object *other) {
    if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value =
            TO_HELIOS_FLOAT(self)->value * TO_HELIOS_FLOAT(other)->value;
        return res;
    } else if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = TO_HELIOS_FLOAT(self)->value *
                                      (double)TO_HELIOS_INTEGER(other)->value;
        return res;
    } else {
        // ERROR system
        printf("can only multiply float by a number\n");
        exit(-1);
    }
}