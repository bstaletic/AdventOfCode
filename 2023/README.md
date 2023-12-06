### General thoughts

1. Working with gcc 13.2.1, so still not `ranges::to`, which would have
   simplified evaluation.
2. Why is `std::views::split` not const-iterable?
3. Day 06 comes down to solving a quadratic inequality. Unfortunately
   `std::sqrt` is not `constexpr` and grabbing the first sqrt implementation
   off SO did not work for part2.
