

#include <cmath>
#include <cstdint>
#include <format>
#include <iomanip>
#include <money/money.hpp>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>

namespace money {

    std::string Money::to_string() const {
        // Use int64_t here for consistency
        const int64_t total_cents = as_cents();
        const int64_t abs_cents = std::abs(total_cents);
        const auto sign = (total_cents < 0) ? "-" : "";

        // now the thousands delimiter
        std::string ss = std::format(std::locale("en_US.UTF-8"), "{:L}", abs_cents / 100);

        return std::format("{}${}.{:02}", sign, ss, abs_cents % 100);
    }

    constexpr std::string Money::currency() {
        return m_currency;
    }


}  // namespace money
