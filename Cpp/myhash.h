#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string>

// http://www.emoticode.net/c-plus-plus/hash-wrappers-for-openssl.txt

/// @brief Converts char to hex and appends it to string.
/// @param s String.
/// @param c Char to be converted to hex and then appended to string s.
inline void appendHex(std::string &s, unsigned char c)
{
    static const char digest_map[] = "0123456789abcdef";
    s.append(&digest_map[(c >> 4) & 0xF], 1)
        .append(&digest_map[c & 0xF], 1);
}

/// @struct append_hex_t
/// @brief Helper struct. Helps to convert types with size greater than one byte.
template <size_t size>
struct append_hex_t {
        /// @brief Converts type T to hex and appends it to string.
        /// @param s String.
        /// @param t Variable to be converted to hex and appended to string s.
    template <typename T>
    inline static void append(std::string &s, T t)
    {
        appendHex(s, *(reinterpret_cast<unsigned char*>(&t) - size + sizeof(T)));
        append_hex_t<size-1>::append(s, t);
    }
};

template <>
struct append_hex_t<0> {
    template <typename T>
    inline static void append(std::string &, T) {}
};

/// @brief Converts digest to hex string
/// @param first The first iterator. (i.e. begin).
/// @param last The last iterator. (i.e. end).
/// @returns Hex string.
template <typename Iter>
std::string digestToHex(Iter first, Iter last, size_t reserve = 256)
{
    std::string result;
    result.reserve(reserve);
    for (; first != last; ++first)
        append_hex_t<sizeof(*first)>::append(result, *first);
    return result;
}

/// @struct hash_creator_t
/// @brief Helper struct. Helps to calculate hash value and convert it to hex string.
template <typename HT = unsigned char>
struct hash_creator_t {
        /// @typedef Hash
        /// @brief Pointer to hash function.
    typedef unsigned char* (*Hash)(const unsigned char*,size_t,
            unsigned char*);

        /// @brief Calculates hash with hash function and converts it to hex string.
        /// @param hash Pointer to hash function.
        /// @param hash_size The size of the hash.
        /// @param pass Data to be hashed.
        /// @returns Hash value as a hex string.
    inline static std::string createHash(Hash hash, size_t hash_size,
            const std::string &pass)
    {
        uint8_t digest[hash_size];
        hash(reinterpret_cast<const uint8_t*>(pass.data()),
                pass.size(), digest);
        return digestToHex(reinterpret_cast<const HT*>(digest),
                reinterpret_cast<const HT*>(digest + hash_size),
                hash_size * 2);
    }
};

/// @brief Calculates MD5 hash.
/// @param pass Data to be hashed.
/// @returns MD5 hash value as a hex string.
std::string createMd5Hash(const std::string &pass)
{
    return hash_creator_t<uint64_t>::createHash(MD5,
            MD5_DIGEST_LENGTH, pass);
}

/// @brief Calculates SHA1 hash.
/// @param pass Data to be hashed.
/// @returns SHA1 hash value as a hex string.
std::string createSha1Hash(const std::string &pass)
{
    return hash_creator_t<uint32_t>::createHash(SHA1,
            SHA_DIGEST_LENGTH, pass);
}


/// @brief Calculates SHA256 hash.
/// @param pass Data to be hashed.
/// @returns SHA256 hash value as a hex string.
std::string createSha256Hash(const std::string &pass)
{
    return hash_creator_t<uint64_t>::createHash(SHA256,
            SHA256_DIGEST_LENGTH, pass);
}


/// @brief Calculates SHA512 hash.
/// @param pass Data to be hashed.
/// @returns SHA512 hash value as a hex string.
std::string createSha512Hash(const std::string &pass)
{
    return hash_creator_t<uint64_t>::createHash(SHA512,
            SHA512_DIGEST_LENGTH, pass);
}
