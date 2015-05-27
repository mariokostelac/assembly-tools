# filter-transitive

Tool filters all transitive overlaps and writes the rest to the standard
output.

## Installation
```
make filter-transitive
```

## Usage
```
./bin/filter-transitive <input_files...>
```
`-` filename stands for stdin
If no filenames are defined, it reads from stdin.

## Example usages
Reads and overlaps from separated files.
```
./bin/filter-transitive reads.afg overlaps.afg > overlaps.notran.afg
```

```
./bin/filter-transitive < reads_and_overlaps.afg > overlaps.notran.afg
```
