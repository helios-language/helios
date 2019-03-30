#include <helios_boolean.h>
#include <helios_float.h>
#include <helios_integer.h>
#include <helios_object.h>
#include <math.h>
#include <stdio.h>

helios_object *helios_boolean_equal(helios_object *self, helios_object *other) {
    return helios_integer_equal(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_less(helios_object *self, helios_object *other) {
    return helios_integer_less(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_greater(helios_object *self,
                                      helios_object *other) {
    return helios_integer_greater(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_notequal(helios_object *self,
                                       helios_object *other) {
    return helios_integer_notequal(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_greaterequal(helios_object *self,
                                           helios_object *other) {
    return helios_integer_greaterequal(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_lessequal(helios_object *self,
                                        helios_object *other) {
    return helios_integer_lessequal(helios_boolean_integer(self), other);
}

helios_object *helios_boolean_boolean(helios_object *self) {
    return self;
}