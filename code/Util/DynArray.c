#include <cstdlib>
#include <cstdint>

#include "Dynarray.h"

struct DynArray 
{
    void **Items;
    int size;
    int capacity;
};

inline void 
DynArrayInit(DynArray *, int) 
{
 
}
