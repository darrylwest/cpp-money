#include "shopping_cart.hpp"
#include <algorithm>
#include <iomanip>
#include <print>
#include <ranges>

namespace examples {

    CartItem::CartItem(const std::string& name, const std::string& desc, const money::Money& price, int qty)
        : product_name(name), description(desc), unit_price(price), quantity(qty) {
        if (qty <= 0) {
            throw std::invalid_argument("Quantity must be positive");
        }
    }

    money::Money CartItem::line_total() const {
        return unit_price * quantity;
    }

    void ShoppingCart::add_item(const CartItem& item) {
        auto it = std::ranges::find_if(items, [&item](const CartItem& existing) {
            return existing.product_name == item.product_name;
        });

        if (it != items.end()) {
            it->quantity += item.quantity;
        } else {
            items.push_back(item);
        }
    }

    void ShoppingCart::add_item(const std::string& name, const std::string& desc, 
                               const money::Money& price, int qty) {
        add_item(CartItem(name, desc, price, qty));
    }

    bool ShoppingCart::remove_item(const std::string& product_name) {
        auto it = std::ranges::find_if(items, [&product_name](const CartItem& item) {
            return item.product_name == product_name;
        });

        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }

    void ShoppingCart::clear() {
        items.clear();
    }

    money::Money ShoppingCart::total() const {
        auto line_totals = items | std::views::transform([](const CartItem& item) {
            return item.line_total();
        });

        money::Money sum = money::Money::zero();
        for (const auto& line_total : line_totals) {
            sum += line_total;
        }
        return sum;
    }

    std::size_t ShoppingCart::item_count() const {
        return items.size();
    }

    bool ShoppingCart::is_empty() const {
        return items.empty();
    }

    const std::vector<CartItem>& ShoppingCart::get_items() const {
        return items;
    }

    void ShoppingCart::display() const {
        if (is_empty()) {
            std::println("Shopping cart is empty.");
            return;
        }

        std::println("Shopping Cart:");
        std::println("{:<20} {:<30} {:<12} {:<8} {:<12}", 
                    "Product", "Description", "Unit Price", "Qty", "Line Total");
        std::println("{:-<82}", "");

        for (const auto& item : items) {
            std::println("{:<20} {:<30} {:<12} {:<8} {:<12}",
                        item.product_name,
                        item.description,
                        item.unit_price.to_string(),
                        item.quantity,
                        item.line_total().to_string());
        }

        std::println("{:-<82}", "");
        std::println("{:<70} {:<12}", "Total:", total().to_string());
    }

}