//Important!!
//This disk-based bplus tree implementation is found from the following website
//https://github.com/zcbenz/BPlusTree.
//The MIT License (MIT)
//Copyright (c) 2012 Zhao Cheng

#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>

namespace bpt {

/* predefined B+ info */
#define BP_ORDER 20

/* key/value type */
typedef int value_t;
struct key_t {
    char k[16];

    key_t(const char *str = "")
    {
        bzero(k, sizeof(k));
        strcpy(k, str);
    }
};

inline int keycmp(const key_t &a, const key_t &b) {
    //int x = strlen(a.k) - strlen(b.k);
    //return x == 0 ? strcmp(a.k, b.k) : x;
    return strcmp(a.k, b.k);
}

#define OPERATOR_KEYCMP(type) \
    bool operator< (const key_t &l, const type &r) {\
        return keycmp(l, r.key) < 0;\
    }\
    bool operator< (const type &l, const key_t &r) {\
        return keycmp(l.key, r) < 0;\
    }\
    bool operator== (const key_t &l, const type &r) {\
        return keycmp(l, r.key) == 0;\
    }\
    bool operator== (const type &l, const key_t &r) {\
        return keycmp(l.key, r) == 0;\
    }

}

#endif /* end of PREDEFINED_H */
