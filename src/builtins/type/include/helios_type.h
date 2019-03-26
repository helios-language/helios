
#ifndef HELIOS_TYPE_H
#define HELIOS_TYPE_H

#include <helios_memory.h>
#include <stdint.h>
#include <stdlib.h>

// forward declaration
struct __helios_type_s;

/**
 * WARNING: Every helios object class must have HELIOS_OBJECT_COMMON_BASE as its
 * *FIRST* FIELD. This is very important for casting to work.
 */
#define HELIOS_OBJECT_COMMON_BASE                                              \
    struct __helios_type_s *class;                                             \
    garbagecollector *gc;                                                      \
    uint32_t refcount

/**
 * Initializes the HELIOS_OBJECT_COMMON_BASE field of any helios object.
 */
#define HELIOS_OBJECT_COMMON_BASE_INIT(_class) .refcount = 0, .class = _class

// definitions of common sets of class fields

/**
 * This macro defines the most basic fields an object can have: a name,
 * constructor, destructor and tostring method.
 */
#define HELIOS_OBJECT_BASIC_FIELDS                                             \
    struct __helios_object_s *(*constructor)();                                \
    void (*destructor)(struct __helios_object_s * obj);                        \
    char *classname;                                                           \
    struct __helios_object_s *(*represent)(struct __helios_object_s * obj);    \
    struct __helios_object_s *(*tostring)(struct __helios_object_s * obj);     \
    struct __helios_object_s *(*copy)(struct __helios_object_s * obj);

/**
 * Initializes all HELIOS_OBJECT_BASIC_FIELDS to NULL. Technically not needed as
 * struct fields are nulled automatically. But it does make stuff much clearer.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_NULL()                                      \
    .constructor = NULL, .destructor = NULL, .classname = NULL,                \
    .tostring = NULL, .represent = NULL, .copy = NULL

/**
 * Set the constructor in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(function) .constructor = function
/**
 * Set the destructor in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(function) .destructor = function
/**
 * Set the classname in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME(name) .classname = name
/**
 * Set the tostring function in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_TOSTRING(function) .tostring = function
/**
 * Set the represent function in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_REPRESENT(function) .represent = function
/**
 * Set the copy function in a struct initialization.
 */
#define HELIOS_OBJECT_BASIC_FIELDS_COPY(function) .copy = function

/**
 * Define the fields corresponding to binary number operators for helios object
 * struct definitions.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS                                            \
    struct __helios_object_s *(*add)(struct __helios_object_s * self,          \
                                     struct __helios_object_s * other);        \
    struct __helios_object_s *(*subtract)(struct __helios_object_s * self,     \
                                          struct __helios_object_s * other);   \
    struct __helios_object_s *(*divide)(struct __helios_object_s * self,       \
                                        struct __helios_object_s * other);     \
    struct __helios_object_s *(*multiply)(struct __helios_object_s * self,     \
                                          struct __helios_object_s * other);   \
    struct __helios_object_s *(*modulo)(struct __helios_object_s * self,       \
                                        struct __helios_object_s * other);     \
    struct __helios_object_s *(*power)(struct __helios_object_s * self,        \
                                       struct __helios_object_s * other);      \
    struct __helios_object_s *(*floordivide)(                                  \
        struct __helios_object_s * self, struct __helios_object_s * other);

/**
 * Zero out all HELIOS_OBJECT_BINOPS_FIELDS. Technically not needed as
 * struct fields are nulled automatically. But it does make stuff much
 * clearer.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_NULL()                                     \
    .add = NULL, .subtract = NULL, .divide = NULL, .multiply = NULL,           \
    .modulo = NULL, .power = NULL, .floordivide = NULL

/**
 * Set the add function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_ADD(function) .add = function
/**
 * Set the subtract function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_SUBTRACT(function) .subtract = function
/**
 * Set the divide function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_DIVIDE(function) .divide = function
/**
 * Set the multiply function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_MULTIPLY(function) .multiply = function
/**
 * Set the modulo function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_MODULO(function) .modulo = function
/**
 * Set the power function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_POWER(function) .power = function
/**
 * Set the floordivide function in a struct initialization.
 */
#define HELIOS_OBJECT_BINOPS_FIELDS_FLOORDIVIDE(function)                      \
    .floordivide = function

/**
 * Define the fields corresponding to unary number operators for helios object
 * struct definitions.
 */
#define HELIOS_OBJECT_UNARYOPS_FIELDS                                          \
    struct __helios_object_s *(*minus)(struct __helios_object_s * self);       \
    struct __helios_object_s *(*invert)(struct __helios_object_s * self);      \
    struct __helios_object_s *(*plus)(struct __helios_object_s * self)

/**
 * Zero out all HELIOS_OBJECT_UNARYOPS_FIELDS. Technically not needed as
 * struct fields are nulled automatically. But it does make stuff much
 * clearer.
 */
#define HELIOS_OBJECT_UNARYOPS_FIELDS_NULL()                                   \
    .minus = NULL, .invert = NULL, .plus = NULL

/**
 * Set the minus function in a struct initialization.
 */
#define HELIOS_OBJECT_UNARYOPS_FIELDS_MINUS(function) .minus = function

/**
 * Set the invert function in a struct initialization.
 */
#define HELIOS_OBJECT_UNARYOPS_FIELDS_INVERT(function) .invert = function

/**
 * Set the plus function in a struct initialization.
 */
#define HELIOS_OBJECT_UNARYOPS_FIELDS_PLUS(function) .plus = function

/**
 * If this field is true, instances of this object can never be freed. This is
 * for example done with the True and False objects.
 */
#define HELIOS_OBJECT_STATIC_FIELD bool staticobj;

//! sets this static field
#define HELIOS_OBJECT_STATIC() .staticobj = true

/**
 * The definition of a helios type struct. This is the part in any helios
 * object defining what type it is. Any helios struct must define a type and
 * use it in the HELIOS_OBJECT_COMMON_BASE which tells the object system
 * what type the object is. It has pointers to all the functions and fields
 * acting on the type which are standardized. Non standard functions can
 * still be exported (example: helios_string_from_charp).
 *
 * Inheritance system:
 * Any object has an helios_type object in it of which the object is an
 * instance. helios_type is always instance of itself. By putting a pointer
 * to the type of an object in the first field of the struct, any
 * helios_object can be cast to the msot basic object possible
 * (helios_object), and using the type it's type can be determined and the
 * object can be cast back. This makes it possible to use helios_object as
 * type for any helios object.
 */
typedef struct __helios_type_s {
    HELIOS_OBJECT_COMMON_BASE;
    HELIOS_OBJECT_BASIC_FIELDS;
    HELIOS_OBJECT_BINOPS_FIELDS;
    HELIOS_OBJECT_UNARYOPS_FIELDS;

    HELIOS_OBJECT_STATIC_FIELD;
} helios_type;

#endif