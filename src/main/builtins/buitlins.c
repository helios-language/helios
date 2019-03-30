
#include <builtins.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Ands two booleans (possibly casts to boolean). If the left side is false, the
 * boolean function on the right side is never called.
 *
 * TODO: is this something that's needed? might be a vm function.
 *
 * @param a the left side of the and operation
 * @param b the right side of the and operation
 * @return a and b
 */
helios_object *helios_lazy_and(helios_object *a, helios_object *b) {
    if (!HELIOS_HAS_MEMBER(boolean, b)) {
        printf("couldn't cast to boolean\n");
        exit(-1);
    }

    helios_object *a_bool = HELIOS_CALL_MEMBER(boolean, a);
    if (IS_HELIOS_BOOLEAN(a_bool)) {
        if (HELIOS_BOOLEAN_IS_FALSE(a_bool)) { // shortcut, dont eval right
            return helios_boolean_false;
        } else {
            if (!HELIOS_HAS_MEMBER(boolean, b)) {
                printf("couldn't cast to boolean\n");
                exit(-1);
            }

            helios_object *b_bool = HELIOS_CALL_MEMBER(boolean, b);
            if (IS_HELIOS_BOOLEAN(b_bool)) {
                if (HELIOS_BOOLEAN_IS_TRUE(b_bool)) {
                    return helios_boolean_true;
                } else {
                    return helios_boolean_false;
                }
            } else {
                printf("couldn't cast to boolean, invalid return value from "
                       "obj.boolean()\n");
                exit(-1);
            }
        }
    } else {
        printf("couldn't cast to boolean, invalid return value from "
               "obj.boolean()\n");
        exit(-1);
    }
}

/**
 * Ors two booleans (possibly casts to boolean). If the left side is true, the
 * boolean function on the right side is never called.
 *
 * TODO: is this something that's needed? might be a vm function.
 *
 * @param a the left side of the or operation
 * @param b the right side of the or operation
 * @return a or b
 */
helios_object *helios_lazy_or(helios_object *a, helios_object *b) {
    if (!HELIOS_HAS_MEMBER(boolean, a)) {
        printf("couldn't cast to boolean\n");
        exit(-1);
    }

    helios_object *a_bool = HELIOS_CALL_MEMBER(boolean, a);
    if (IS_HELIOS_BOOLEAN(a_bool)) {
        if (HELIOS_BOOLEAN_IS_TRUE(a_bool)) { // shortcut, dont eval right
            return helios_boolean_true;
        } else {
            if (!HELIOS_HAS_MEMBER(boolean, b)) {
                printf("couldn't cast to boolean\n");
                exit(-1);
            }

            helios_object *b_bool = HELIOS_CALL_MEMBER(boolean, b);
            if (IS_HELIOS_BOOLEAN(b_bool)) {
                if (HELIOS_BOOLEAN_IS_TRUE(b_bool)) {
                    return helios_boolean_true;
                } else {
                    return helios_boolean_false;
                }
            } else {
                printf("couldn't cast to boolean, invalid return value from "
                       "obj.boolean()\n");
                exit(-1);
            }
        }
    } else {
        printf("couldn't cast to boolean, invalid return value from "
               "obj.boolean()\n");
        exit(-1);
    }
}

/**
 * calculates the inverse of a boolean or an object cast to a boolean with it's
 * boolean member function
 *
 * TODO: is this something that's needed? might be a vm function.
 *
 * @param obj the boolean to invert
 * @return not(obj)
 */
helios_object *helios_not(helios_object *obj) {
    if (!HELIOS_HAS_MEMBER(boolean, obj)) {
        printf("couldn't cast to boolean\n");
        exit(-1);
    }

    helios_object *obj_bool = HELIOS_CALL_MEMBER(boolean, obj);
    if (IS_HELIOS_BOOLEAN(obj_bool)) {
        if (HELIOS_BOOLEAN_IS_TRUE(obj_bool)) {
            return helios_boolean_false;
        }
        return helios_boolean_true;
    } else {
        printf("couldn't cast to boolean, invalid return value from "
               "obj.boolean()\n");
        exit(-1);
    }
}

/**
 * In rich comparison some assumptions are made. For example: in comparisons
 * like a == b, a and b can be swapped. So this is equal to b == a. Similarly a
 * > b --> b < a and the same holds for a >= b --> b <= a. What however is not
 * assumed is that a == b is eqal to not(a != b). This might be true for
 * integers but not necessarily for any type. Similarly a > b --> not(a <= b) is
 * not assumed.
 */

/**
 * Checks if two objects are equal to eachother. Shortcuts when the
 * references to self and other are the same.
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
        printf("could not determine comparison.\n");
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
        printf("could not determine comparison.\n");
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
        printf("could not determine comparison.\n");
        exit(-1);
    }
}

helios_object *helios_notequal(helios_object *self, helios_object *other) {
    if (self == other) { // exact copy / pointer compare
        return helios_boolean_false;
    }

    else if (HELIOS_HAS_MEMBER(notequal, self)) {
        // return a != b
        return HELIOS_CALL_MEMBER(notequal, self, other);
    } else if (HELIOS_HAS_MEMBER(notequal, other)) {
        // return b != a
        return HELIOS_CALL_MEMBER(notequal, other, self);
    } else {
        printf("could not determine comparison.\n");
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
        printf("could not determine comparison.\n");
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
        printf("could not determine comparison.\n");
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