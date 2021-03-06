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

#ifndef MODIFIERUTILS_H
#define MODIFIERUTILS_H

#include "immunecat.h"
#include "midgardid.h"
#include <set>

namespace game {
struct IMidgardObjectMap;
struct CMidUnit;
struct UnitInfo;
struct CUmModifier;
struct CUmUnit;
struct LAttackSource;
struct LAttackClass;
struct IAttack;
struct BattleMsgData;
} // namespace game

namespace hooks {

bool unitCanBeModified(game::BattleMsgData* battleMsgData, game::CMidgardID* targetUnitId);

game::CUmUnit* castUmModifierToUmUnit(game::CUmModifier* modifier);

game::CUmModifier* getModifier(const game::CMidgardID* modifierId);

void getModifierAttackSource(game::CUmUnit* modifier, game::LAttackSource* value);

void getModifierAttackClass(game::CUmUnit* modifier, game::LAttackClass* value);

void resetUnitAttackSourceWard(game::BattleMsgData* battleMsgData,
                               const game::CMidgardID* unitId,
                               game::CUmUnit* modifier);

void resetUnitAttackClassWard(game::BattleMsgData* battleMsgData,
                              const game::CMidgardID* unitId,
                              game::CUmUnit* modifier);

bool canApplyImmunityModifier(game::BattleMsgData* battleMsgData,
                              const game::CMidUnit* targetUnit,
                              game::CUmUnit* modifier,
                              game::ImmuneId immuneId);

bool canApplyImmunityclassModifier(game::BattleMsgData* battleMsgData,
                                   const game::CMidUnit* targetUnit,
                                   game::CUmUnit* modifier,
                                   game::ImmuneId immuneId);

bool canApplyModifier(game::BattleMsgData* battleMsgData,
                      const game::CMidUnit* targetUnit,
                      const game::CMidgardID* modifierId);

bool canApplyAnyModifier(game::IAttack* attack,
                         game::IMidgardObjectMap* objectMap,
                         game::BattleMsgData* battleMsgData,
                         game::CMidgardID* targetUnitId);

void resetModifiedUnitsInfo(game::UnitInfo* unitInfo);

bool addModifiedUnitInfo(const game::CMidgardID* unitId,
                         game::BattleMsgData* battleMsgData,
                         game::CMidUnit* targetUnit,
                         const game::CMidgardID* modifierId);

bool applyModifier(const game::CMidgardID* unitId,
                   game::BattleMsgData* battleMsgData,
                   game::CMidUnit* targetUnit,
                   const game::CMidgardID* modifierId);

void removeModifier(game::BattleMsgData* battleMsgData,
                    game::CMidUnit* unit,
                    const game::CMidgardID* modifierId);

void removeModifiers(game::BattleMsgData* battleMsgData,
                     game::IMidgardObjectMap* objectMap,
                     game::UnitInfo* unitInfo,
                     const game::CMidgardID* modifiedUnitId);

std::set<game::CMidgardID> getModifiedUnitIds(game::UnitInfo* unitInfo);

std::set<game::CMidgardID> getUnitModifierIds(game::UnitInfo* unitInfo,
                                              const game::CMidgardID* modifiedUnitId);

} // namespace hooks

#endif // MODIFIERUTILS_H
