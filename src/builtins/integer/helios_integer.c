
#include <helios_integer.h>
#include <helios_object.h>

helios_object *helios_integer_add(helios_object *self, helios_object *other) {
}

helios_object *helios_integer_subtract(helios_object *self,
                                       helios_object *other) {
}

helios_object *helios_integer_divide(helios_object *self,
                                     helios_object *other) {
}

helios_object *helios_integer_multiply(helios_object *self,
                                       helios_object *other) {
}

void helios_integer_delete(helios_object *obj) {
}

helios_object *helios_integer_init() {
    helios_integer *self = malloc(sizeof(helios_integer));
    *self =
        (helios_integer){HELIOS_OBJECT_COMMON_BASE_INIT(&helios_integer_type),
                         .value = HELIOS_INTEGER_DEFAULT_VALUE};
    return TO_HELIOS_OBJECT(self);
}

helios_object helios_integer_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_integer_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_integer_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("Integer"),

    HELIOS_OBJECT_BINOPS_FIELDS_ADD(helios_integer_add),
    HELIOS_OBJECT_BINOPS_FIELDS_SUBTRACT(helios_integer_subtract),
    HELIOS_OBJECT_BINOPS_FIELDS_DIVIDE(helios_integer_divide),
    HELIOS_OBJECT_BINOPS_FIELDS_MULTIPLY(helios_integer_multiply),
};