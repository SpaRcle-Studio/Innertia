//
// Created by innerviewer on 2024-02-16.
//
#ifndef INNERTIA_HASH_H
#define INNERTIA_HASH_H

#include <macros.h>

#include <Utils/Common/Hashes.h>
#include <Utils/Types/MerkleTree.h>
#include <Utils/Types/UintWide_t.h>

namespace IR_TYPES_NS {
    namespace HashDetails {
        static constexpr size_t hash_size_bytes = 32;
        static constexpr size_t hash_size = 32 * 8;
    }


    template<typename T> class Hash {
    public:
        Hash() = default;
        ~Hash() = default;

        T GetValue() const { return m_hash; }
    private:
        T m_hash = IR_HASH_INVALID;
    };



    using IRHash = SR_TYPES_NS::HashT<HashDetails::hash_size_bytes>; // Here in the template parameter the size in bytes is required
    using HashPtr = std::shared_ptr<IRHash>;
}

#endif //INNERTIA_HASH_H
