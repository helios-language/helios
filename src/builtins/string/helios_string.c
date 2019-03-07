
#include <helios_object.h>
#include <helios_string.h>

helios_object *helios_string_from_charp(char *value) {
    helios_object *res = helios_string_init();
    TO_HELIOS_STRING(res)->value = value;
    return res;
}

void helios_string_delete(helios_object *obj) {
    free(TO_HELIOS_STRING(obj)->value);
    free(TO_HELIOS_STRING(obj));
}

helios_object *helios_string_init() {
    helios_string *self = malloc(sizeof(helios_string));
    *self = (helios_string){HELIOS_OBJECT_COMMON_BASE_INIT(&helios_string_type),
                            .size = HELIOS_STRING_DEFAULT_SIZE, .filled = 0,
                            .value = malloc(HELIOS_STRING_DEFAULT_SIZE)};
    return TO_HELIOS_OBJECT(self);
}

helios_type helios_string_type = {
    HELIOS_OBJECT_COMMON_BASE_INIT(&helios_object_type),

    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_string_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_string_delete),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("string"),
};
