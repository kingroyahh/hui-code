#include "ActorSkill.h"
#include "ActorSkillSub.h"
#include "Actor.h"
#include "EffectMeta.h"
#include "ActorModifier.h"
#include "BattleField.h"
#include "BattleArea.h"
#include "AnalysisUtil.h"
#include "Type.h"
#include "Enum.h"
#include "Log.h"
#include "Calculator.h"
#include "EffectUtil.h"
#if defined(_MSC_VER)
#pragma warning(disable : 4703)
#endif

ActorSkill::ActorSkill()
{
    m_pOwner = NULL_PTR;
    defaultSkill = NULL_PTR;
    xpSkill = NULL_PTR;
    combineSkill = NULL_PTR;
    legendSkill = NULL_PTR;
}

ActorSkill::~ActorSkill()
{
    TRACE("ActorSkill::~ActorSkill");
    if (defaultSkill != NULL_PTR)
    {
        delete defaultSkill;
        defaultSkill = NULL;
    }
    if (xpSkill != NULL_PTR)
    {
        delete xpSkill;
        xpSkill = NULL;
    }
    if (combineSkill != NULL_PTR)
    {
        delete combineSkill;
        combineSkill = NULL;
    }
    if (legendSkill != NULL_PTR)
    {
        delete legendSkill;
        legendSkill = NULL;
    }
}
ActorPtr ActorSkill::GetOwner()
{
    return m_pOwner;
}
void ActorSkill::InitOwner(ActorPtr actor)
{
    TRACE("ActorSkill::InitOwner: actor templateId:[%d]", actor->templateId);
    m_pOwner = actor;
}

void ActorSkill::InitDefaultSkill(int skillId, int skillLv)
{
    TRACE("ActorSkill::InitDefaultSkill actorID[%d] skillID[%d]", m_pOwner->actorId, skillId);
    defaultSkill = new ActorSkillSub(this);
    if (!defaultSkill->InitSkill(skillId, skillLv,true))
    {
        delete defaultSkill;
        defaultSkill = NULL_PTR;
    }
}

void ActorSkill::InitXpSkill(int skillId, int skillLv)
{
    TRACE("ActorSkill::InitXpSkill actorID[%d] skillID[%d]", m_pOwner->actorId, skillId);
    xpSkill = new ActorSkillSub(this);
    if (!xpSkill->InitSkill(skillId, skillLv,false))
    {
        delete xpSkill;
        xpSkill = NULL_PTR;
    }
}

void ActorSkill::InitCombineSkill(int skillId, int skillLv)
{
    TRACE("ActorSkill::InitCombineSkill actorID[%d] skillID[%d]", m_pOwner->actorId, skillId);
    combineSkill = new ActorSkillSub(this);
    if (!combineSkill->InitSkill(skillId, skillLv,false))
    {
        delete combineSkill;
        combineSkill = NULL_PTR;
    }
}

void ActorSkill::InitLegendSkill(int skillId, int skillLv)
{
    TRACE("ActorSkill::InitLegendSkill actorID[%d] skillID[%d]", m_pOwner->actorId, skillId);
    legendSkill = new ActorSkillSub(this);
    if (!legendSkill->InitSkill(skillId, skillLv,true))
    {
        delete legendSkill;
        legendSkill = NULL_PTR;
    }
}

bool ActorSkill::AllowSkill()
{
    TRACE("ActorSkill::AllowSkill");
    if (!m_pOwner->IsAlive())
    {
        debug("actor is dead : %d\n", m_pOwner->actorId);
        return false;
    }
    if (m_pOwner->actorMod->IsRestrictAction())
    {
        debug("actor is restrict id : %d\n", m_pOwner->actorId);
        return false;
    }
    if (xpSkill == NULL_PTR)
    {
        debug("actor xpSkill == null : %d\n", m_pOwner->actorId);
        return false;
    }
    if (!m_pOwner->actorDatas.CanDoXpSkill())
    {
        debug("actor CanDoXpSkill is false : %d\n", m_pOwner->actorId);
        return false;
    }
    if (m_pOwner->actorMod->IsSealedState())
    {
        debug("actor Is in Sealed State : %d\n", m_pOwner->actorId);
        return false;
    }
    return true;
}

bool ActorSkill::AllowComAtk()
{
    TRACE("ActorSkill::AllowComAtk");
    //if (m_pOwner->battleField->InfiniteSkill)
    //{
    //    return true;
    //}
    if (!m_pOwner->IsAlive())
    {
        debug("actor is dead : %d\n", m_pOwner->actorId);
        return false;
    }
    if (m_pOwner->actorMod->IsRestrictAction())
    {
        debug("actor is restrict id : %d\n", m_pOwner->actorId);
        return false;
    }
    if (m_pOwner->actionCount > 0)
    {
        debug("owner->actionCount > 0: %d\n", m_pOwner->actorId);
        return false;
    }
    if (defaultSkill == NULL_PTR)
    {
        debug("defaultSkill == null: %d\n", m_pOwner->actorId);
        return false;
    }
    if (m_pOwner->actorMod->IsRestrictCommonSkill())
    {
        debug("owner->IsRestrictCommonSkill(): %d\n", m_pOwner->actorId);
        return false;
    }
    return true;
}

void ActorSkill::CastSkillByType(SkillType sType, int targetPos)
{
    TRACE("ActorSkill::CastSkillByType SkillTYpe[%d] Actor[%d]", sType, this->m_pOwner->actorId);
    if (!m_pOwner->IsAlive())
    {
        debug("actor is dead : %d\n", m_pOwner->actorId);
        return;
    }
    if (m_pOwner->actorMod->IsRestrictAction())
    {
        TRACE("actor is restrict id : %d\n", m_pOwner->actorId);
        return;
    }
    m_pOwner->battleField->curOrder = 0;
    switch (sType) 
    {
        case CommonSkillType:
            {
                // if legend skill condition is satisfied,then just induce it !
                if (m_pOwner->actorMod->IsRestrictCommonSkill())
                {
                    error("ActorSkill::CastSkillByType comAtk Actor[%d] is IsRestrictCommonSkill", m_pOwner->actorId);
                    return;
                }
                ActorSkillSubPtr comAtkSkill = NULL;
                
                if (legendSkill != NULL && m_pOwner->actorMod->IsLegendSkillActive())
                {
                    comAtkSkill = legendSkill;
                    sType = SkillType::LegendSkillType;
                }
                else if (defaultSkill != NULL)
                {
                    comAtkSkill = defaultSkill;
                }
                
                if (comAtkSkill != NULL)
                {
                    TRACE("actor:%d do common skill\n", m_pOwner->actorId);
                    m_pOwner->battleField->InitOneReplayCmd();
                    
                    
                    int xpChange = comAtkSkill->xpChange * m_pOwner->GetAttrByType(XpGainRate) * m_pOwner->GetAttrByType(AtkExtraXpRecovery);
                    if (xpChange > 0)
                    {
                        if (m_pOwner->ChangeXp(xpChange))
                        {
                            auto param = FillXpChangeParam(m_pOwner, m_pOwner, 0, xpChange);
                            param.recovery = XpRecoveryType::Attack;
                            FillXpChangeCmd(param);
                        }
                    }
                    m_pOwner->battleField->curComAtkIndex++;
                    m_pOwner->battleField->curReplayCmd->set_triggertime(m_pOwner->battleField->curAttackTime);
                    int doubleHitRank = -1;
                    m_pOwner->actorDatas.doubleHitRate = 0;
                    auto curAtkComboRule = (m_pOwner->areaIndex == 0) ? m_pOwner->battleField->CurBattleMode->ComboRule_Attacker : m_pOwner->battleField->CurBattleMode->ComboRule_Defender;
                    if (curAtkComboRule == EBMCombo::True)
                    {
                        const SkillMeta* lastSkill = m_pOwner->battleField->LastComSkill;
                        int *pDoubleHitTimeRadius = m_pOwner->battleField->doubleHitTimeRadius;
                        if (lastSkill != NULL && pDoubleHitTimeRadius != NULL)
                        {
                            int comboTime = m_pOwner->battleField->curAttackTime - m_pOwner->battleField->lastAttackTime;
                            int startCombo = lastSkill->Atk123Time + lastSkill->StartComboTime;
                            int endCombo = lastSkill->Atk123Time + lastSkill->EndComboTime;
                            int startScroll = lastSkill->Atk123Time + lastSkill->StartScrollTime;
                            int endScroll = lastSkill->Atk123Time + lastSkill->EndScrollTime;
                            if (comboTime >= startScroll && comboTime <= endScroll)
                            {
                                int comboTarget = (startCombo + endCombo)*0.5f;
                                bool forward = (comboTime > comboTarget) ? true : false;
                                int rankTime = (forward) ? (comboTime - comboTarget) : (comboTarget - comboTime);
                                int accuTime = 0;
                                for (int i = 0; i < DOUBLE_HIT_TIME_RADIUS_COUNT; i++)
                                {
                                    accuTime = accuTime + pDoubleHitTimeRadius[i];
                                    if (rankTime <= accuTime)
                                    {
                                        doubleHitRank = 4 - i;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                doubleHitRank = 0;
                            }
                            /*error("doublehit [%d] lastTime[%d],curAtk[%d],comboTime[%d],startCombo[%d],endCombo[%d],atk123[%d],start[%d],end[%d]",
                            success, owner->battleField->lastAttackTime, owner->battleField->curAttackTime, comboTime,
                            startCombo, endCombo, lastSkill->Atk123Time, lastSkill->StartComboTime, lastSkill->EndComboTime);*/
                        }
                        if (doubleHitRank > 0)
                        {
                            m_pOwner->battleField->doubleHitRate += m_pOwner->battleField->doubleHitDamageRate[doubleHitRank - 1];
                            m_pOwner->actorDatas.doubleHitRate = m_pOwner->battleField->doubleHitRate;
                            if (m_pOwner->areaIndex == 0)
                            {
                                m_pOwner->battleField->doubleHitTypeInfo[doubleHitRank - 1]++;
                            }
                        }
                        else if (doubleHitRank == 0)
                        {
                            if (m_pOwner->areaIndex == 0)
                            {
                                m_pOwner->battleField->doubleHitTypeInfo[0]++;
                            }
                        }
                        m_pOwner->battleField->curSkillCmd->set_doublehitrank(doubleHitRank);
                    }
                    
                    
                    SkillInfo sInfo;
                    sInfo.skillId = comAtkSkill->skillId;
                    sInfo.skillType = sType;
                    sInfo.ComAtkIndex = m_pOwner->battleField->curComAtkIndex;
                    sInfo.DoubleHitRank = doubleHitRank;
                    sInfo.doubleHitDamageRate = m_pOwner->actorDatas.doubleHitRate;
                    
                    if (comAtkSkill->DoSkill(targetPos,sInfo))
                    {
                        m_pOwner->battleField->InitOneReplayCmd();
                        m_pOwner->battleField->curReplayCmd->set_redo(true);
                        comAtkSkill->DoSkill(targetPos,sInfo);
                    }
                    
                    TriggerParam param1;
                    param1.evtType = EEventType::AfterSkill;
                    param1.c1.list[0] = m_pOwner;
                    param1.r7 = comAtkSkill->skillId;
                    param1.level = comAtkSkill->skillLv;
                    m_pOwner->battleField->FireEvent(param1);
                    
                    m_pOwner->actionCount = 1;
                    m_pOwner->battleField->LastComSkill = comAtkSkill->skillMeta;
                }
            }
            break;
        case XpSkillType:
            {
                m_pOwner->battleField->LastComSkill = NULL;
                m_pOwner->battleField->doubleHitRate = 0;
                m_pOwner->actorDatas.doubleHitRate = 0;
                if (m_pOwner->actorMod->IsSealedState())
                {
                    error("ActorSkill::CastSkillByType xpAtk Actor[%d] is IsSealedState", m_pOwner->actorId);
                    return;
                }
               
                if (!m_pOwner->actorDatas.CanDoXpSkill())
                {
                    error("ActorSkill::CastSkillByType xpAtk Actor[%d] is not CanDoXpSkill", m_pOwner->actorId);
                    return;
                }
                int xpRecovery = m_pOwner->actorAttrs.GetAttrByType(CastSkillXpRecovery);
                int resultXp = xpRecovery - m_pOwner->actorDatas.xpMax;
                if (m_pOwner->battleField->InfiniteSkill)
                {
                    resultXp = xpRecovery;
                }
               
                if (combineSkill != NULL && m_pOwner->battleField->GetOwnArea(m_pOwner->areaIndex)->ActorIsExist(combineSkill->skillMeta->PartnerId))
                {
                    TRACE("actor: %d do combine skill\n", m_pOwner->actorId);
                    //setCurrAnalysisField(owner, skill_combine);
                    m_pOwner->battleField->InitOneReplayCmd();
                    if (m_pOwner->ChangeXp(resultXp))
                    {
                        auto param = FillXpChangeParam(m_pOwner, m_pOwner, 0, resultXp);
                        param.recovery = XpRecoveryType::Skill;
                        FillXpChangeCmd(param);
                    }
                    SkillInfo sinfo = SkillInfo();
                    sinfo.skillType = CombineSkillType;
                    if (combineSkill->DoSkill(targetPos, sinfo))
                    {
                        m_pOwner->battleField->InitOneReplayCmd();
                        m_pOwner->battleField->curReplayCmd->set_redo(true);
                        combineSkill->DoSkill(targetPos, SkillInfo());
                    }
                    TriggerParam param1;
                    param1.evtType = EEventType::AfterSkill;
                    param1.c1.list[0] = m_pOwner;
                    param1.r7 = combineSkill->skillId;
                    param1.level = combineSkill->skillLv;
                    m_pOwner->battleField->FireEvent(param1);
                }
                else
                {
                    if (xpSkill == NULL)
                    {
                        error("ActorSkill::CastSkillByType xpAtk Actor[%d] xpSkill is NULL", m_pOwner->actorId);
                        return;
                    }
                    TRACE("actor:%d do xp skill\n", m_pOwner->actorId);
                    //setCurrAnalysisField(owner, skill_xp);
                    m_pOwner->battleField->InitOneReplayCmd();
                    if (m_pOwner->ChangeXp(resultXp))
                    {
                        auto param = FillXpChangeParam(m_pOwner, m_pOwner, 0, resultXp);
                        param.recovery = XpRecoveryType::Skill;
                        FillXpChangeCmd(param);
                    }
                    SkillInfo sinfo = SkillInfo();
                    sinfo.skillType = XpSkillType;
                    if (xpSkill->DoSkill(targetPos, sinfo))
                    {
                        m_pOwner->battleField->InitOneReplayCmd();
                        m_pOwner->battleField->curReplayCmd->set_redo(true);
                        xpSkill->DoSkill(targetPos, sinfo);
                    }
                    
                    TriggerParam param1;
                    param1.evtType = EEventType::AfterSkill;
                    param1.c1.list[0] = m_pOwner;
                    param1.r7 = xpSkill->skillId;
                    param1.level = xpSkill->skillLv;
                    m_pOwner->battleField->FireEvent(param1);
                }
            }
            break;
        default:
            break;
    }
}
