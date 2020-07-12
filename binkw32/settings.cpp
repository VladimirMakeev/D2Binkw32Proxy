/*
 * This file is part of the binkw32 proxy dll for Disciples 2.
 * (https://github.com/VladimirMakeev/D2Binkw32Proxy)
 * Copyright (C) 2020 Vladimir Makeev.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settings.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace hooks {

UserSettings settings;

const UserSettings& userSettings()
{
    return settings;
}

void readUserSettings()
{
    HMODULE module = GetModuleHandle(NULL);

    std::string iniPath(MAX_PATH, '\0');
    GetModuleFileName(module, &iniPath[0], MAX_PATH - 1);

    const auto n = iniPath.rfind('\\');
    iniPath.replace(n, iniPath.length() - n, "\\disciple.ini");

    const char disciple[] = "Disciple";
    settings.showResources = GetPrivateProfileInt(disciple,
                                                  "ShowResources",
                                                  settings.showResources,
                                                  iniPath.c_str())
                             != 0;
    settings.showBanners = GetPrivateProfileInt(disciple,
                                                "ShowBanners",
                                                settings.showBanners,
                                                iniPath.c_str())
                           != 0;
    settings.debugMode = GetPrivateProfileInt(disciple,
                                              "DebugHooks",
                                              settings.debugMode,
                                              iniPath.c_str())
                         != 0;
}

} // namespace hooks
