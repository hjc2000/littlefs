#pragma once
#include <bsp-interface/di/flash.h>
#include <lfs.h>

namespace Lfs
{
    /// @brief 用 flash 实现 lfs_config 接口。
    class LfsFlashPort
    {
    private:
        std::string _flash_name;

    public:
        /// @brief
        /// @param flash_name 要用来实现 lfs_config 接口的 flash 的名称。
        LfsFlashPort(std::string const &flash_name);
    };
} // namespace Lfs
