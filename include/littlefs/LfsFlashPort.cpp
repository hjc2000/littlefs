#include "LfsFlashPort.h"

Lfs::LfsFlashPort::LfsFlashPort(std::string const &flash_name)
{
    _flash = DI_FlashCollection().Get(flash_name);

    _lfs.context = this;
    _lfs.read_size = 1;
    _lfs.prog_size = _flash->ProgrammingSize();
    _lfs.block_size = _flash->SectorSize();

    _lfs.read = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void *buffer, lfs_size_t size) -> int
    {
        return 0;
    };

    _lfs.prog = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void const *buffer, lfs_size_t size) -> int
    {
        return 0;
    };

    _lfs.erase = [](lfs_config const *c, lfs_block_t block) -> int
    {
        return 0;
    };

    _lfs.sync = [](lfs_config const *c) -> int
    {
        return 0;
    };

    _lfs.lock = [](lfs_config const *c) -> int
    {
        return 0;
    };

    _lfs.unlock = [](lfs_config const *c) -> int
    {
        return 0;
    };
}

lfs_config const &Lfs::LfsFlashPort::Port() const
{
    return _lfs;
}
