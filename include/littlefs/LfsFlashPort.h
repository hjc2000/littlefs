#pragma once
#include <bsp-interface/di/console.h>
#include <bsp-interface/di/flash.h>
#include <lfs.h>

namespace Lfs
{
    /// @brief 用 flash 实现 lfs_config 接口。
    class LfsFlashPort
    {
    private:
        bsp::IFlash *_flash = nullptr;
        lfs_config _lfs{};

        void InitializeLfsPort();

    public:
        /// @brief
        /// @param flash_name 要用来实现 lfs_config 接口的 flash 的名称。
        LfsFlashPort(std::string const &flash_name);

        /// @brief 获取被本类包装的 littlefs 库所需要的端口 —— lfs_config.
        /// @return
        lfs_config const &Port() const;
    };

    inline void TestLittleFs()
    {
        Lfs::LfsFlashPort port{"internal-flash"};

        // 初始化 LittleFS 文件系统
        lfs_t lfs;
        int res = lfs_format(&lfs, &port.Port()); // 格式化文件系统
        if (res)
        {
            DI_Console().WriteLine("format failed: ");
        }

        res = lfs_mount(&lfs, &port.Port()); // 挂载文件系统
        if (res)
        {
            DI_Console().WriteLine("mount failed: ");
        }

        // 创建文件并写入数据
        char const *filename = "example.txt";
        char const *str = "Hello, LittleFS!";
        lfs_file_t file;

        res = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
        if (res)
        {
            DI_Console().WriteLine("open failed: ");
        }

        res = lfs_file_write(&lfs, &file, str, strlen(str));
        if (res < 0)
        {
            DI_Console().WriteLine("write failed: ");
        }

        lfs_file_close(&lfs, &file);

        // 读取文件内容
        res = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
        if (res)
        {
            DI_Console().WriteLine("open failed: ");
        }

        char buffer[128] = {0};
        res = lfs_file_read(&lfs, &file, buffer, sizeof(buffer) - 1);
        if (res < 0)
        {
            DI_Console().WriteLine("read failed: ");
        }

        DI_Console().WriteLine("have read: ");
        DI_Console().WriteLine(buffer);

        lfs_file_close(&lfs, &file);

        // 卸载文件系统
        lfs_unmount(&lfs);
    }
} // namespace Lfs
