﻿
#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "space.h"

typedef struct Address      Address;

struct Address {
    AddrSpace *base;
    int offset;

    enum {
        m_minimal,
        m_maximal,
    } mach_extreme;
};

Address*        Address_new(void);
Address*        Address_new1(AddrSpace *id, uintb off);
Address*        Address_new2(Address *op2);
void            Address_delete(Address *op);

#define Address_isInvalid(a)    (a->base == NULL)
#define Address_getAddrSize(a)  AddrSpace_getAddrSize(a->base)
#define Address_isBigEndian(a)  AddrSpace_isBigEndian(a->base)

inline Address* Address_assign(Address *op1, Address *op2) 
{
    op1->base = op2->base;
    op1->offset = op2->offset;

    return op1;
}

inline bool Address_equal(Address *op1, Address *op2)
{
    return (op1->base == op2->base) && (op1->offset == op2->offset);
}

inline bool Address_unequal(Address *op1, Address *op2)
{
    return (op1->base != op2->base) || (op1->offset != op2->offset);
}

inline bool Address_less(Address *op1, Address *op2)
{
    if (op1->base != op2->base) {
        if (op1->base == NULL) return true;
        else if (op1->base == (AddrSpace *)-1) return false;
        else if (op2->base == NULL) return false;
        else if (op2->base == (AddrSpace *)-1) return true;

        return AddrSpace_getIndex(op1->base) < AddrSpace_getIndex(op2->base);
    }

    if (op1->offset != op2->offset) return op1->offset < op2->offset;
    return false;
}

inline bool Address_lessThan(Address *op1, Address *op2)
{
    if (op1->base != op2->base) {
        if (op1->base == NULL) return true;
        else if (op1->base == (AddrSpace *)-1) return false;
        else if (op2->base == NULL) return false;
        else if (op2->base == (AddrSpace *)-1) return true;

        return AddrSpace_getIndex(op1->base) < AddrSpace_getIndex(op2->base);
    }

    if (op1->offset != op2->offset) return op1->offset < op2->offset;
    return true;
}

inline Address* Address_add(Address *op1, int off)
{
    return Address_new1(op1->base, AddrSpace_wrapOffset(op1->base, off));
}

inline uintb calc_mask(int size) {
    uintb uintbmasks[9] = { 
        0, 
        0xff, 
        0xffff, 
        0xffffff, 
        0xffffffff, 
        0xffffffffffLL, 
        0xffffffffffffLL, 
        0xffffffffffffffLL, 
        0xffffffffffffffffLL 
    };

    return uintbmasks[(size < 8) ? size : 8];
}


void        sign_extend(intb *val, int bit);
void        zero_extend(intb *val, int bit);
void        byte_swap(intb *val, int size);
uintb       coveringmask(uintb val);

#ifdef __cplusplus
}
#endif

#endif