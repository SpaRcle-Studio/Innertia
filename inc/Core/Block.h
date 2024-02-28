//
// Created by innerviewer on 2024-02-16.
//
#ifndef INNERTIA_BLOCK_H
#define INNERTIA_BLOCK_H

#include <Utils/Debug.h>
#include <macros.h>

#include "Transaction.h"


namespace IR_CORE_NS {
    class Block {
    public:
        using MerkleTree = 
            SR_TYPES_NS::TreeT<IR_TYPES_NS::HashDetails::hash_size_bytes, SR_TYPES_NS::sha256_compress>;
        
        Block(const std::string& timestamp, uint64_t difficulty, 
            IR_TYPES_NS::HashPtr hash, IR_TYPES_NS::HashPtr previousBlock, 
            uint64_t nonce);

        IR_NODISCARD IR_TYPES_NS::HashPtr GetHash() const;
        
        IR_NODISCARD IR_TYPES_NS::HashPtr GetPreviousBlockHash() const;
        
        IR_NODISCARD uint64_t GetNonce() const;
        
        IR_NODISCARD std::string GetTimestamp() const;
        
        IR_NODISCARD uint64_t GetDifficulty() const;
        
        IR_NODISCARD uint64_t GetSize() const;

        ~Block();

    private:
        // Header
        IR_TYPES_NS::HashPtr m_previousBlock = nullptr;
        std::string m_timestamp;
        uint64_t m_difficulty = 0;
        uint64_t m_nonce = 0;
        MerkleTree m_merkleHash;

        // Block data
        IR_TYPES_NS::HashPtr m_hash = nullptr;
        uint64_t m_size = 0;
        std::vector<Transaction>* m_transactions = nullptr;
    };
}

#endif //INNERTIA_BLOCK_H
