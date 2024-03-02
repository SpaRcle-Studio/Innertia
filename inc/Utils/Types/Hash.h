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

        // It can be useful
        uint32_t switchEndian(uint32_t num) {
            return ((num >> 24) & 0xff) |
                ((num << 8) & 0xff0000) |
                ((num >> 8) & 0xff00) |
                ((num << 24) & 0xff000000);
        }
    }


    // Size - hash size in bytes
    // Repr - data type representing an unsigned integer
    template<size_t Size, typename Repr>
          requires std::same_as<typename Repr::limb_type, uint8_t>   // Currently only uint8_t as limb type
    class Hash {
    public:
        using ValueType = Repr;
        using LimbType = Repr::limb_type;

        Hash() = default;

        Hash(Repr&& val) 
            : m_hash(std::move(val))
        {}

        // hexStr - a string representing a hexadecimal number
        Hash(std::string&& hexStr) {
            SRAssert2(hexStr.size() == 2 * Size, "Hexadecimal number size(string length) must be twice Size");

            if (hexStr.substr(0, 2) != "0x") {
                hexStr = "0x" + hexStr;
            }

            m_hash = std::move(hexStr);

            auto&& rep = m_hash.representation();
            std::reverse(rep.begin(), rep.end());
        }

        std::string ToString(bool isLower=true) const { 
            std::string result(2 * Size, '_');

            auto&& limbs = m_hash.representation();
            for (size_t i = 0; i < Size; i++)
                snprintf(
                    const_cast<char*>(result.data() + 2 * i),
                    2 * Size + 1 - 2 * i,
                    isLower ? "%02x" : "%02X",
                    limbs.at(i));
            return result;
        }

        bool IsEmpty() const {
            return m_hash == IR_HASH_INVALID;
        }

        ~Hash() = default;
    
    private:
        ValueType m_hash = IR_HASH_INVALID;
    };


    using uint256_8_t = SR_TYPES_NS::uintwide_t<256, uint8_t>;
    using IRHash = Hash<HashDetails::hash_size_bytes, uint256_8_t>;
    using HashPtr = std::shared_ptr<IRHash>;
}

#endif //INNERTIA_HASH_H
