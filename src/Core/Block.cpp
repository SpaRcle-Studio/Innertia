//
// Created by innerviewer on 2024-02-16.
//

#include <Core/Block.h>
#include <iostream>

namespace IR_CORE_NS {

    void Block::Init() {
        m_hash = new Hash();
    }
}