#include <helios_float.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <stdio.h>

helios_object *helios_integer_add(helios_object *self, helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_integer_type.constructor();
        TO_HELIOS_INTEGER(res)->value =
            TO_HELIOS_INTEGER(self)->value + TO_HELIOS_INTEGER(other)->value;
        return res;
    } else if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = (double)TO_HELIOS_INTEGER(self)->value +
                                      TO_HELIOS_FLOAT(other)->value;
        return res;
    } else {
        printf("can only add number to integer\n");
        exit(-1);
    }
}

helios_object *helios_integer_subtract(helios_object *self,
                                       helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_integer_type.constructor();
        TO_HELIOS_INTEGER(res)->value =
            TO_HELIOS_INTEGER(self)->value - TO_HELIOS_INTEGER(other)->value;
        return res;
    } else if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = (double)TO_HELIOS_INTEGER(self)->value -
                                      TO_HELIOS_FLOAT(other)->value;
        return res;
    } else {
        printf("can only subtract number from integer\n");
        exit(-1);
    }
}

helios_object *helios_integer_divide(helios_object *self,
                                     helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = (double)TO_HELIOS_INTEGER(self)->value /
                                      (double)TO_HELIOS_INTEGER(other)->value;
        return res;
    } else if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = (double)TO_HELIOS_INTEGER(self)->value /
                                      TO_HELIOS_FLOAT(other)->value;
        return res;
    } else {
        printf("can only divide integer by a number\n");
        exit(-1);
    }
}

helios_object *helios_integer_multiply(helios_object *self,
                                       helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        helios_object *res = helios_integer_type.constructor();
        TO_HELIOS_INTEGER(res)->value =
            TO_HELIOS_INTEGER(self)->value * TO_HELIOS_INTEGER(other)->value;
        return res;
    } else if (IS_HELIOS_FLOAT(other)) {
        helios_object *res = helios_float_type.constructor();
        TO_HELIOS_FLOAT(res)->value = (double)TO_HELIOS_INTEGER(self)->value *
                                      TO_HELIOS_FLOAT(other)->value;
        return res;
    } else {
        printf("can only divide integer by a number\n");
        exit(-1);
    }
}