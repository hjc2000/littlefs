#include "LfsFlashPort.h"

Lfs::LfsFlashPort::LfsFlashPort(std::string const &flash_name)
{
    _flash = DI_FlashCollection().Get(flash_name);
}
