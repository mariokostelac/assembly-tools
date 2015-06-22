# assembly-tools
Useful tools for assembling the genome

## Preparing
```
git clone https://github.com/mariokostelac/assembly-tools.git
git submodule update --init
make lib
make
```

## Tools list
- [layout](src/layout/README.md) - simplifies layout graph
- [overlap2dot](help/overlap2dot.md) - transforms overlaps to dot graph
- [filter-contained](help/filter-contained.md) - filters overlaps
  coincident with contained reads.
- [filter-transitive](help/filter-transitive.md) - filters transitive overlaps

## General requirements
- make
- gcc


