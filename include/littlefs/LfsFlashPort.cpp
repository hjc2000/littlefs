#include "LfsFlashPort.h"

void Lfs::LfsFlashPort::InitializeLfsPort()
{
    _lfs.context = this;

    _lfs.read = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void *buffer, lfs_size_t size) -> int
    {
        try
        {
            DI_Console().WriteLine("read" + std::to_string(size));
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            self->_flash->Read(block, off, reinterpret_cast<uint8_t *>(buffer), size);
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            DI_Console().WriteLine("read catch");
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.prog = [](lfs_config const *c, lfs_block_t block,
                   lfs_off_t off, void const *buffer, lfs_size_t size) -> int
    {
        try
        {
            DI_Console().WriteLine("prog" + std::to_string(size));
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            lfs_size_t have_write = 0;
            while (have_write < size)
            {
                self->_flash->Program(block, off, reinterpret_cast<uint8_t const *>(buffer) + have_write);
                have_write += self->_flash->ProgrammingSize();
            }

            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            DI_Console().WriteLine("prog catch");
            return lfs_error::LFS_ERR_IO;
        }
    };

    _lfs.erase = [](lfs_config const *c, lfs_block_t block) -> int
    {
        try
        {
            DI_Console().WriteLine("erase" + std::to_string(block));
            LfsFlashPort *self = reinterpret_cast<LfsFlashPort *>(c->context);
            self->_flash->EraseSector(block);
            return lfs_error::LFS_ERR_OK;
        }
        catch (...)
        {
            DI_Console().WriteLine("erase catch");
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
    _lfs.block_cycles = -1;
    _lfs.cache_size = 256;
    _lfs.lookahead_size = 256;
}

Lfs::LfsFlashPort::LfsFlashPort(bsp::IFlash *flash)
{
    _flash = flash;
    InitializeLfsPort();
}

lfs_config const &Lfs::LfsFlashPort::Port() const
{
    return _lfs;
}

#include <bsp-interface/flash/RmaFlash.h>

void Lfs::TestLittleFs()
{
    // Lfs::LfsFlashPort port{DI_FlashCollection().Get("internal-flash")};

    bsp::RmaFlash ram_flash{
        "flash",
        base::Span{
            reinterpret_cast<uint8_t *>(0XC0000000),
            32 * 1024 * 1024,
        },
    };

    Lfs::LfsFlashPort port{&ram_flash};

    // 初始化 LittleFS 文件系统
    lfs_t lfs;
    int res = lfs_format(&lfs, &port.Port()); // 格式化文件系统
    if (res)
    {
        DI_Console().WriteLine("format failed: " + std::to_string(res));
    }

    DI_Console().WriteLine("666666");

    res = lfs_mount(&lfs, &port.Port()); // 挂载文件系统
    if (res)
    {
        DI_Console().WriteLine("mount failed: " + std::to_string(res));
    }

    // 创建文件并写入数据
    char const *filename = "example.txt";
    char const *str = "Hello, LittleFS!";
    lfs_file_t file;

    res = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
    if (res)
    {
        DI_Console().WriteLine("open failed: " + std::to_string(res));
    }

    res = lfs_file_write(&lfs, &file, str, strlen(str));
    if (res < 0)
    {
        DI_Console().WriteLine("write failed: " + std::to_string(res));
    }

    lfs_file_close(&lfs, &file);

    // 读取文件内容
    res = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
    if (res)
    {
        DI_Console().WriteLine("open failed: " + std::to_string(res));
    }

    char buffer[128] = {0};
    res = lfs_file_read(&lfs, &file, buffer, sizeof(buffer) - 1);
    if (res < 0)
    {
        DI_Console().WriteLine("read failed: " + std::to_string(res));
    }

    DI_Console().WriteLine("have read: " + std::to_string(res));
    DI_Console().WriteLine(buffer);

    lfs_file_close(&lfs, &file);

    // 卸载文件系统
    lfs_unmount(&lfs);
}
