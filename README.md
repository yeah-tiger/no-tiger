# no-tiger
Not Ordinary Tiger Language 

[![Build Status](https://travis-ci.com/TH3CHARLie/no-tiger.svg?branch=master)](https://travis-ci.com/TH3CHARLie/no-tiger)

## Introduction

no-tiger is the course project for Compiler, Zhejiang University, Spring 2019.

## Reference

1. [ANSI-C-grammar-lex](http://www.quut.com/c/ANSI-C-grammar-l-2011.html)

2. [ANSI-C-grammar-yacc](http://www.quut.com/c/ANSI-C-grammar-y-2011.html)

3. [FLEX AND BISON IN C++](http://www.jonathanbeard.io/tutorials/FlexBisonC++) best workable example

4. [compiler2018_big_picture](https://acm.sjtu.edu.cn/w/images/b/b5/Compiler2018_big_picture.pdf) slides from Lequn Chen, TA from SJTU (now PhD at UW)

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

- [x] support `main` (at exp1.c)

- [x] Support basic control flow: ```if while for break continue ```

- [x] Support function definition

- [x] Support expressions

- [x] XML AST format output

- [ ] LLVM codegen

- [ ] Support builtin array

- [ ] Support custom data type

- [ ] Support builtin I/O functions


## Built With

- [cxxopts](https://github.com/jarro2783/cxxopts) - Lightweight C++ command line option parser
