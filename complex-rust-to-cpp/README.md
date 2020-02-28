# Exposing a complex data-structure from rust to C++

This is a somewhat reduced example of how you can expose complex rust
data-structures to C++ seamlessly.

It uses C++20 because I didn't want to write my own `span<T>` :-)

I wrote a [blog post explaining
it](https://crisal.io/words/2020/02/28/C++-rust-ffi-patterns-1-complex-data-structures.html).
