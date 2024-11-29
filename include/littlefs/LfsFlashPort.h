#pragma once
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

    public:
        /// @brief
        /// @param flash_name 要用来实现 lfs_config 接口的 flash 的名称。
        LfsFlashPort(std::string const &flash_name);

        /// @brief 获取被本类包装的 littlefs 库所需要的端口 —— lfs_config.
        /// @return
        lfs_config const &Port() const;
    };
} // namespace Lfs
