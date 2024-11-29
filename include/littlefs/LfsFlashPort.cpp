#include "LfsFlashPort.h"
#include <bsp-interface/di/console.h>

void Lfs::LfsFlashPort::InitializeLfsPort()
{
    _lfs.context = this;

    _lfs.read = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void *buffer, lfs_size_t size) -> int
    {
        try
        {
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            self->_flash->Read(block, off, reinterpret_cast<uint8_t *>(buffer), size);
            DI_Console().WriteLine("read ok");
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.prog = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void const *buffer, lfs_size_t size) -> int
    {
        try
        {
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            lfs_size_t have_write = 0;
            while (have_write < size)
            {
                self->_flash->Program(block, off, reinterpret_cast<uint8_t const *>(buffer));
                have_write += self->_flash->ProgrammingSize();
            }

            DI_Console().WriteLine("prog ok");
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.erase = [](lfs_config const *c, lfs_block_t block) -> int
    {
        try
        {
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            self->_flash->EraseSector(block);
            DI_Console().WriteLine("erase ok");
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.sync = [](lfs_config const *c) -> int
    {
        try
        {
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.lock = [](lfs_config const *c) -> int
    {
        try
        {
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.unlock = [](lfs_config const *c) -> int
    {
        try
        {
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.read_size = 1;
    _lfs.prog_size = _flash->ProgrammingSize();
    _lfs.block_size = _flash->SectorSize();
    _lfs.block_count = _flash->SectorCount();
    _lfs.block_cycles = 100;
    _lfs.cache_size = 128;
    _lfs.lookahead_size = 128;
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
