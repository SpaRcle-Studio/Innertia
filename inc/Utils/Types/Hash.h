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


    // Size - hash size in bytes
    // Repr - data type representing an unsigned integer
    template<size_t Size, typename Repr>
         requires std::same_as<typename Repr::limb_type, uint32_t>   // Currently only uint32_t as limb type
    class Hash {
    public:
        using ValueType = Repr;
        using LimbType = Repr::limb_type;

        Hash() = default;

        Hash(Repr&& val) 
            : m_hash(std::move(val))
        {}

        // hexStr - a string representing a hexadecimal number
        Hash(const std::string& hexStr) {
            assert(hexStr.size() == 2 * Size && "Hexadecimal number size(string length) must be twice Size");
            std::array<uint8_t, Size> limbs;

            for (size_t i = 0; i < Size; i++)
            {
                int tmp;
                sscanf(hexStr.c_str() + 2 * i, "%02x", &tmp);
                arr.at(i) = tmp;
            }
        }
        
        ValueType GetRawHash() const { 
            return m_hash; 
        }

        std::string ToStringU8() const {
            std::string result;
            result.reserve(2 * Size);

            for (const auto& limb : arr) {
                auto&& str = std::format("{:02x}", limb);

                result += str;
            }

            return result;
        }

        std::string ToStringU8Vendor() const {
            size_t num_chars = 2 * Size;
            std::string r(num_chars, '_');
            for (size_t i = 0; i < Size; i++)
                snprintf(
                    const_cast<char*>(r.data() + 2 * i),
                    num_chars + 1 - 2 * i,
                    "%02x",
                    arr[i]);
            return r;
        }

        
        std::string ToString(bool isLower=true) const { 
            std::string result;
            result.reserve(8 * Size);

            for (const auto& limb : m_hash.representation()) {
                for (auto i = 3; i >= 0; i--) {
                    uint8_t byte = (limb >> (8 * i)) & 0xFF;
                    auto&& str = std::format("{:02x}", byte);

                    result += str;
                }
            }

            return result;
        }

        bool IsEmpty() const {
            return m_hash == IR_HASH_INVALID;
        }

    
        ~Hash() = default;
    
    private:
        ValueType m_hash = IR_HASH_INVALID;
        std::array<uint8_t, Size> arr;
    };


    using IRHash = Hash<HashDetails::hash_size_bytes, SR_TYPES_NS::uint256_t>;
    using HashPtr = std::shared_ptr<IRHash>;
}

#endif //INNERTIA_HASH_H
