# FDump - The Memory Process File System

usage: fdump <filename>
command:
d [offset] dump from new offset (eg. d 3f8)
d continue dump from current offset
f <offset> <data> fill <data> from <offset>
s <data> search <data> from current offset
s continue search

example:
>d 3000
>d
>f 11111 ‘this is a book’
>f 33333 ‘this is a book’
>f 55555 ‘this is a book’
>s ‘this is a book’
>s
>s
>f 22222 12 34 56 78 90 aa bb cc dd ee ff
>f 44444 12 34 56 78 90 aa bb cc dd ee ff
>f 66666 12 34 56 78 90 aa bb cc dd ee ff
>s 12 34 56 78 90 aa bb cc dd ee ff
>s
>s