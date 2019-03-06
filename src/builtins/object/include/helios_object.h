#ifndef HELIOS_OBJECT_H

#define HELIOS_OBJECT_H

#include <stdint.h>
#include <stdlib.h>

// forward declaration
struct __helios_object_s;

/*
WARNING: Every helios object class must have HELIOS_OBJECT_COMMON_BASE as its
*FIRST* FIELD. This is very important for casting to work.
*/
#define HELIOS_OBJECT_COMMON_BASE                                              \
    struct __helios_object_s *class;                                           \
    uint32_t refcount

#define HELIOS_OBJECT_COMMON_BASE_INIT(_class) .refcount = 0, .class = _class

// definitions of common sets of class fields
#define HELIOS_OBJECT_BASIC_FIELDS                                             \
    struct __helios_object_s *(*constructor)();                                \
    void (*destructor)(struct __helios_object_s * obj);                        \
    char *classname

#define HELIOS_OBJECT_BASIC_FIELDS_NULL()                                      \
    .constructor = NULL, .destructor = NULL, .classname = NULL
#define HELIOS_OBJECT_BASIC_FIELDS_CONSTRUCTOR(function) .constructor = function
#define HELIOS_OBJECT_BASIC_FIELDS_DESTRUCTOR(function) .destructor = function
#define HELIOS_OBJECT_BASIC_FIELDS_CLASSNAME(name) .classname = name

#define HELIOS_OBJECT_BINOPS_FIELDS                                            \
    struct __helios_object_s *(*add)(struct __helios_object_s * self,          \
                                     struct __helios_object_s * other);        \
    struct __helios_object_s *(*subtract)(struct __helios_object_s * self,     \
                                          struct __helios_object_s * other);   \
    struct __helios_object_s *(*divide)(struct __helios_object_s * self,       \
                                        struct __helios_object_s * other);     \
    struct __helios_object_s *(*multiply)(struct __helios_object_s * self,     \
                                          struct __helios_object_s * other)

#define HELIOS_OBJECT_BINOPS_FIELDS_NULL()                                     \
    .add = NULL, .subtract = NULL, .divide = NULL, .multiply = NULL
#define HELIOS_OBJECT_BINOPS_FIELDS_ADD(function) .add = function
#define HELIOS_OBJECT_BINOPS_FIELDS_SUBTRACT(function) .subtract = function
#define HELIOS_OBJECT_BINOPS_FIELDS_DIVIDE(function) .divide = function
#define HELIOS_OBJECT_BINOPS_FIELDS_MULTIPLY(function) .multiply = function

typedef struct __helios_object_s {
    HELIOS_OBJECT_COMMON_BASE;
    HELIOS_OBJECT_BASIC_FIELDS;
    HELIOS_OBJECT_BINOPS_FIELDS;
} helios_object;

struct __helios_object_s helios_object_type;

// casting

#define TO_HELIOS_OBJECT(obj) ((struct __helios_object_s *)obj)
#define IS_HELIOS_OBJECT(obj) ((struct __helios_object_s *)obj)

#endif
