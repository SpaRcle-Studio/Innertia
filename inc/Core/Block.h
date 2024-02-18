//
// Created by innerviewer on 2024-02-16.
//
#ifndef INNERTIA_BLOCK_H
#define INNERTIA_BLOCK_H

#include <Utils/stdInclude.h>
#include <Utils/macros.h>

#include <Utils/Types/Hash.h>

namespace IR_CORE_NS {
    class Block {
        using Hash = IR_TYPES_NS::Hash<IR_TYPES_NS::uint256_t>;
    public:
        void Init();

        IR_NODISCARD Hash* GetHash() const { return m_hash; }
    private:
        Hash* m_hash = nullptr;
    };
}

#endif //INNERTIA_BLOCK_H
