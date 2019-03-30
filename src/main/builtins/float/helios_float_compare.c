#include <helios_boolean.h>
#include <helios_float.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <math.h>
#include <stdio.h>

helios_object *helios_float_equal(helios_object *self, helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value ==
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) ==
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) ==
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_less(helios_object *self, helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value < TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) <
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) <
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_greater(helios_object *self, helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value > TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) >
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) >
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_notequal(helios_object *self,
                                     helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value !=
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) !=
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) !=
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_greaterequal(helios_object *self,
                                         helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value >=
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) >=
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) >=
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_lessequal(helios_object *self,
                                      helios_object *other) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if (IS_HELIOS_FLOAT(other)) {
        return helios_boolean_from_cbool(value <=
                                         TO_HELIOS_FLOAT(other)->value);
    } else if (IS_HELIOS_INTEGER(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(ceil(value) <=
                                             TO_HELIOS_INTEGER(other)->value);
        } else {
            return helios_boolean_false;
        }
    } else if (IS_HELIOS_BOOLEAN(other)) {
        if (ceil(value) == value) {
            return helios_boolean_from_cbool(
                ceil(value) <=
                TO_HELIOS_INTEGER(helios_boolean_integer(other))->value);
        } else {
            return helios_boolean_false;
        }
    } else {
        return helios_boolean_false;
    }
}

helios_object *helios_float_boolean(helios_object *self) {
    double value = TO_HELIOS_FLOAT(self)->value;
    if ((ceil(value) == value && ceil(value) == 0) || value == 0) {
        return helios_boolean_false;
    }
    return helios_boolean_true;
}