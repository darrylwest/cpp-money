//
// dpw
//
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <iomanip>
#include <print>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>

namespace money {
    constexpr auto VERSION = "0.5.2-103";

    struct Money {
      private:
        // Use the explicit 64-bit integer type
        int64_t m_cents;

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

        static Money zero() { return Money(0); }

        static Money from_dollars(const double dollars) {
            // this way works; don't try to manipulate the double

            const auto ss = std::format("{:.2f}", dollars);
            // std::println("num: {}, str: {}", dollars, ss);
            auto m = Money::from_string(ss);

            return m;
        }

        static Money from_string(const std::string& value) {
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
        constexpr int64_t as_cents() const { return m_cents; }
        double as_dollars() const { return static_cast<double>(m_cents) / 100.0; }

        // returns the dollar component of this money object
        constexpr int64_t dollars() const { return m_cents / 100; }

        // returns the cents component of this money object
        constexpr int8_t cents() const { return m_cents % 100; }

        // --- Operator Overloading (no changes needed here) ---
        friend Money operator+(const Money& lhs, const Money& rhs) { return Money(lhs.m_cents + rhs.m_cents); }

        friend Money operator-(const Money& lhs, const Money& rhs) { return Money(lhs.m_cents - rhs.m_cents); }

        // Note: Money * Money is usually not a valid concept (what is a dollar-squared?)
        // But multiplication by a scalar is.
        friend Money operator*(const Money& lhs, const int scalar) { return Money(lhs.m_cents * scalar); }

        // scale up or down by a percentage
        friend Money operator*(const Money& lhs, const double scale) { return Money::from_dollars(lhs.as_dollars() * scale); }

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
    };

}  // namespace money
