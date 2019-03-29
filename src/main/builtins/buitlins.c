
#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * In rich comparison some assumptions are made. For example: in comparisons
 * like a == b, a and b can be swapped. So this is equal to b == a. Similarly a
 * > b --> b < a and the same holds for a >= b --> b <= a. What however is not
 * assumed is that a == b is eqal to not(a != b). This might be true for
 * integers but not necessarily for any type. Similarly a > b --> not(a <= b) is
 * not assumed.
 */

/**
 * Checks if two objects are equal to eachother. Shortcuts when the references
 * to self and other are the same.
 *
 * @param self
 * @param other
 * @return a boolean saying if they are equal.
 */
helios_object *helios_equal(helios_object *self, helios_object *other) {
    if (self == other) { // exact copy / pointer compare
        return helios_boolean_true;
    }

    else if (HELIOS_HAS_MEMBER(equal, self)) {
        // return a == b
        return HELIOS_CALL_MEMBER(equal, self, other);
    } else if (HELIOS_HAS_MEMBER(equal, other)) {
        // return b == a
        return HELIOS_CALL_MEMBER(equal, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

helios_object *helios_greater(helios_object *self, helios_object *other) {
    if (HELIOS_HAS_MEMBER(greater, self)) {
        // return a > b
        return HELIOS_CALL_MEMBER(greater, self, other);
    } else if (HELIOS_HAS_MEMBER(less, other)) {
        // return b < a
        return HELIOS_CALL_MEMBER(less, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

helios_object *helios_less(helios_object *self, helios_object *other) {
    if (HELIOS_HAS_MEMBER(less, self)) {
        // return a < b
        return HELIOS_CALL_MEMBER(less, self, other);
    } else if (HELIOS_HAS_MEMBER(greater, other)) {
        // return b > a
        return HELIOS_CALL_MEMBER(greater, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

helios_object *helios_notequal(helios_object *self, helios_object *other) {
    if (HELIOS_HAS_MEMBER(notequal, self)) {
        // return a != b
        return HELIOS_CALL_MEMBER(notequal, self, other);
    } else if (HELIOS_HAS_MEMBER(notequal, other)) {
        // return b != a
        return HELIOS_CALL_MEMBER(notequal, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

helios_object *helios_greaterequal(helios_object *self, helios_object *other) {
    if (HELIOS_HAS_MEMBER(greaterequal, self)) {
        // return a >= b
        return HELIOS_CALL_MEMBER(greaterequal, self, other);
    } else if (HELIOS_HAS_MEMBER(lessequal, other)) {
        // return b <= a
        return HELIOS_CALL_MEMBER(lessequal, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

helios_object *helios_lessequal(helios_object *self, helios_object *other) {
    if (HELIOS_HAS_MEMBER(lessequal, self)) {
        // return a <= b
        return HELIOS_CALL_MEMBER(lessequal, self, other);
    } else if (HELIOS_HAS_MEMBER(greaterequal, other)) {
        // return b >= a
        return HELIOS_CALL_MEMBER(greaterequal, other, self);
    } else {
        printf("could not determine comparison.");
        exit(-1);
    }
}

/**
 * Simple function to hash any helios object. Assuming the object has a hash
 * function, use it. if it's NULL: calculate a hash from it's address.
 *
 * @param obj the object to hash
 * @return the hash of this object
 */
helios_object *helios_hash(helios_object *obj) {
    if (HELIOS_HAS_MEMBER(hash, obj)) {
        return HELIOS_CALL_MEMBER(hash, obj);
    }
    return helios_integer_from_cint((int64_t)obj);
}

/**
 * Copy of helios_hash for internal use. Returns a c integer.
 *
 * @param obj the object to hash
 * @return the hash of the object as cint
 */
int64_t helios_hash_cint(helios_object *obj) {
    if (HELIOS_HAS_MEMBER(hash, obj)) {
        helios_object *hashobj = HELIOS_CALL_MEMBER(hash, obj);
        int64_t hash = helios_integer_to_cint(hashobj);
        HELIOS_DECREF(hashobj);
        return hash;
    }
    return (int64_t)obj;
}