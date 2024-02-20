//
// Created by innerviewer on 2024-02-16.
//
#ifndef INNERTIA_HASH_H
#define INNERTIA_HASH_H

#include <macros.h>
#include <Utils/Types/UintWide_t.h>

namespace IR_TYPES_NS {
    template<typename T> class Hash {
    public:
        Hash() = default;
        ~Hash() = default;

        T GetValue() const { return m_hash; }
    private:
        T m_hash = IR_HASH_INVALID;
    };
}

#endif //INNERTIA_HASH_H
