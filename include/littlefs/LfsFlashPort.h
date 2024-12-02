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
        LfsFlashPort(bsp::IFlash *flash);

        /// @brief 获取被本类包装的 littlefs 库所需要的端口 —— lfs_config.
        /// @return
        lfs_config const &Port() const;
    };

    void TestLittleFs();
} // namespace Lfs
