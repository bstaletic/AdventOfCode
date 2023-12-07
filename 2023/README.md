### General thoughts

1. Working with gcc 13.2.1, so still not `ranges::to`, which would have
   simplified evaluation.
2. Why is `std::views::split` not const-iterable?
3. Day 06 comes down to solving a quadratic inequality. Unfortunately
   `std::sqrt` is not `constexpr` so I needed a manual implementation of it.
4. Day 07 - ~~I maintain that the description of the rules do not match
   expectations when it comes to Jokers/Jacks and breaking ties.~~ I was a
   muppet who had not read the paragraph about readjusting the J's numeric value.
