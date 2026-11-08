#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Вычисляет криптографический хеш SHA-256.
 */
class VHSHA256
{
public:
    /// Размер дайджеста SHA-256 в байтах.
    static constexpr size_t DIGEST_SIZE = 32;

    /// Тип для хранения бинарного дайджеста SHA-256.
    using Digest = std::array<uint8_t, DIGEST_SIZE>;

    /**
     * @brief Вычисляет SHA-256 для массива байтов.
     *
     * @param data Входные данные.
     * @return Бинарный дайджест SHA-256 (32 байта).
     */
    static Digest hash(const std::vector<uint8_t> &data);

    /**
     * @brief Вычисляет SHA-256 для массива байтов и возвращает строку в hex.
     *
     * @param data Входные данные.
     * @return Строка из 64 шестнадцатеричных символов (нижний регистр).
     */
    static std::string hashHex(const std::vector<uint8_t> &data);

    /**
     * @brief Преобразует бинарный дайджест в hex-строку.
     *
     * @param digest Бинарный дайджест SHA-256.
     * @return Строка из 64 шестнадцатеричных символов (нижний регистр).
     */
    static std::string toHex(const Digest &digest);
};
