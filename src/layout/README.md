# Layout
Layout simplifies overlaps graph and outputs `contigs.afg` file.

## Usage
```
usage: ./bin/layout --reads=string --overlaps=string [options] ...
options:
  -r, --reads              reads file (string)
  -x, --overlaps           overlaps file (string)
  -v, --verbose            verbose output; if set to 1, outputs intermediate afg files (bool [=0])
  -f, --overlaps_format    overlaps file format; supported: afg, mhap (string [=afg])
  -?, --help               print this message
```

## Steps
- filtering containment edges
- filtering transitive edges
- trimming
- bubble popping
