/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
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

#ifndef ATTACKREACHCAT_H
#define ATTACKREACHCAT_H

#include "categories.h"

namespace game {

struct LAttackReachTable : CEnumConstantTable<AttackReachId>
{ };

struct LAttackReach : public Category<AttackReachId>
{ };

namespace AttackReachCategories {

struct Categories
{
    LAttackReach* all;
    LAttackReach* any;
    LAttackReach* adjacent;
};

Categories& get();

/** Returns address of LAttackReach::vftable used in game. */
const void* vftable();

} // namespace AttackReachCategories

namespace LAttackReachTableApi {

using Api = CategoryTableApi::Api<LAttackReachTable, LAttackReach>;

Api& get();

/** Returns address of LAttackReachTable::vftable used in game. */
const void* vftable();

} // namespace LAttackReachTableApi

} // namespace game

#endif // ATTACKREACHCAT_H
