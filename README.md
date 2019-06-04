# no-tiger
Not Ordinary Tiger Language 

[![Build Status](https://travis-ci.org/yeah-tiger/no-tiger.svg?branch=master)](https://travis-ci.org/yeah-tiger/no-tiger)

## Introduction

no-tiger is the course project for Compiler, Zhejiang University, Spring 2019.

## Reference

1. [ANSI-C-grammar-lex](http://www.quut.com/c/ANSI-C-grammar-l-2011.html)

2. [ANSI-C-grammar-yacc](http://www.quut.com/c/ANSI-C-grammar-y-2011.html)

3. [FLEX AND BISON IN C++](http://www.jonathanbeard.io/tutorials/FlexBisonC++) best workable example

4. [compiler2018_big_picture](https://acm.sjtu.edu.cn/w/images/b/b5/Compiler2018_big_picture.pdf) slides from Lequn Chen, TA from SJTU (now PhD at UW)

## Prerequisite

- Flex: 2.5.37 (different version may cause UB)
- Bison: 3.0.4 (for Bison version >= 3.0.4, remove ```sed``` command in ```build.sh``` since move semantics is already supported)
- GCC: 7.3.0

#### For macOS

You can easily install the newest Bison throwgh `homebrew`. However, currently for Flex version >= 2.6, there exist some reference problems. Thus, we use the version 2.5.37 to develop and test. You might download the source from [here](<https://github.com/westes/flex/releases/tag/flex-2.5.37>) and compile it yourself.

What needs to be pointed out is that macOS has a default Flex (2.5.35) and a default Bison (2.3), and they may cause some conflicts with your manual installed versions. Thus, you should export the new ones' path into `~/.bash_profile` to make them available. Type in these in terminal:

```bash
echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.bash_profile
echo 'export PATH="/usr/local/opt/bison/bin:$PATH"' >> ~/.bash_profile
```


## Build
```
git clone --recursive [url]
```

```bash
sh build.sh
```

## Visualization
Our compiler can dump AST in XML format. The provided python script can help visualize the AST
```
python tools/visualize.py input.xml
```

## Roadmap

- [x] C++ style Flex/Bison generated scanner/parser

- [x] support `main` (at exp1.c)

- [x] Support basic control flow: ```if while for break continue ```

- [x] Support function definition

- [x] Support expressions

- [x] XML AST format output

- [x] LLVM codegen

- [x] Support builtin array

- [x] Support builtin I/O functions

## Built With

- [cxxopts](https://github.com/jarro2783/cxxopts) - Lightweight C++ command line option parser
