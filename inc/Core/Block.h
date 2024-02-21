//
// Created by innerviewer on 2024-02-16.
//
#ifndef INNERTIA_BLOCK_H
#define INNERTIA_BLOCK_H

#include <Utils/Types/MerkleTree.h>
#include <Utils/Types/UintWide_t.h>
#include <Utils/Debug.h>
#include <Utils/macros.h>

#include <Utils/Common/Hashes.h>
#include <Utils/Types/Hash.h>



namespace IR_CORE_NS {
    class Block {
        // TODO: implement sha256 and replace this V
        using Hash = IR_TYPES_NS::Hash<SR_TYPES_NS::uint256_t>;
    public:
        Block(const std::string& timestamp, uint64_t diffuculty, Hash* hash, Hash* previousBlock, uint64_t nonce);

        IR_NODISCARD Hash* GetHash() const;
        
        IR_NODISCARD Hash* GetPreviousBlockHash() const;
        
        IR_NODISCARD uint64_t GetNonce() const;
        
        IR_NODISCARD std::string GetTimestamp() const;
        
        IR_NODISCARD uint64_t GetDifficulty() const;
        
        IR_NODISCARD uint64_t GetSize() const;

        IR_NODISCARD std::string ToString() const;

        ~Block();


    private:
        // Header
        Hash* m_previousBlock = nullptr;
        std::string m_timestamp;
        uint64_t m_difficulty = 0;
        uint64_t m_nonce = 0;
        // SR_MATH_NS::TreeT<256> m_merkleHash;      // TODO: sha256
        
        // Block data
        Hash* m_hash = nullptr;
        uint64_t m_size = 0;
        std::vector<size_t>* m_transactions = nullptr;  // TODO: replace size_t on type, which represent Transaction
    };
}

#endif //INNERTIA_BLOCK_H
