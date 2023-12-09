### General thoughts

1. ~~Working with gcc 13.2.1, so still not `ranges::to`, which would have
   simplified evaluation.~~ Actually, `ranges::to` does not work with `std::array`.
2. Why is `std::views::split` not const-iterable?
3. Day 06 comes down to solving a quadratic inequality. Unfortunately
   `std::sqrt` is not `constexpr` so I needed a manual implementation of it.
4. Day 07 - ~~I maintain that the description of the rules do not match
   expectations when it comes to Jokers/Jacks and breaking ties.~~ I was a
   muppet who had not read the paragraph about readjusting the J's numeric value.
5. Day 8 once again has beaten my laptop capabilities of constexpr evaluation.
5.1. Assuming a few things allows for a much more efficient solution and it just
     so happens that those assumptions hold invariably in puzzle for day 8.
     Spoiler: one can use LCM to get the result. Sigh...
