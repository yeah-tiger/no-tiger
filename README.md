# no-tiger
Not Ordinary Tiger Language

## Introduction

no-tiger is the course project for Compiler, Zhejiang University, Spring 2019.

## Build
```bash
mkdir build && cd build && cmake3 ..
make
```

## Reference

1. [ANSI-C-grammar-lex](http://www.quut.com/c/ANSI-C-grammar-l-2011.html)

2. [ANSI-C-grammar-yacc](http://www.quut.com/c/ANSI-C-grammar-y-2011.html)


## Roadmap

- [x] Use Rust-like type names , eg: `f32, f64, u8, u16, u32, u64, i8, i16, i32, i64`
- [ ] Support basic control flow
- [ ] Support function
- [ ] Support builtin array
- [ ] Support custom data type
- [ ] Support builtin I/O functions
- [ ] For simplicity, I think we should not support pointer for now