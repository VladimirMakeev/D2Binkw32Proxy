/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
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

#include "transformselfhooks.h"
#include "attack.h"
#include "batattacktransformself.h"
#include "battleattackinfo.h"
#include "battlemsgdata.h"
#include "customattacks.h"
#include "game.h"
#include "globaldata.h"
#include "log.h"
#include "midgardobjectmap.h"
#include "midunit.h"
#include "scripts.h"
#include "settings.h"
#include "unitgenerator.h"
#include "unitimplview.h"
#include "unitutils.h"
#include "unitview.h"
#include "ussoldier.h"
#include "usunitimpl.h"
#include "utils.h"
#include "visitors.h"
#include <fmt/format.h>

namespace hooks {

static int getTransformSelfLevel(const game::CMidUnit* unit, game::TUsUnitImpl* transformImpl)
{
    const auto path{scriptsFolder() / "transformSelf.lua"};
    const auto lua{loadScriptFile(path, true, true)};
    if (!lua) {
        return 0;
    }

    using GetLevel = std::function<int(const bindings::UnitView&, const bindings::UnitImplView&)>;
    auto getLevel = getScriptFunction<GetLevel>(*lua, "getLevel");
    if (!getLevel) {
        showErrorMessageBox(fmt::format("Could not find function 'getLevel' in script '{:s}'.\n"
                                        "Make sure function exists and has correct signature.",
                                        path.string()));
        return 0;
    }

    try {
        const bindings::UnitView attacker{unit};
        const bindings::UnitImplView impl{transformImpl};

        return (*getLevel)(attacker, impl);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to run '{:s}' script.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
        return 0;
    }
}

void __fastcall transformSelfAttackOnHitHooked(game::CBatAttackTransformSelf* thisptr,
                                               int /*%edx*/,
                                               game::IMidgardObjectMap* objectMap,
                                               game::BattleMsgData* battleMsgData,
                                               game::CMidgardID* targetUnitId,
                                               game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!getCustomAttacks().transformSelf.targetSelf) {
        auto altAttack = thisptr->altAttack;
        altAttack->vftable->onHit(altAttack, objectMap, battleMsgData, targetUnitId, attackInfo);
        return;
    }

    auto attack = fn.getAttackById(objectMap, &thisptr->id2, thisptr->attackNumber, false);
    auto attackId = IAttackApi::get().getId(attack);

    CMidgardID targetGroupId{emptyId};
    fn.getAllyOrEnemyGroupId(&targetGroupId, battleMsgData, targetUnitId, true);

    const auto position = fn.getUnitPositionInGroup(objectMap, &targetGroupId, targetUnitId);

    const CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    const CMidgardID targetUnitImplId{targetUnit->unitImpl->unitId};

    CMidgardID transformImplId{emptyId};
    fn.getSummonUnitImplIdByAttack(&transformImplId, attackId, position, isUnitSmall(targetUnit));

    if (transformImplId == emptyId) {
        return;
    }

    if (userSettings().leveledTransformSelfAttack != baseSettings().leveledTransformSelfAttack) {
        const auto& global = GlobalDataApi::get();
        auto globalData = *global.getGlobalData();

        auto transformImpl = static_cast<TUsUnitImpl*>(
            global.findById(globalData->units, &transformImplId));

        const auto transformLevel = getTransformSelfLevel(targetUnit, transformImpl);

        CUnitGenerator* unitGenerator = globalData->unitGenerator;

        CMidgardID leveledImplId{transformImplId};
        unitGenerator->vftable->generateUnitImplId(unitGenerator, &leveledImplId, &transformImplId,
                                                   transformLevel);

        unitGenerator->vftable->generateUnitImpl(unitGenerator, &leveledImplId);

        transformImplId = leveledImplId;
    }

    const auto& visitors = VisitorApi::get();
    visitors.transformUnit(targetUnitId, &transformImplId, false, objectMap, 1);

    const auto& battle = BattleMsgDataApi::get();

    if (userSettings().freeTransformSelfAttack != baseSettings().freeTransformSelfAttack) {
        auto& customTransformSelf = getCustomAttacks().transformSelf;
        if (customTransformSelf.freeAttackUnitId != targetUnit->unitId) {
            customTransformSelf.freeAttackUnitId = targetUnit->unitId;

            const auto soldier = fn.castUnitImplToSoldier(targetUnit->unitImpl);
            battle.giveAttack(battleMsgData, &targetUnit->unitId,
                              soldier->vftable->getAttackTwice(soldier) ? 2 : 1, 1);
        }
    }

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = targetUnitImplId;
    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);

    battle.removeTransformStatuses(targetUnitId, battleMsgData);
    battle.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::TransformSelf, true);

    battle.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
}

void __fastcall transformSelfAttackFillTargetsListHooked(game::CBatAttackTransformSelf* thisptr,
                                                         int /*%edx*/,
                                                         game::IMidgardObjectMap* objectMap,
                                                         game::BattleMsgData* battleMsgData,
                                                         game::TargetsList* targetsList)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& listApi = TargetsListApi::get();

    CMidgardID unitGroupId{emptyId};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, &thisptr->unitId, true);

    int unitPosition = fn.getUnitPositionInGroup(objectMap, &unitGroupId, &thisptr->unitId);

    auto altAttack = thisptr->altAttack;
    if (altAttack) {
        altAttack->vftable->fillTargetsList(altAttack, objectMap, battleMsgData, targetsList);

        CMidgardID targetGroupId{emptyId};
        altAttack->vftable->getTargetGroupId(altAttack, &targetGroupId, battleMsgData);

        if (targetGroupId != unitGroupId)
            unitPosition = -(unitPosition + 1);
    }

    Pair<TargetsListIterator, bool> tmp{};
    listApi.insert(targetsList, &tmp, &unitPosition);
}

} // namespace hooks
