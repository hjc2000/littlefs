#include <new>
#include <stddef.h>
#include <stdint.h>

extern "C"
{
    void *lfs_malloc(size_t size)
    {
        return new (std::nothrow) uint8_t[static_cast<size_t>(size)];
    }

    void lfs_free(void *p)
    {
        if (p == nullptr)
        {
            return;
        }

        delete[] static_cast<uint8_t *>(p);
    }
}
