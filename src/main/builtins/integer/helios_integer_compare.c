#include <helios_boolean.h>
#include <helios_float.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <math.h>
#include <stdio.h>

helios_object *helios_integer_equal(helios_object *self, helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value ==
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        double value = TO_HELIOS_FLOAT(other)->value;
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) ==
                                             TO_HELIOS_INTEGER(self)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_equal(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_less(helios_object *self, helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value <
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value <
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_less(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_greater(helios_object *self,
                                      helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value >
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value >
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_greater(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_notequal(helios_object *self,
                                       helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value !=
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        double value = TO_HELIOS_FLOAT(other)->value;
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) !=
                                             TO_HELIOS_INTEGER(self)->value);
        } else {
            return helios_boolean_true;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_notequal(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_greaterequal(helios_object *self,
                                           helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value >=
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        // for floats its easier to check if it's not
        // smaller to get GE as this is just one check and
        // doesnt need the ceil stuff needed for equals.
        return helios_boolean_from_cbool(
            !(TO_HELIOS_INTEGER(self)->value < TO_HELIOS_FLOAT(other)->value));
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_greaterequal(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_lessequal(helios_object *self,
                                        helios_object *other) {
    if (IS_HELIOS_INTEGER(other)) {
        return helios_boolean_from_cbool(TO_HELIOS_INTEGER(self)->value >=
                                         TO_HELIOS_INTEGER(other)->value);
    } else if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(
            !(TO_HELIOS_INTEGER(self)->value > TO_HELIOS_FLOAT(other)->value));
    } else if (IS_HELIOS_BOOLEAN(other)) {
        return helios_integer_lessequal(self, helios_boolean_integer(other));
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_integer_boolean(helios_object *self) {
    if (TO_HELIOS_INTEGER(self)->value == 0) {
        return helios_boolean_false;
    }
    return helios_boolean_true;
}