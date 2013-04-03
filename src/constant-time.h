///////////////////////////////////////////////////////////////////////
//
// constant-time.h
//
// Copyright (c) 2013 Eric Lombrozo
//  all rights reserved
//
// A collection of useful macros that exhibit no branching dependent
// on the contents of their input.
//

#ifndef _CONSTANT_TIME_H_
#define _CONSTANT_TIME_H_

#define CT_MAX(a,b) ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    ((_a ^ _b) & -(_a > _b)) ^ _b; \
})

#define CT_MIN(a,b) ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    ((_a ^ _b) & -(_a < _b)) ^ _b; \
})

#define CT_COND_SEL(a,b,t) ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    ((_a ^ _b) & -(t)) ^ _a; \
})

#define CT_SORT_ASC(a,b) \
    do { \
        typeof (a) _mask = -(a > b); \
        a ^= (b & _mask); \
        b ^= (a & _mask); \
        a ^= (b & _mask); \
    } while (0)

#define CT_SORT_DESC(a,b) \
    do { \
        typeof (a) _mask = -(a < b); \
        a ^= (b & _mask); \
        b ^= (a & _mask); \
        a ^= (b & _mask); \
    } while (0)

#define CT_COND_SWAP(a,b,t) \
    do { \
        typeof (a) _mask = -(t); \
        a ^= (b & _mask); \
        b ^= (a & _mask); \
        a ^= (b & _mask); \
    } while (0)

#endif /* _CONSTANT_TIME_H_ */
