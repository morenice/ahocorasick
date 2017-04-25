[![Build Status](https://travis-ci.org/morenice/ahocorasick.svg?branch=master)](https://travis-ci.org/morenice/ahocorasick)

C implementation Aho-Corasick string match.
- Support multi-thread
- Support callback match API
- Support multiple languages(english, korean, ...)
- example: See src/main.c


# Build
```sh
$ mkdir build; cd build
$ cmake ..
$ make
```


# Performance
## Prepare
Get sample data and Build source

```
wget http://storage.googleapis.com/books/ngrams/books/googlebooks-eng-all-1gram-20120701-0.gz
gzip -d googlebooks-eng-all-1gram-20120701-0.gz
```


## Testing (file: 184MB)
ahocorasick : pattern 1984, 1985
```
time ./ahocorasick

real    0m2.253s
user    0m2.180s
sys     0m0.067s
```

fgrep : pattern 1984, 1985
```
time fgrep -c -e 1984 -e 1985 googlebooks-eng-all-1gram-20120701-0

real    0m12.269s
user    0m11.389s
sys     0m0.081s
```

# See also
reference
- https://www.slideshare.net/ssuser81b91b/ahocorasick-algorithm
