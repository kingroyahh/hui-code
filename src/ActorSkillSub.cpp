#include "Type.h"
#include "ActorSkillSub.h"
#include "ActorModifier.h"
#include "ActorSkill.h"
#include "EffectUtil.h"
#include "Actor.h"
#include "BattleField.h"
#include "SkillMeta.h"
#include "EffectMeta.h"
#include "EventData.h"
#include "Random.h"
#include "Enum.h"
#include "Log.h"
#include "Calculator.h"
#include "AnalysisUtil.h"
#include "BattleArea.h"
#include "ModifierUtils.h"
ActorSkillSub::ActorSkillSub(ActorSkillPtr skill)
{
    ActorSkill = skill;
    AttackEffectCnt = 0;
    xpChange = 0;
    memset(SubEffectIDArray, 0, sizeof(SubEffectIDArray));
    memset(BuffIDArray, 0, sizeof(BuffIDArray));
    memset(AttackEffects, 0, sizeof(EffectSub*)*MAX_EFFECT_COUNT);
}

ActorSkillSub::~ActorSkillSub()
{
    TRACE("ActorSkillSub::~ActorSkillSub");
    for (int i = 0; i < MAX_EFFECT_COUNT;i++)
    {
        if (AttackEffects[i] != NULL)
        {
            delete AttackEffects[i];
            AttackEffects[i] = NULL;
        }
    }
}

bool ActorSkillSub::InitSkill(int skillId, int skillLv,bool addXpValue)
{
    TRACE("ActorSkillSub::InitSkill : skillID[%d]  skillLV[%d]\n", skillId, skillLv);
    //TODO: SKILL lv
    skillMeta = SkillMetaSet::instance()->Get(skillId);
    if (skillMeta == NULL_PTR)
    {
        ActorPtr actor = ActorSkill->GetOwner();
        error("skill is not right : %d,Actor[%d] Area[%d]Pos[%d]\n", skillId, actor->templateId,actor->areaIndex,actor->posIndex);
        return false;
    }
    this->skillId = skillId;
    this->xpChange = skillMeta->ExpChange;
    this->skillLv = skillLv;
    AttackEffectCnt = 0;
    for (int i = 0; i < MAX_EFFECT_COUNT; ++i)
    {
        if (skillMeta->AttackEffects[i] != NULL && skillMeta->AttackEffects[i] != 0)
        {
            debug("create attack effect : %d, skillLv : %d", skillMeta->AttackEffects[i], skillLv);
            AttackEffects[AttackEffectCnt] = new EffectSub(ActorSkill->GetOwner(), skillMeta->AttackEffects[i], skillLv);
            TriggerTrace traceData;
            traceData.skillId = skillId;
            traceData.effectId = skillMeta->AttackEffects[i];
            AttackEffects[AttackEffectCnt]->MakeTrace(traceData);
            AttackEffectCnt++;
        }
    }
    return true;
}

bool ActorSkillSub::DoSkill(int targetPos, SkillInfo& sInfo)
{
    return DoSkill(targetPos, std::move(sInfo));
}


bool ActorSkillSub::DoSkill(int targetPos, SkillInfo&& sInfo)
{
    TRACE("ActorSkillSub::DoSkill : Skill[%d], actorId : %d\n", skillMeta->id, ActorSkill->GetOwner()->actorId);
    if (skillMeta == NULL)
    {
        error("skillMeta == NULL");
        return false;
    }
    if (ActorSkill == NULL)
    {
        error("ActorSkill == NULL");
        return false;
    }
    if (ActorSkill->GetOwner() == NULL)
    {
        error("ownSkill->owner == NULL");
        return false;
    }
    BattleFieldPtr pBattleField = ActorSkill->GetOwner()->battleField;
    if (pBattleField == NULL)
    {
        error("battleField == NULL");
        return false;
    }
    if (pBattleField->curReplayCmd == NULL)
    {
        error("pBattleField->curReplayCmd == NULL");
        return false;
    }

    pBattleField->curOrder = 0;

    /*WhenCastSkillEventData skillData;
    skillData.caster = ActorSkill->GetOwner();
    skillData.skillId = skillMeta->id;
    pBattleField->whenCastSkillTigger.Trigger(&skillData);

    WhenCastTypeSkillEventData typeSkillData;
    typeSkillData.caster = ActorSkill->GetOwner();
    typeSkillData.skillType = skillMeta->skillType;
    pBattleField->whenCastTypeSkillTrigger.Trigger(&typeSkillData);*/

   
    bool isRedo = false;
    bool isSetMainTarget = false;

    TriggerParam param1;
    param1.evtType = EEventType::BeforeSkill;
    param1.c1.list[0] = ActorSkill->GetOwner();
    param1.r7 = skillId;
    param1.level = skillLv;
    param1.doubleParams[0] = skillMeta->RedoRate;
    param1.hasResult = false;
    ActorSkill->GetOwner()->battleField->FireEvent(param1);
    
    //check redo
    int rate = pBattleField->random.RandForWidth(100);
    double v = skillMeta->RedoRate;
    if(sInfo.skillType !=CommonSkillType && param1.hasResult){
        v = param1.resultDouble;
        param1.hasResult = false;
        param1.resultDouble = 0;
    }
    if (rate <= v * 100)
    {
        info("trigger redo: actorId: %d skillId: %d\n", ActorSkill->GetOwner()->actorId, skillMeta->id);
        isRedo = true;
    }
    int isResetTarget = skillMeta->ReChoose;
    int atkIndex = 0;

    
    if (isResetTarget == 0)
    {
        for (int i = 0; i < AttackEffectCnt; ++i)
        {
            if (AttackEffects[i] != NULL_PTR)
            {
                AttackEffects[i]->GetEffectTarget(targetPos, NULL, NULL, NULL, NULL);
            }
        }
    }

    bool mainCmdInited = false;
    for (int i = 0; i < AttackEffectCnt; ++i)
    {
        if (AttackEffects[i] != NULL_PTR)
        {
            
            if (isResetTarget == 1)
            {
                AttackEffects[i]->GetEffectTarget(targetPos, NULL, NULL, NULL, NULL);
            }
            
            if (pBattleField->curSkillCmd == NULL)
            {
                error("pBattleField->curSkillCmd == NULL");
                return false;
            }
            if (!mainCmdInited)
            {
                int mainTargetID = (AttackEffects[i]->GetMainTarget() != NULL) ? AttackEffects[i]->GetMainTarget()->actorId : 0;
                pBattleField->AddSkillMainCmd(ActorSkill->GetOwner()->actorId, skillMeta->id, mainTargetID);
                mainCmdInited = true;
            }
            AttackEffects[i]->ExecuteEffect(sInfo);
        }
    }
    return isRedo;
}

int ActorSkillSub::RegisterSubeffectID(int subEffID)
{
    TRACE("ActorSkillSub::RegisterSubeffectID skillID[%d] int subEffID[%d]", skillMeta->id, subEffID);
    int indexCnt = 0;
    for (int i = 0; i < MaxSubEffectCnt; i++)
    {
        if (SubEffectIDArray[i] == 0)
        {
            SubEffectIDArray[i] = subEffID;
            return indexCnt;
        }
        else if (SubEffectIDArray[i] == subEffID)
        {
            indexCnt++;
        }
    }
    return indexCnt;
}

int ActorSkillSub::RegisterBuffID(int buffID)
{
    TRACE("ActorSkillSub::RegisterBuffID skillID[%d] int buffID[%d]", skillMeta->id, buffID);
    int indexCnt = 0;
    for (int i = 0; i < MaxBuffCnt; i++)
    {
        if (BuffIDArray[i] == 0)
        {
            BuffIDArray[i] = buffID;
            return indexCnt;
        }
        else if (BuffIDArray[i] == buffID)
        {
            indexCnt++;
        }
    }
    return indexCnt;
}

EffectTrigger::~EffectTrigger()
{

}

void EffectTrigger::ExecuteTrigger(ActorPtr target, float tarDmgParam, int skillLv, bool isMain, bool isLowestXp, bool isHighestXp, SkillType skillType)
{
    TRACE("EffectTrigger::ExecuteTrigger targetID[%d] isMain[%d] isLowestXp[%d] isHightestXp[%d]", target->actorId, isMain, isLowestXp, isHighestXp);
    
    if (CheckSubTarget(target, isMain, isLowestXp, isHighestXp))
    {
        ExecuteTriggerImpl(target, isMain,tarDmgParam, skillLv,skillType);
    }
}

bool EffectTrigger::CheckSubTarget(ActorPtr target, bool isMain, bool isLowestXp, bool isHighestXp)
{
    
    if (firstSubTarget)
    {
        return CheckSubTarget(effParent->GetEffectMeta()->SubTarget, isMain, isLowestXp, isHighestXp);
    }
    else
    {
        return CheckSubTarget(effParent->GetEffectMeta()->SubTarget2, isMain, isLowestXp, isHighestXp);
    }
}


bool EffectTrigger::CheckSubTarget(SubTargetType subTarget, bool isMain, bool isLowestXp, bool isHighestXp)
{
    TRACE("EffectTrigger::CheckSubTarget SubTargetType[%d]", subTarget);
    switch (subTarget)
    {
    case NoTar:
        return false;
    case AllTar:
        return true;
    case MainTar:
        if (GetParent()->GetEffectMeta() != NULL)
        {
            return isMain;
        }
        return false;
    case OtherTar:
    {
        if (GetParent()->GetEffectMeta() != NULL)
        {
            return !isMain;
        }
        return false;
    }
    case LowerstXpTar:
    {
        return isLowestXp;
    }
    case HighestXpTar:
    {
        return isHighestXp;
    }
    default:
        break;
    }
    return false;
}

void SubEffectTrigger::ExecuteTriggerImpl(ActorPtr target, bool isMainTarget, float tarDmgParam, int skillLv, SkillType skillType)
{
    TRACE("SubEffectTrigger::ExecuteTriggerImpl");
    ActorPtr caster = effParent->GetCaster();
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(effTriggerID);
    if (subMeta != NULL_PTR)
    {
        this->effParent->SetCurOrder(effOrderID);
        TriggerTrace tc;
        tc.skillId = this->effParent->traceData.skillId;
        tc.effectId = effParent->EffectId();
        tc.subEffectId = effTriggerID;
        tc.buffId = effParent->GetOriginBuffID();
//        error("tc %d %d %d %d",tc.skillId,tc.effectId,tc.subEffectId,tc.buffId);
        makeEffectByType(caster, target, subMeta, tarDmgParam, skillLv, effParent->GetOriginBuffID(),skillType,tc);
    }
    else
    {
        error("subEffect meta cant find: %d\n", effTriggerID);
    }
}

void SubEffectTrigger::Init(EffectSub * parent, int triggerID,bool first)
{
    EffectTrigger::Init(parent, triggerID, first);
    effOrderID = parent->RegisterSubeffectID(triggerID);
}

void SpEffectTrigger::ExecuteTriggerImpl(ActorPtr target, bool isMainTarget, float tarDmgParam, int skillLv, SkillType skillType)
{
    TRACE("SpEffectTrigger::ExecuteTriggerImpl effTriggerID[%d] tarDmgParam[%f]", effTriggerID, tarDmgParam);
    if (effTriggerID == 0)
    {
        return;
    }
    ActorPtr caster = effParent->GetCaster();
    
    //send a event before a buff
    TriggerParam param1;
    param1.evtType = EEventType::BeforeEffectAddSpEffect;
    param1.pEffect = effParent;
    param1.c1.list[0] = caster;
    param1.c2.list[0] = target;
    param1.r5 = effTriggerID;
    param1.r6 = effParent->EffectId();
    param1.r7 = this->effParent->traceData.skillId;
    param1.r8 = effParent->GetOriginBuffID();
    param1.level = skillLv;
//    error("ExecuteTriggerImpl %d %d %d %d",param1.r5,param1.r6,param1.r7,param1.r8);
    caster->battleField->FireEvent(param1);
    
    
    float rate = caster->battleField->random.RandForWidth(100);
    
    auto chance = hitProbability;
    if(param1.hasResult){
        chance = hitProbability * param1.resultDouble +param1.resultDouble2;
    }
   
    if (rate > chance)
    {
        return;
    }
    const SubEffectsMeta *subMeta = SubEffectsMetaSet::instance()->Get(effTriggerID);
    if (subMeta != NULL_PTR)
    {
        this->effParent->SetCurOrder(effOrderID);
        TriggerTrace tc;
        tc.skillId = this->effParent->traceData.skillId;
        tc.effectId = effParent->EffectId();
        tc.buffId = effParent->GetOriginBuffID();
        tc.subEffectId = effTriggerID;
        makeEffectByType(caster, target, subMeta, tarDmgParam, skillLv, effParent->GetOriginBuffID(),skillType,tc);
    }
    else
    {
        error("subEffect meta cant find: %d\n", effTriggerID);
    }
}

void BuffEffectTrigger::ExecuteTriggerImpl(ActorPtr target, bool isMainTarget, float tarDmgParam, int skillLv, SkillType skillType)
{
    TRACE("BuffEffectTrigger::ExecuteTriggerImpl effTriggerID[%d]", effTriggerID);
    if (effTriggerID == 0)
    {
        return;
    }
    ActorPtr caster = effParent->GetCaster();
    
    //send a event before a buff
    TriggerParam param1;
    param1.evtType = EEventType::BeforeEffectAddBuff;
    param1.pEffect = effParent;
    param1.c1.list[0] = caster;
    param1.c2.list[0] = target;
    param1.r5 = effTriggerID;
    param1.r6 = effParent->EffectId();
    param1.r7 = this->effParent->traceData.skillId;
    param1.r8 = effParent->GetOriginBuffID();
    param1.level = skillLv;
    caster->battleField->FireEvent(param1);
    
    float rate = caster->battleField->random.RandForWidth(100);
    int exChance = 0;
    if (effParent != nullptr)
    {
        exChance = effParent->GetExChanceValue(buffOrderID);
        if(exChance != 0){
            if (rate > hitProbability + exChance)
            {
                return;
            }
        }else{ // support No.2 chance check
            double chance = hitProbability;
            if(param1.hasResult){
                chance = hitProbability* param1.resultDouble +param1.resultDouble2;
            }
            if (rate > chance)
            {
                return;
            }
            
        }
    }
    this->effParent->SetCurOrder(buffOrderID);
    target->actorMod->AddMod(caster, effTriggerID, skillLv, -1);
}

void BuffEffectTrigger::Init(EffectSub * parent, int triggerID, bool firstSubTarget)
{
    EffectTrigger::Init(parent, triggerID, firstSubTarget);
    buffOrderID = parent->RegisterBuffID(triggerID);
}

EffectSub::EffectSub(ActorPtr caster, int eId, int sLv)
{
    TRACE("EffectSub::EffectSub effectID[%d]", eId);
    m_pMainTarget = NULL;
    originBuffID = 0;
    m_pCaster = caster;
    m_pSkillSub = NULL;
    m_iEffectId = eId; //effectId
    m_iTriggerIndex = 0;
    skillLv = sLv;
    
    memset(m_pTriggerEffectList, 0, sizeof(m_pTriggerEffectList));
    memset(m_pTargetList, 0, sizeof(m_pTargetList));
    m_pEffectMeta = (EffectMeta*)EffectMetaSet::instance()->Get(m_iEffectId);
    if (m_pEffectMeta == NULL)
    {
        error("EffectMeta is not existed ,EffectID %d", m_iEffectId);
        return;
    }


    for (int i = 0; i < MAX_DMG_PARAM; ++i)
    {
        if (m_pEffectMeta->TriggerSeq[i] != NULL && m_pEffectMeta->TriggerSeq[i] != 0)
        {
            switch ((LogicEffectType)m_pEffectMeta->TriggerSeq[i]) {
            case SubEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {

                    AppendSubEffTrigger(m_pEffectMeta->SubEffectsPara[i], true);
                }
                break;
            case BuffEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {
                    AppendBuffEffTrigger(m_pEffectMeta->BuffPara[i], m_pEffectMeta->BuffChance[i], true);
                }
                break;
            case SpEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {
                    AppendSpSubEffTrigger(m_pEffectMeta->SpEffects[i], m_pEffectMeta->SpChance[i], true);
                }
                break;
            default:
                error("not have this triggerseq type effect id : %d", m_pEffectMeta->id);
                break;
            }
        }
    }
    

    for (int i = 0; i < MAX_DMG_PARAM; ++i)
    {
        if (m_pEffectMeta->TriggerSeq2[i] != NULL && m_pEffectMeta->TriggerSeq2[i] != 0)
        {
            switch ((LogicEffectType)m_pEffectMeta->TriggerSeq2[i]) {
            case SubEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {
                    AppendSubEffTrigger(m_pEffectMeta->SubEffectsPara2[i], false);
                }
                break;
            case BuffEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {
                    AppendBuffEffTrigger(m_pEffectMeta->BuffPara2[i], m_pEffectMeta->BuffChance2[i], false);
                }
                break;
            case SpEffectType:
                for (int i = 0; i < MAX_DMG_PARAM; ++i)
                {
                    AppendSpSubEffTrigger(m_pEffectMeta->SpEffects2[i], m_pEffectMeta->SpChance2[i], false);
                }
                break;
            default:
                error("not have this triggerseq type effect id : %d", m_pEffectMeta->id);
                break;
            }
        }
    }
}

EffectSub::~EffectSub()
{
    TRACE("EffectSub::~EffectSub");
    m_pMainTarget = NULL_PTR;
    m_pCaster = NULL;
    m_pSkillSub = NULL;
    m_pEffectMeta = NULL;
    
    for (int i = 0; i < MaxSubEffectCnt;i++)
    {
        if (m_pTriggerEffectList[i] != NULL)
        {
            delete m_pTriggerEffectList[i];
            m_pTriggerEffectList[i] = NULL;
        }
    }
    
    memset(m_pTriggerEffectList, 0, sizeof(m_pTriggerEffectList));
    memset(m_pTargetList, 0, sizeof(m_pTargetList));
}


void EffectSub::GetEffectTarget(int targetPos, ActorList* c1, ActorList *c2, Actor* buffCaster, Actor *buffOwner)
{
    TRACE("EffectSub::GetEffectTarget");

    m_pMainTarget = NULL;
    memset(m_pTargetList, 0, sizeof(m_pTargetList));
    if (m_pEffectMeta == NULL_PTR)
    {
        return;
    }
    if (m_pCaster == NULL)
    {
        return;
    }
    SelectEffectTargets(this,targetPos,c1,c2,buffCaster,buffOwner);
}

void EffectSub::ExecuteEffect(const SkillInfo& sInfo)
{
    if (m_pEffectMeta == nullptr)
    {
        error("EffectMeta is not existed ,EffectID %d", m_iEffectId);
        return;
    }
    TRACE("EffectSub::ExecuteEffect Root Caster[%d] EffectID:[%d]",
        m_pCaster->actorId, m_pEffectMeta->id);
    if (m_pMainTarget != NULL_PTR)
    {
        if (!m_pMainTarget->IsAlive())
        {
            debug("effect maintarget is dead. effId : %d, targetId : %d", m_pEffectMeta->id, m_pMainTarget->actorId);
            return;
        }
    }
    if (m_pEffectMeta == NULL)
    {
        return;
    }
    
    if (m_pEffectMeta->DoEffect)
    {
        TriggerParam param1,param2;
        param1.evtType = EEventType::CauseSkill;
        param2.evtType = EEventType::SufferSkill;
        param1.skillType = sInfo.skillType;
        param2.skillType = sInfo.skillType;
        param1.c1.list[0] = m_pCaster;
        param2.c2.list[0] = m_pCaster;
        int targetIndex = 0;
        for (int j = 0; j < MAX_DMG_TARGET_COUNT; ++j)
        {
            if (m_pTargetList[j] != NULL_PTR && m_pTargetList[j]->IsAlive())
            {
                param1.c2.list[targetIndex] = m_pTargetList[j];
                param2.c1.list[targetIndex] = m_pTargetList[j];
                targetIndex++;
            }
        }
        if (sInfo.skillType == CommonSkillType || sInfo.skillType == LegendSkillType)
        {
            param1.r2 = sInfo.DoubleHitRank;
            param1.r3 = sInfo.ComAtkIndex;
            param1.r4 = sInfo.doubleHitDamageRate;
        }
        param1.pEffect = this;
        m_pCaster->battleField->FireEvent(param1);
        if (sInfo.skillType == CommonSkillType || sInfo.skillType == LegendSkillType)
        {
            param2.r2 = param1.r2;
            param2.r3 = param1.r3;
            param2.r4 = param1.r4;
        }
        param2.pEffect = this;
        m_pCaster->battleField->FireEvent(param2);
    }
    
    float tarDmgParam = 0;
    //TODO: ARRAY OUT OF INDEX

    if (m_pEffectMeta->TargetDmgParam[0] != 0)
    {
        int targetNum = 0;
        for (int j = 0; j < MAX_DMG_TARGET_COUNT; ++j)
        {
            if (m_pTargetList[j] != NULL_PTR && m_pTargetList[j]->IsAlive())
            {
                targetNum++;
            }
        }
        tarDmgParam = m_pEffectMeta->TargetDmgParam[targetNum - 1];
    }

    int lowestXpId = 999;
    int hightestXpId = 999;
    int tarNum = 0;
    ActorPtr targetTemp = NULL_PTR;

    if (m_pEffectMeta->SubTarget == LowerstXpTar || m_pEffectMeta->SubTarget2 == LowerstXpTar)
    {

        for (int i = 0; i < MAX_DMG_TARGET_COUNT; ++i)
        {
            if (m_pTargetList[i] != NULL_PTR && m_pTargetList[i]->IsAlive())
            {
                if (targetTemp == NULL_PTR)
                {
                    targetTemp = m_pTargetList[i];
                }
                else if (0 < m_pTargetList[i]->actorDatas.hp && m_pTargetList[i]->actorDatas.xp <= targetTemp->actorDatas.xp)
                {
                    targetTemp = m_pTargetList[i];
                }
            }
        }
        
        ActorPtr tempTargets[MAX_DMG_TARGET_COUNT] = { 0 };
        for (int i = 0; i < MAX_DMG_TARGET_COUNT; ++i)
        {
            if (m_pTargetList[i] != NULL_PTR && m_pTargetList[i]->IsAlive())
            {
                if (0 < m_pTargetList[i]->actorDatas.hp && m_pTargetList[i]->actorDatas.xp == targetTemp->actorDatas.xp)
                {
                    tempTargets[tarNum] = m_pTargetList[i];
                    tarNum++;
                }
            }
        }

        int index = m_pCaster->battleField->random.RandForWidthFromZero(tarNum);
        lowestXpId = tempTargets[index]->actorId;
    }


    tarNum = 0;
    if (m_pEffectMeta->SubTarget == HighestXpTar || m_pEffectMeta->SubTarget2 == HighestXpTar)
    {
        for (int i = 0; i < MAX_DMG_TARGET_COUNT; ++i)
        {
            if (m_pTargetList[i] != NULL_PTR && m_pTargetList[i]->IsAlive())
            {
                if (targetTemp == NULL_PTR)
                {
                    targetTemp = m_pTargetList[i];
                }
                else if (0 < m_pTargetList[i]->actorDatas.hp && m_pTargetList[i]->actorDatas.xp > targetTemp->actorDatas.xp)
                {
                    targetTemp = m_pTargetList[i];
                }
            }
        }
        ActorPtr tempTargets[MAX_DMG_TARGET_COUNT] = { 0 };
        for (int i = 0; i < MAX_DMG_TARGET_COUNT; ++i)
        {
            if (m_pTargetList[i] != NULL_PTR && m_pTargetList[i]->IsAlive())
            {
                if (0 < m_pTargetList[i]->actorDatas.hp && m_pTargetList[i]->actorDatas.xp == targetTemp->actorDatas.xp)
                {
                    tempTargets[tarNum] = m_pTargetList[i];
                    tarNum++;
                }
            }
        }

        int index = m_pCaster->battleField->random.RandForWidthFromZero(tarNum);
        hightestXpId = tempTargets[index]->actorId;
    }

    if (modifyAffixList.size() > 0)
    {
        for (auto & actorInfo : modifyAffixList)
        {
            for (auto & attrInfo : actorInfo.attrInfoList)
            {
                ApplyAffix(actorInfo.pActor, attrInfo.first, attrInfo.second, actorInfo.tempInfo);
            }
        }
    }
    

    for (int j = 0; j < MAX_DMG_TARGET_COUNT; ++j)
    {
        if (m_pTargetList[j] != NULL_PTR && m_pTargetList[j]->IsAlive())
        {
            debug("executeEffect : %d, skillLv : %d", m_pEffectMeta->id, skillLv);
            bool isMain = false;
            if (m_pMainTarget != NULL_PTR)
            {
                if (m_pMainTarget->actorId == m_pTargetList[j]->actorId)
                {
                    isMain = true;
                }
            }
            bool isLowestXp = false;
            bool isHighestXp = false;
            if (m_pTargetList[j]->actorId == lowestXpId)
            {
                isLowestXp = true;
            }
            if (m_pTargetList[j]->actorId == hightestXpId)
            {
                isHighestXp = true;
            }
            executeEffectForTarget(m_pTargetList[j], tarDmgParam, skillLv, isMain, isLowestXp, isHighestXp,sInfo.skillType);
        }
    }

    if (modifyAffixList.size() > 0)
    {
        for (auto & actorInfo : modifyAffixList)
        {
            RemoveAffix(actorInfo.pActor, actorInfo.tempInfo);
        }
        modifyAffixList.clear();
    }
    if (exBuffChanceList.size() > 0)
    {
        exBuffChanceList.clear();
    }
}


void EffectSub::executeEffectForTarget(ActorPtr target, float tarDmgParam, int skillLv, bool isMain, bool isLowestXp, bool isHighestXp, SkillType skillType)
{
    //if calculate hit
    TRACE("EffectSub::ExecuteEffect Child Caster [%d],targetId [%d] EffectID [%d]",
        m_pCaster->actorId,
        target->actorId,
        m_pEffectMeta->id);


    if (m_pEffectMeta->TargetType == Enemy)
    {

        if (!m_pEffectMeta->NoDodge)
        {
            float rate = m_pCaster->battleField->random.RandForWidth(100);
            // use close
            float chance = CalculateHitChance(ClosePhysical, m_pCaster, target) * 100;
            if (rate > chance)
            {
                /*WhenDodgeEventData dData;
                dData.caster = m_pCaster;
                dData.target = target;
                m_pCaster->battleField->whenDodgeTrigger.Trigger(&dData);*/
                int subEffId = m_pEffectMeta->SubEffectsPara[0];
                FillDodgeCmd(m_pCaster,target, subEffId,originBuffID);
                
                
                TriggerParam param1;
                param1.evtType = EEventType::Miss;
                param1.c1.list[0] = m_pCaster;
                param1.c2.list[0] = target;
                m_pCaster->battleField->FireEvent(param1);

               
                TriggerParam param2;
                param2.evtType = EEventType::Dodge;
                param2.c1.list[0] = target;
                param2.c2.list[0] = m_pCaster;
                m_pCaster->battleField->FireEvent(param2);
                return;
            }
        }
    }
  
    for (int i = 0; i < m_iTriggerIndex; i++)
    {
        EffectTrigger* trigger = m_pTriggerEffectList[i];
        if (trigger == NULL)
        {
            continue;
        }
        trigger->ExecuteTrigger(target, tarDmgParam, skillLv, isMain, isLowestXp, isHighestXp,skillType);
    }
}


void EffectSub::AppendSubEffTrigger(int triggerID,bool first)
{
    TRACE("EffectSub::AppendSubEffTrigger triggerID[%d] first[%d]", triggerID, first);
    if (triggerID == 0)
    {
        return;
    }
    EffectTrigger* trigger = new SubEffectTrigger();
    trigger->Init(this, triggerID,first);
    AppendTriggerEffect(trigger);
}

void EffectSub::AppendSpSubEffTrigger(int triggerID, int probability, bool first)
{
    TRACE("EffectSub::AppendSpSubEffTrigger triggetID[%d] probablity[%d] first[%d]", triggerID, probability, first);
    if (triggerID == 0)
    {
        return;
    }
    SpEffectTrigger* trigger = new SpEffectTrigger();
    trigger->Init(this, triggerID,first);
    trigger->InitProbability(probability);
    AppendTriggerEffect(trigger);
}

void EffectSub::AppendBuffEffTrigger(int triggerID, int probability, bool first)
{
    TRACE("EffectSub::AppendBuffEffTrigger triggerID[%d] probalility[%d] first[%d]", triggerID, probability, first);
    if (triggerID == 0)
    {
        return;
    }
    BuffEffectTrigger* trigger = new BuffEffectTrigger();
    trigger->Init(this, triggerID,first);
    trigger->InitProbability(probability);
    AppendTriggerEffect(trigger);
}


void EffectSub::AppendTriggerEffect(EffectTrigger* effect)
{
    m_pTriggerEffectList[m_iTriggerIndex] = effect;
    m_iTriggerIndex++;
}
void EffectSub::SetCurOrder(int order)
{
    m_pCaster->battleField->curOrder = order;
}

int EffectSub::RegisterSubeffectID(int subID)
{
    if (m_pSkillSub != NULL)
    {
        return m_pSkillSub->RegisterSubeffectID(subID);
    }
    return 0;
}

int EffectSub::RegisterBuffID(int buffID)
{
    if (m_pSkillSub != NULL)
    {
        return m_pSkillSub->RegisterBuffID(buffID);
    }
    return 0;
}

void EffectSub::ModifyExtraAffix(Actor* pActor, int affixID, int affixBase, int affixGrow)
{
    std::vector<ActorAffix>::iterator it;
    ActorAffix* pAA = nullptr;
    for (auto & actorAffix : modifyAffixList)
    {
        if (actorAffix.pActor == pActor)
        {
            pAA = &actorAffix;
        }
    }
    if (pAA == nullptr)
    {
        modifyAffixList.push_back(ActorAffix());
        pAA = &modifyAffixList.back();
        pAA->pActor = pActor;
    }
    if (pAA == nullptr)
    {
        error("ModifyExtraAffix pointer to ActorAffix is not existant");
        return;
    }
    int realSkillLevel = skillLv;
    if (realSkillLevel >= 1)
    {
        realSkillLevel--;
    }
    int affixPower = affixBase + affixGrow * realSkillLevel;
    pAA->attrInfoList.push_back(std::make_pair(affixID, affixPower));
}
