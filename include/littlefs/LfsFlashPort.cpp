#include "LfsFlashPort.h"

void Lfs::LfsFlashPort::InitializeLfsPort()
{
    _lfs.context = this;

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

    _lfs.read_size = 1;
    _lfs.prog_size = _flash->ProgrammingSize();
    _lfs.block_size = _flash->SectorSize();
    _lfs.block_count = _flash->SectorCount();
    _lfs.block_cycles = 100;
    _lfs.cache_size = 32;
    _lfs.lookahead_size = _flash->SectorCount() / 8;
}

Lfs::LfsFlashPort::LfsFlashPort(std::string const &flash_name)
{
    _flash = DI_FlashCollection().Get(flash_name);
    InitializeLfsPort();
}

lfs_config const &Lfs::LfsFlashPort::Port() const
{
    return _lfs;
}
