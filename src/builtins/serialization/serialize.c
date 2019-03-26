#include <builtins.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

enum helios_serializable_datatypes {
    HELIOS_CODE,
    HELIOS_INT,
    HELIOS_STRING,
    HELIOS_FLOAT,
    HELIOS_HASHMAP,
};

void helios_serialized_object_print(helios_object *self) {
    if (IS_HELIOS_SERIALIZED_OBJECT(self)) {
        helios_serialized_object *obj = TO_HELIOS_SERIALIZED_OBJECT(self);
        for (uint32_t i = 0; i < obj->filled; i++) {
            printf(" 0x%02" PRIx8, obj->data[i]);
        }
        printf("\n");

    } else {
        printf("can only print helios_serialized_object\n");
        exit(-1);
    }
}

static void helios_serialized_object_add_byte(helios_object *self,
                                              uint8_t data) {
    helios_serialized_object *obj = TO_HELIOS_SERIALIZED_OBJECT(self);
    obj->data[obj->filled++] = data;
    if (obj->filled >= obj->size) {
        obj->size <<= 1;
        obj->data = realloc(obj->data, sizeof(uint8_t) * obj->size);
    }
}

static void helios_serialized_object_add_int(helios_object *self,
                                             uint32_t data) {
    helios_serialized_object_add_byte(self, (data >> 24) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 16) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 8) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 0) & 0xff);
}

static void helios_serialized_object_add_long(helios_object *self,
                                              uint64_t data) {
    helios_serialized_object_add_byte(self, (data >> 56) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 48) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 40) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 32) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 24) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 16) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 8) & 0xff);
    helios_serialized_object_add_byte(self, (data >> 0) & 0xff);
}

// static helios_object *helios_serialized_object_from_range(helios_object *old,
//                                                           uint32_t start,
//                                                           uint32_t end) {
//     helios_object *res = helios_serialized_object_init();
//     if (start < 0) {
//         start = 0;
//     }
//     if (end >= TO_HELIOS_SERIALIZED_OBJECT(old)->filled) {
//         end = TO_HELIOS_SERIALIZED_OBJECT(old)->filled - 1;
//     }
//     for (uint32_t i = start; i < end; i++) {
//         helios_serialized_object_add_byte(
//             res, TO_HELIOS_SERIALIZED_OBJECT(old)->data[i]);
//     }
//     return res;
// }

helios_object *helios_deserialize_object(helios_object *self) {
    if (IS_HELIOS_SERIALIZED_OBJECT(self)) {
        uint32_t index = 0;
        helios_serialized_object *obj = TO_HELIOS_SERIALIZED_OBJECT(self);
        enum helios_serializable_datatypes helios_object_type =
            obj->data[index++];
        helios_object *retval = NULL;

        switch (helios_object_type) {
        case HELIOS_CODE:;
            break;
        case HELIOS_INT:;
            uint64_t resint = 0;
            for (; index < obj->filled; index++) {
                resint = (resint << 8) | obj->data[index];
            }
            retval = helios_integer_from_cint(resint);
            break;
        case HELIOS_STRING:;
            break;
        case HELIOS_FLOAT:;
            uint64_t resdouble = 0;
            for (; index < obj->filled; index++) {
                resdouble = (resdouble << 8) | obj->data[index];
            }
            union {
                uint64_t i;
                double d;
            } d = {.i = resdouble};
            retval = helios_float_from_cdouble(d.d);
            break;
        case HELIOS_HASHMAP:;
            break;
        default:;
            printf("Cant deserialize this object type");
            exit(-1);
            break;
        }

        return retval;
    } else {
        printf("object is not a serialized object\n");
        exit(-1);
    }
}

helios_object *helios_serialize_object(helios_object *obj) {
    helios_object *retval = helios_serialized_object_init();

    if (IS_HELIOS_CODE(obj)) {
        return NULL;
    } else if (IS_HELIOS_FLOAT(obj)) {
        helios_serialized_object_add_byte(retval, (uint8_t)HELIOS_FLOAT);
        // union a double and int together so their bits lie in the same
        // memory area.
        union {
            uint64_t i;
            double d;
        } d = {.d = TO_HELIOS_FLOAT(obj)->value};

        helios_serialized_object_add_long(retval, d.i);
    } else if (IS_HELIOS_HASHMAP(obj)) {
        return NULL;
    } else if (IS_HELIOS_INTEGER(obj)) {
        helios_serialized_object_add_byte(retval, (uint8_t)HELIOS_INT);
        helios_serialized_object_add_int(retval, TO_HELIOS_INTEGER(obj)->value);
    } else if (IS_HELIOS_STRING(obj)) {
        helios_serialized_object_add_byte(retval, (uint8_t)HELIOS_STRING);
        helios_serialized_object_add_int(retval, TO_HELIOS_STRING(obj)->filled);
        for (uint32_t i = 0; i < TO_HELIOS_STRING(obj)->filled; i++) {
            helios_serialized_object_add_byte(retval,
                                              TO_HELIOS_STRING(obj)->value[i]);
        }
    } else if (IS_HELIOS_HASHMAP(obj)) {
        return NULL;
    } else {
        printf("object not serializable\n");
        exit(-1);
    }
    return retval;
}

helios_object *helios_serialized_object_init() {
    helios_serialized_object *self =
        (helios_serialized_object *)helios_allocate_object(
            sizeof(helios_serialized_object));
    *self = (helios_serialized_object){
        HELIOS_OBJECT_COMMON_BASE_INIT(&helios_serialized_object_type),
        .filled = 0,
        .size = HELIOS_SERIALIZED_OBJECT_DEFAULT_SIZE,
        .data = malloc(HELIOS_SERIALIZED_OBJECT_DEFAULT_SIZE * sizeof(uint8_t)),
    };
    helios_set_garbagecollectable(TO_HELIOS_OBJECT(self));

    return TO_HELIOS_OBJECT(self);
}

void helios_serialized_object_delete(helios_object *self) {
    helios_free_object(self);
}

helios_type helios_serialized_object_type = {
    HELIOS_OBJECT_BASIC_FIELDS_NULL(),
    HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME("helios serialized object"),
    HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(helios_serialized_object_init),
    HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(helios_serialized_object_delete),
};