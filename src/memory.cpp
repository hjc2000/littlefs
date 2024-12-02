#include <bsp-interface/di/console.h>
#include <new>
#include <stddef.h>
#include <stdint.h>

extern "C"
{
    void *lfs_malloc(size_t size)
    {
        void *p = new (std::nothrow) uint8_t[static_cast<size_t>(size)];
        std::string msg = "lfs_malloc new " + std::to_string(size);
        if (p == nullptr)
        {
            DI_Console().WriteLine(msg + " failed");
        }
        else
        {
            DI_Console().WriteLine(msg + " successfully");
        }

        return p;
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
