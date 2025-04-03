# Markov Text Generation

This very simple library uses [Markov Chains](https://en.wikipedia.org/wiki/Markov_chain#Markov_text_generators) to generate pseudo-random text output.

## Why?

You're probably thinking "there's tons of Markov generator implementations out there, surely we don't need one more one the pile?", and you'd be correct. But I noticed most of the implementations I could find were using higher-level languages such as Python or Javascript, which makes them potentially unsuitable if the goal is to integrate them into some other tool.

And also, I wanted to do things my own way, and no one can stop me.

## How?

If you're using cmake, simply add the library into your own project:
```cmake
add_subdirectory(Markov)

target_link_libraries(target PUBLIC Markov)
```

If you're not using cmake, consider doing it, but either way: the library is very small, all you technically need are the [Markov.hpp](source/Markov.hpp), [Markov.cpp](source/Markov.cpp) and [Alloca.hpp](source/Alloca.hpp) files from the source directory. I also recommend adding the [View.natvis](source/View.natvis) file for debugging when using Visual Studio.