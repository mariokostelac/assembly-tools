# filter-contained

Tool filters all overlaps associated with contained reads.

Let's say we have
```
A -------------
B    -----
```

After running `filter-contained < overlaps.afg > overlaps.filtered.afg`,
`overlaps.filtered.afg` will contain all overlaps not coincident with with read B.

## Installation
```
make filter-contained
```

## Usage
```
./bin/filter-contained < overlaps.afg > overlaps.filtered.afg
```
