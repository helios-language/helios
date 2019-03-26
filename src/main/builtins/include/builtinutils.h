/**
 * This header is only there to resolve some circular dependency (with memory.h)
 * issues but it fully belongs to builtins.h *
 */

#ifndef BUILTINUTILS_H
#define BUILTINUTILS_H

//! generic function for calling a member function of a helios_object struct.
//! note: doesn't include obj.__dict__ like lookup
#define HELIOS_CALL_MEMBER(func, obj, ...)                                     \
    (obj->class->func(obj, ##__VA_ARGS__))

#define HELIOS_GET_MEMBER(member, obj) (obj->class->member)
#define HELIOS_HAS_MEMBER(member, obj) (obj->class->member != NULL)

#endif