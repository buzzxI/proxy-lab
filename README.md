this is csapp proxy lab project(offline)

`csapp.c` and `csapp.h` is not suppied in this project
> you need to fetch `csapp.c` and `csapp.h` from official website, complie `csapp.c` into `csapp.so`
> move `csapp.so` to `/usr/local/lib` and `csapp.h` to `/usr/local/include`

original `README` has be renamed as `LAB_README`

```shell
# build with:
$ make all
```

this branch implememts a cached proxy

switch to cmake to manage this project

`sbuf.c` implements a safe dequed buffer, thread pool is based on that buffer

`map.c` used array of linked list to implement a simple map, which is used as cache in proxy