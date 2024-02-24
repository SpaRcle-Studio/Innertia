//
// Created by innerviewer on 2024-02-16.
//

#include <Core/Block.h>

namespace IR_CORE_NS {

    Block::Block(const std::string& timestamp, uint64_t difficulty, HashPtr hash, HashPtr previousBlock, uint64_t nonce)
        : m_timestamp(timestamp)
        , m_difficulty(difficulty)
        , m_hash(hash)
        , m_previousBlock(previousBlock)
        , m_nonce(nonce)
    {}


    IR_NODISCARD HashPtr Block::GetHash() const {
        return m_hash;
    }

    IR_NODISCARD HashPtr Block::GetPreviousBlockHash() const {
        return m_previousBlock;
    }

    IR_NODISCARD uint64_t Block::GetNonce() const {
        return m_nonce;
    }

    IR_NODISCARD std::string Block::GetTimestamp() const {
        return m_timestamp;
    }

    IR_NODISCARD uint64_t Block::GetDifficulty() const {
        return m_difficulty;
    }

    IR_NODISCARD uint64_t Block::GetSize() const {
        return m_size;
    }


    std::string Block::ToString() const {
        SRHalt("Block::ToString() not implemented over yet.");

    
        return std::string();
    }
    
}