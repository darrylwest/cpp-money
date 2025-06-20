# Cpp Money

A small c++23 library for handling currency, mostly US Dollars.

```
 ______                     _______                          
|      |.-----.-----.______|   |   |.-----.-----.-----.--.--.
|   ---||  _  |  _  |______|       ||  _  |     |  -__|  |  |
|______||   __|   __|      |__|_|__||_____|__|__|_____|___  |
        |__|  |__|                                    |_____|
```

## Overview

A library to support saving and restoring monitary values like cost, prices, total sales etc.  Should be used where monitary calculations are required.  

The base type is int64_t representing currency to the penny.  Constructors convert a double to the appropriate penny amount (cents).

## Construct & Use

```
const auto p1 = Money::from_dollars(342.45);
assert(p1.as_cents() == 34245);
std::println(p1.to_string()); // $342.45

const int64_t cents = 34245;
const auto p2 = Money(cents);

std::println(p2.to_string()); // $342.45

assert(p1 == p2); // true

assert(p1 + p2 == p1 * 2);
assert(p1 - p2 == Money::zero());

const auto p3 = Money::from_string("$100.00");

assert(p3 == Money(10000));
assert(p3 == Money::from_dollars(100));
assert(p3 / 2 == Money(5000));

const double dollars = -123.00;
const p4 = Money::from_dollars(dollars);
assert(p4.as_cents() == -12300);

std::println("{}", p4.to_string()); // -$123.00
```

_See unit tests for more examples..._

###### dpw | 2025-06-20

