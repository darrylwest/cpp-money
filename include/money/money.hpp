//
// dpw
//
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>

namespace money {
    constexpr auto VERSION = "0.5.5-108";

    struct Money {
      private:
        // Use the explicit 64-bit integer type
        int64_t m_cents;
        std::string m_currency = "USD";

      public:
        // --- Constructors ---
        constexpr Money() : m_cents(0) {}
        // Use int64_t in the constructor
        constexpr explicit Money(int64_t cents) : m_cents(cents) {}

        constexpr explicit Money(const int64_t dollars, const u_int8_t cents) {
            if (cents > 99) {
                throw std::invalid_argument("cents value must be between 0..99");
            }

            if (dollars < 0) {
                m_cents = dollars * 100 - cents;
            } else {
                m_cents = dollars * 100 + cents;
            }
        }

        constexpr std::string currency();

        static constexpr Money zero() { return Money(0); }

        static constexpr Money from_dollars(const double dollars) {
            // this way works; don't try to manipulate the double

            const auto ss = std::format("{:.2f}", dollars);
            auto m = Money::from_string(ss);

            return m;
        }

        static constexpr Money from_string(const std::string& value) {
            // Remove the dollar sign and commas
            std::string cleaned_str;
            std::copy_if(value.begin(), value.end(), std::back_inserter(cleaned_str),
                         [](char ch) { return ch != '$' && ch != ','; });

            size_t decimalPos = cleaned_str.find('.');
            if (decimalPos != std::string::npos) {
                if (cleaned_str.length() - decimalPos - 1 != 2) {
                    throw std::invalid_argument(
                        "Invalid format: must have exactly two digits after the decimal point.");
                }
                // Remove the decimal point
                cleaned_str.erase(decimalPos, 1);
            }

            // Convert to int64_t representing cents
            int64_t cents = 0;
            std::istringstream iss(cleaned_str);
            iss >> std::noskipws;  // Don't skip whitespace
            iss >> cents;          // Read the integer part

            // Handle negative values
            if (value[0] == '-') {
                Money(cents * -1);
            }

            return Money{cents};
        }

        // --- Accessors ---
        // The accessor now returns int64_t
        [[nodiscard]] constexpr int64_t as_cents() const { return m_cents; }
        [[nodiscard]] double as_dollars() const { return static_cast<double>(m_cents) / 100.0; }

        // returns the dollar component of this money object
        [[nodiscard]] constexpr int64_t dollars() const { return m_cents / 100; }

        // returns the cents component of this money object
        [[nodiscard]] constexpr int8_t cents() const { return m_cents % 100; }

        // --- Operator Overloading --
        constexpr Money& operator+=(const Money& rhs) noexcept {
            // Modify the member of the left-hand-side object
            m_cents += rhs.m_cents;
            // Return a reference to the modified object
            return *this;
        }

        constexpr Money& operator-=(const Money& rhs) noexcept {
            // Modify the member of the left-hand-side object
            m_cents -= rhs.m_cents;
            // Return a reference to the modified object
            return *this;
        }

        friend Money operator+(const Money& lhs, const Money& rhs) { return Money(lhs.m_cents + rhs.m_cents); }

        friend Money operator-(const Money& lhs, const Money& rhs) { return Money(lhs.m_cents - rhs.m_cents); }

        // Note: Money * Money is usually not a valid concept (what is a dollar-squared?)
        // But multiplication by a scalar is.
        friend Money operator*(const Money& lhs, const int scalar) { return Money(lhs.m_cents * scalar); }

        // scale up or down by a percentage
        friend Money operator*(const Money& lhs, const double scale) {
            return Money::from_dollars(lhs.as_dollars() * scale);
        }

        friend Money operator/(const Money& lhs, int scalar) {
            // Handle rounding if necessary, here we just truncate
            return Money(lhs.m_cents / scalar);
        }

        // spaceshitp operator?
        friend bool operator==(const Money& lhs, const Money& rhs) { return lhs.m_cents == rhs.m_cents; }
        friend bool operator!=(const Money& lhs, const Money& rhs) { return lhs.m_cents != rhs.m_cents; }
        friend bool operator<(const Money& lhs, const Money& rhs) { return lhs.m_cents < rhs.m_cents; }
        friend bool operator>(const Money& lhs, const Money& rhs) { return lhs.m_cents > rhs.m_cents; }
        friend bool operator<=(const Money& lhs, const Money& rhs) { return lhs.m_cents <= rhs.m_cents; }
        friend bool operator>=(const Money& lhs, const Money& rhs) { return lhs.m_cents >= rhs.m_cents; }

        // convert this to US dollars
        std::string to_string() const;

        // --- State Checks ---
        constexpr bool is_zero() const { return m_cents == 0; }
        constexpr bool is_positive() const { return m_cents > 0; }
        constexpr bool is_negative() const { return m_cents < 0; }

        // --- Unary Operators ---
        constexpr Money abs() const { return Money(std::abs(m_cents)); }
    };

    // --- Stream Operator ---
    inline std::ostream& operator<<(std::ostream& os, const Money& money) {
        os << money.to_string();
        return os;
    }

}  // namespace money

// std::formatter specialization for Money
template <> struct std::formatter<money::Money> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const money::Money& money, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", money.to_string());
    }
};
