
#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

helios_object *helios_equal(helios_object *self, helios_object *other);
helios_object *helios_greater(helios_object *self, helios_object *other);
helios_object *helios_less(helios_object *self, helios_object *other);
helios_object *helios_notequal(helios_object *self, helios_object *other);
helios_object *helios_greaterequal(helios_object *self, helios_object *other);
helios_object *helios_lessequal(helios_object *self, helios_object *other);

/**
 * Simple function to hash any helios object. Assuming the object has a hash
 * function, use it. if it's NULL: calculate a hash from it's address.
 *
 * @param obj the object to hash
 * @param
 */
helios_object *helios_hash(helios_object *obj) {
    if (HELIOS_HAS_MEMBER(hash, obj)) {
        return HELIOS_CALL_MEMBER(hash, obj);
    }
    return helios_integer_from_cint((int64_t)obj);
}