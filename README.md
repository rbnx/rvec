# rvec
RAII dynamic array in C

## About
rvec is a lightweight and generic dynamic array library that makes use of GCC/Clang's cleanup extension to release the memory being used by the array once it goes out of scope.

## Usage
```c
rvec_t(type) v;
rvec_init(v);
rvec_push(v,val);
```

```c
#include <stdio.h>
#include "rvec.h"


int main(int argc, const char *argv[]) {
    rvec_t(int) v1;
    rvec_init(v1);

    printf("cap: %zu, size: %zu\n", rvec_capacity(v1), rvec_size(v1));

    for(size_t i = 0; i < 1000; i++) {
      rvec_push(v1,i);
    }

    printf("last item: %d\n",rvec_pop(v1));

    printf("1st item: %d\n",rvec_get(v1,0));
    // or 
    printf("1st item: %d\n",rvec_i(v1,0));

    rvec_set(v1,0,999);
    // or
    rvec_i(v1,0) = 999;

    rvec_resize(v1,100);

    {
      rvec_t(double *) v2;
      rvec_init(v2);
    } // <--- v2 freed here

    return 0;
} // <-- v1 freed here
```
## API

### rvec_init(v)
Initialises the array, it must be called before any other function.
### rvec_size(v)
Evaluates to the size of the array. 
### rvec_capacity(v)
Evaluates to the capacity of the array.
### rvec_push(v,x)
Adds a new element at the end of the array.
### rvec_pop(v)
Pops and returns the last value.
### rvec_i(v,i)
Expands to `(v)->data[(i)]`, it can be used like this: `rvec_i(v,0) = value;` or `x = rvec_i(v,0);`.
### rvec_set(v,i,x)
`rvec_set(v,index,value);`.
### rvec_get(v,i)
`rvec_get(v,index);`.
### rvec_begin(v)
Returns a pointer to the first element of the array.
### rvec_end(v)
Returns a pointer to one-past-the-end of the array, (like std::vector::end).
### rvec_resize(v,sz)
Resizes the array.
### rvec_copy(dest,src)
Copies the elements of `src` into `dest`. `dest` will be resized as needed.
### rvec_destroy
Destroys the array. Only useful if you are using the library without RAII support (see `#RVEC_NO_RAII` in rvec.h).

## License
https://github.com/rbnx/rvec/blob/master/rvec.h#L1-L5