//
// dpw
//
#include "shopping_cart.hpp"
#include <money/money.hpp>
#include <print>

using namespace money;
using namespace examples;

int main() {
    std::println("Shopping Cart Demo\n");

    ShoppingCart cart;

    cart.add_item("Laptop", "High-performance laptop", Money::from_dollars(1299.99), 1);
    cart.add_item("Mouse", "Wireless optical mouse", Money::from_dollars(29.95), 2);
    cart.add_item("Keyboard", "Mechanical gaming keyboard", Money::from_dollars(149.50), 1);
    cart.add_item("Monitor", "27-inch 4K display", Money::from_dollars(399.00), 1);

    std::println("After adding items:");
    cart.display();

    std::println("\nCart has {} items", cart.item_count());
    std::println("Cart total: {}\n", cart.total());

    cart.add_item("Mouse", "Additional wireless mouse", Money::from_dollars(29.95), 1);
    std::println("After adding another mouse (should update quantity):");
    cart.display();

    std::println("\nRemoving keyboard...");
    if (cart.remove_item("Keyboard")) {
        std::println("Keyboard removed successfully.");
    } else {
        std::println("Keyboard not found in cart.");
    }

    std::println("\nFinal cart:");
    cart.display();

    auto items = cart.get_items();
    std::println("\nDetailed breakdown:");
    for (const auto& item : items) {
        std::println("{} x {} = {}", 
                    item.quantity, 
                    item.unit_price, 
                    item.line_total());
    }

    std::println("\nDemo of various Money operations:");
    auto laptop_price = Money::from_dollars(1299.99);
    auto tax_rate = 0.08;
    auto tax_amount = laptop_price * tax_rate;
    auto total_with_tax = laptop_price + tax_amount;

    std::println("Laptop price: {}", laptop_price);
    std::println("Tax (8%): {}", tax_amount);
    std::println("Total with tax: {}", total_with_tax);

    std::println("\nClearing cart...");
    cart.clear();
    cart.display();

    return 0;
}