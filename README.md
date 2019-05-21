# no-tiger
Not Ordinary Tiger Language

## Introduction

no-tiger is the course project for Compiler, Zhejiang University, Spring 2019.

## Reference

1. [ANSI-C-grammar-lex](http://www.quut.com/c/ANSI-C-grammar-l-2011.html)

2. [ANSI-C-grammar-yacc](http://www.quut.com/c/ANSI-C-grammar-y-2011.html)

3. [FLEX AND BISON IN C++](http://www.jonathanbeard.io/tutorials/FlexBisonC++) best workable example

## Build
```
git clone --recursive [url]
```

- prerequisite: cmake3, Flex, Bison, GCC >= 4.9.0 (for `regex` support)

```bash
sh build.sh
```

## Roadmap

- [x] C++ style Flex/Bison generated scanner/parser

- [ ] support `main`

- [ ] basic arithmetic and variable declaration and definition

- [ ] Support basic control flow

- [ ] Support function

- [ ] Support builtin array

- [ ] Support custom data type

- [ ] Support builtin I/O functions

- [ ] For simplicity, I think we should not support pointer for now

## Built With

- [cxxopts](https://github.com/jarro2783/cxxopts) - Lightweight C++ command line option parser
