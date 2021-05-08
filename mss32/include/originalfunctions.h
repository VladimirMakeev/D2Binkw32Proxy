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

#ifndef ORIGINALFUNCTIONS_H
#define ORIGINALFUNCTIONS_H

#include "battlemsgdata.h"
#include "citystackinterf.h"
#include "d2osexception.h"
#include "ddcarryoveritems.h"
#include "enclayoutspell.h"
#include "exchangeinterf.h"
#include "game.h"
#include "midunit.h"
#include "pickupdropinterf.h"
#include "sitemerchantinterf.h"

namespace hooks {

struct OriginalFunctions
{
    game::ComputeDamage computeDamage;
    game::ComputeUnitEffectiveHp computeUnitEffectiveHp;
    game::ComputePlayerDailyIncome computePlayerDailyIncome;
    game::CreateBatAttack createBatAttack;
    game::GetAttackClassWardFlagPosition getAttackClassWardFlagPosition;
    game::AttackClassToString attackClassToString;
    game::MarkMapPosition markMapPosition;
    game::CCityStackInterfApi::Api::Constructor cityStackInterfCtor;
    game::CExchangeInterfApi::Api::Constructor exchangeInterfCtor;
    game::CPickUpDropInterfApi::Api::Constructor pickupDropInterfCtor;
    game::CSiteMerchantInterfApi::Api::Constructor siteMerchantInterfCtor;
    game::CDDCarryOverItemsApi::Api::Constructor carryOverItemsCtor;
    game::CEncLayoutSpellApi::Api::Constructor encLayoutSpellCtor;
    game::os_exceptionApi::Api::ThrowException throwException;
    game::CMidUnitApi::Api::AddRemoveModifier removeModifier;
    game::BattleMsgDataApi::Api::SetUnknown9Bit1AndClearBoostLowerDamage
        setUnknown9Bit1AndClearBoostLowerDamage;
    game::BattleMsgDataApi::Api::FindAttackTarget findAttackTarget;
    game::GetSoldierImmunityPower getSoldierImmunityPower;
    game::GetAttackSourceWardFlagPosition getAttackSourceWardFlagPosition;
};

OriginalFunctions& getOriginalFunctions();

} // namespace hooks

#endif // ORIGINALFUNCTIONS_H
