/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef UNITUTILS_H
#define UNITUTILS_H

#include "linkedlist.h"

namespace game {
struct CMidgardID;
struct CMidUnit;
struct IUsUnit;
struct IUsSoldier;
struct TUsUnitImpl;
struct TUsSoldierImpl;
struct LImmuneCat;
struct LAttackSource;
struct IAttack;
} // namespace game

namespace hooks {

void generateUnitImplByAttackId(const game::CMidgardID* attackId);
game::IUsSoldier* castUnitImplToSoldierWithLogging(const game::IUsUnit* unitImpl);
void getSoldierAttackSourceImmunities(const game::LImmuneCat* immuneCat,
                                      const game::IUsSoldier* soldier,
                                      game::LinkedList<game::LAttackSource>* value);
bool isUnitSmall(const game::CMidUnit* unit);
game::CMidgardID getGlobalUnitImplId(const game::CMidUnit* unit);
game::TUsUnitImpl* getGlobalUnitImpl(const game::CMidUnit* unit);
game::TUsSoldierImpl* getSoldierImpl(const game::IUsSoldier* soldier);
game::IAttack* getAttack(const game::IUsSoldier* soldier, bool primary, bool checkAltAttack);

} // namespace hooks

#endif // UNITUTILS_H
