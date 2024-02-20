//
// Created by innerviewer on 2024-02-16.
//

#include <Core/Block.h>
#include <iostream>

namespace IR_CORE_NS {

    Block::Block(std::string&& minedTime, uint64_t difficulty, Hash* hash, Hash* previousBlock, uint64_t nonce)
        : m_minedTime(std::move(minedTime))
        , m_difficulty(difficulty)
        , m_hash(new Hash(*hash))
        , m_previousBlock(new Hash(*previousBlock))
        , m_nonce(nonce)
    {}


    Block::Hash* Block::GetHash() const {
        return m_hash;
    }

    uint64_t Block::GetNonce() const {
        return m_nonce;
    }


    std::string Block::ToString() const {
        SRHalt("Block::ToString() not implemented over yet.");
    
        return std::string();
    }
    
}