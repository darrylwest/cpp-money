
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

    struct Money {
        // --- Constructors ---

        static Money Money::from_dollars(double dollars) {
            // The cast now goes to int64_t
            return Money(static_cast<int64_t>(dollars * 100.0));
        }

        static Money Money::from_string(const std::string& value) {
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

        // convert this to US dollars
        auto to_string() const {
            // Use int64_t here for consistency
            const int64_t total_cents = as_cents();
            const int64_t abs_cents = std::abs(total_cents);
            const auto sign = (total_cents < 0) ? "-" : "";

            // now the thousands delimiter
            std::string ss = std::format(std::locale("en_US.UTF-8"), "{:L}", abs_cents / 100);

            return std::format("{}${}.{:02}", sign, ss, abs_cents % 100);
        }
    };

}  // namespace app

// The std::formatter also needs to know about int64_t
template <> struct std::formatter<app::Money> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(const app::Money& money, format_context& ctx) const {
        return money.to_string();
    }
};
