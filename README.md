[![Build Status](https://travis-ci.org/mariokostelac/assembly-tools.svg?branch=master)](https://travis-ci.org/mariokostelac/assembly-tools)
# DEPRECATED IN FAVOR OF https://github.com/mariokostelac/ra
# assembly-tools
Useful tools for assembling the genome

## Preparing
```
git clone https://github.com/mariokostelac/assembly-tools.git
make get-deps
make
```

## Tools list
- [layout](src/layout/README.md) - simplifies layout graph
- [overlap2dot](help/overlap2dot.md) - transforms overlaps to dot graph
- [filter-contained](help/filter-contained.md) - filters overlaps
  coincident with contained reads.
- [filter-transitive](help/filter-transitive.md) - filters transitive overlaps

## General requirements
- GNU make
- g++ (4.8 or higher)
