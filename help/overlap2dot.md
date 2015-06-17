# overlap2dot
Converts overlaps to dot graph.

## Requirements
- make
- gcc
- graphviz (for plotting part)

## Installation

```
  make
```

## Usage

```
  ./bin/overlap2dot < overlaps.afg > graph.dot
```

Or even better, for direct plotting to a file

```
  ./bin/overlap2dot < overlaps.afg | neato -T png -o graph.png
```
