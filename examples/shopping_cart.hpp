#pragma once

#include <money/money.hpp>
#include <string>
#include <vector>

namespace examples {

    struct CartItem {
        std::string product_name;
        std::string description;
        money::Money unit_price;
        int quantity;

        CartItem(const std::string& name, const std::string& desc, const money::Money& price, int qty);

        money::Money line_total() const;
    };

    struct ShoppingCart {
      private:
        std::vector<CartItem> items;

      public:
        void add_item(const CartItem& item);
        void add_item(const std::string& name, const std::string& desc, const money::Money& price, int qty);
        
        bool remove_item(const std::string& product_name);
        void clear();
        
        money::Money total() const;
        std::size_t item_count() const;
        bool is_empty() const;
        
        const std::vector<CartItem>& get_items() const;
        
        void display() const;
    };

}