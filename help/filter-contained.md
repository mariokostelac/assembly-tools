# filter-contained

Tool filters all overlaps associated with contained reads.

Let's say we have
```
A -------------
B    -----
```

After running `filter-contained < overlaps.afg > overlaps.filtered.afg`,
`overlaps.filtered.afg` will contain of all overlaps that do not
coincidence with read B.

## Installation
```
make filter-contained
```

## Usage
```
./bin/filter-contained < overlaps.afg > overlaps.filtered.afg
```
