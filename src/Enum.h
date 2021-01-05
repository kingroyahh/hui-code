#ifndef _GRAV_ENUM_H_
#define _GRAV_ENUM_H_

namespace battle
{
    enum BattleFailCode
    {
        Error_Normal = 0,
        Error_CheckRoundStart = 1,
        Error_CheckRoundEnd = 2,
        Error_CheckAtkValid = 3,
    };
    enum BattleRoundState
    {
        RS_Default = 0,
        RS_Started = 1,
        RS_Attacking = 2,
        RS_Ended = 3,
    };

    enum SkillType
    {
        Invalid = 0,
        CommonSkillType = 1,
        LegendSkillType = 2,
        XpSkillType = 3,
        CombineSkillType = 4,
    };

    enum ActorActionState
    {
        NoneState,
        BeforeBuff,
        CastSkill,
        AfterBuff,
        Extra,
    };

    enum BattleStateType
    {
        SelfRoundEnd = 1,
        AllRoundEnd = 2,
        Immediately = 3,
    };

    enum PeriodType
    {
        PSelfRoundStart = 1,
        PSelfRoundEnd = 2,
        PAllRoundStart = 3,
        PAllRoundEnd = 4,
    };

    enum PeriodEffectType
    {
        PSubEffect = 1,
        PSkill = 2,
        PEffect = 3,
    };

    enum TriggerEffectType
    {
        SelfSubEffect = 1,
        SelfSkill = 2,
        SelfBuff = 3,
        CasterSubEffect = 4,
        CasterBuff = 5,
        TargetSubEffect = 6,
        TargetBuff = 7,
        SelfApplyAffix = 8,
    };

    // meta enum
    enum LastEffectType
    {
        Paralysis = 1,
        Seal = 2,
        Dizzy = 3,
        BuffShield = 4,
        Immune = 5,
    };

    enum FuncType
    {
        DamageFunc = 1,
        HealFunc = 2,
        ChangeXpFunc = 3,
        DispelFunc = 4,
        ResilienceFunc = 5,
        HealFormHpMax = 6,
        RelifeAndHeal = 7,
        ImmuneAndHeal = 8,
        HealSet = 9,
    };

    enum ModConditionType
    {
        SelfMoreHp = 1,
        SelfLessHp = 2,
        SelfMoreXp = 3,
        SelfLessXp = 4,
        SelfHaveMod = 5,
        TargetHaveMod = 6,
        TargetMoreHp = 7,
        TargetLessHp = 8,
        TargetMoreXp = 9,
        TargetLessXp = 10,
    };

    enum TriggerType
    {
        AddNotTri = 0,
        AddTriOnce = 1,
    };

    enum BuffDuraType
    {
        CasterRoundStart = 0,
        TargetRoundStart = 1,
        TargetRoundEnd = 2,
    };

   /* enum TriggerCondition
    {
        NotTrigger = 0,
        BeforeActionTri = 1,
        HitTri = 2,
        DeathTri = 3,
    };*/

    enum BuffPeriodConType
    {
        Ignore = -1,
        Default = 0,
        RoundCheck = 1,
        RoundKeep = 2,
    };

    //effect enum
    enum TargetType
    {
        Enemy = 1,
        Friend = 2,
    };

    //subTar enum
    enum SubTargetType
    {
        NoTar = 0,
        AllTar = 1,
        MainTar = 2,
        OtherTar = 3,
        LowerstXpTar = 4,
        HighestXpTar = 5,
    };

    enum TargetFilter
    {
        Self = 0,
        OneFront = 1,
        All = 2,
        FrontRow = 3,
        BackRow = 4,
        OneColumn = 5,
        Cross = 6,
        LowestHp = 7,
        Random = 8,
        SelfRL = 9,
        OneBack = 10,
        DeadTar = 11,
        HighestXp = 12,
        SameCountry = 13,
        SelfRow = 14,
        LowestXp = 15,
        OtherFirst = 16,
        TargetAndRL = 17,
        C1 = 18,
        C2 = 19,
        BuffCaster = 20,
        BuffOwner = 21,
    };

    enum LogicEffectType
    {
        SubEffectType = 1,
        BuffEffectType = 2,
        SpEffectType = 3,
    };

    enum DamageType
    {
        NoneDamageType = 0,
        ClosePhysical = 1,
        FireMagical = 2,
        RemotePhysical = 3,
        IceMagical = 4,
        ThunderMagical = 5,
    };

    enum BuffType
    {
        NoneBuff = 0,
        bigBuff = 1,
        bigDebuff = 2,
    };

    enum ActorAttrType
    {
        HpAttrPercent = 1,
        HpMax = 2,
        HpSkillPercent = 6,

        Attack = 10,
        PhysicalAttack = 11,
        MagicalAttack = 12,
        ClosePhysicalAttack = 13,
        RemotePhysicalAttack = 14,
        IceMagicalAttack = 15,
        FireMagicalAttack = 16,
        ThunderMagicalAttack = 17,

        AttackPercentAddition = 20,

        Defense = 30,
        PhysicalDefense = 31,
        MagicalDefense = 32,
        ClosePhysicalDefense = 33,
        RemotePhysicalDefense = 34,
        IceMagicalDefense = 35,
        FireMagicalDefense = 36,
        ThunderMagicalDefense = 37,

        DefensePercentAddition = 40,

        SkillDamageAddition = 50,
        SkillPhysicalDamageAddition = 51,
        SkillMagicalDamageAddition = 52,
        SkillCloseDamageAddition = 53,
        SkillRemoteDamageAddition = 54,
        SkillIceDamageAddition = 55,
        SkillFireDamageAddition = 56,
        SkillThunderDamageAddition = 57,

        AttrDamageAddition = 60,
        AttrPhysicalDamageAddition = 61,
        AttrMagicalDamageAddition = 62,
        AttrCloseDamageAddition = 63,
        AttrRemoteDamageAddition = 64,
        AttrIceDamageAddition = 65,
        AttrFireDamageAddition = 66,
        AttrThunderDamageAddition = 67,

        SkillDamageReduction = 70,
        SkillPhysicalDamageReduction = 71,
        SkillMagicalDamageReduction = 72,
        SkillCloseDamageReduction = 73,
        SkillRemoteDamageReduction = 74,
        SkillIceDamageReduction = 75,
        SkillFireDamageReduction = 76,
        SkillThunderDamageReduction = 77,

        AttrDamageReduction = 80,
        AttrPhysicalDamageReduction = 81,
        AttrMagicalDamageReduction = 82,
        AttrCloseDamageReduction = 83,
        AttrRemoteDamageReduction = 84,
        AttrIceDamageReduction = 85,
        AttrFireDamageReduction = 86,
        AttrThunderDamageReduction = 87,

        HealAddition = 90,
        OnHealAddition = 91,

        HealPercent = 95,
        OnHealPercent = 96,

        HitChance = 100,
        PhysicalHit = 101,
        MagicalHit = 102,
        ClosePhysicalHit = 103,
        RemotePysicalHit = 104,
        IceMagicalHit = 105,
        FireMagicalHit = 106,
        ThunderMagicalHit = 107,

        DodgeChance = 110,
        PhysicalDodge = 111,
        MagicalDodge = 112,
        ClosePhysicalDodge = 113,
        RemotePhysicalDodge = 114,
        IceMagicalDodge = 115,
        FireMagicalDodge = 116,
        ThunderMagicalDodge = 117,

        StrikeChance = 120,
        PhysicalStrike = 121,
        MagicalStrike = 122,
        ClosePhysicalStrike = 123,
        RemotePhysicalStrike = 124,
        IceMagicalStrike = 125,
        FireMagicalStrike = 126,
        ThunderMagicalStrike = 127,
        HealStrikeRate = 128,

        AntiStrikeChance = 130,
        PhysicalAntiStrike = 131,
        MagicalAntiStrike = 132,
        ClosePhysicalAntiStrike = 133,
        RemotePhysicalAntiStrike = 134,
        IceMagicalAntiStrike = 135,
        FireMagicalAntiStrike = 136,
        ThunderMagicalAntiStrike = 137,

        StrikeDamagePercentAddition = 140,

        StrikeDamagePercentReduction = 150,

        BlockChance = 160,
        PhysicalBlock = 161,
        MagicalBlock = 162,
        ClosePhysicalBlock = 163,
        RemotePhysicalBlock = 164,
        IceMagicalBlock = 165,
        FireMagicalBlock = 166,
        ThunderMagicalBlock = 167,

        AntiBlockChance = 170,
        PhysicalAntiBlock = 171,
        MagicalAntiBlock = 172,
        ClosePhysicalAntiBlock = 173,
        RemotePhysicalAntiBlock = 174,
        IceMagicalAntiBlock = 175,
        FireMagicalAntiBlock = 176,
        ThunderMagicalAntiBlock = 177,

        NationDamageAddition1 = 180,
        NationDamageAddition2 = 181,
        NationDamageAddition3 = 182,

        NationDamageReduction1 = 190,
        NationDamageReduction2 = 191,
        NationDamageReduction3 = 192,

        LifeSteal = 200,
        Resilience = 210,

        FinalDamageAddition = 230,
        FinalDamageReduction = 240,


        LevelDamagePercentAddition = 300,
        LevelHpPercentAddition = 301,

        PvpDmgAddition = 310,
        PvpDmgReduction = 311,
        PvpHealRate = 312,
        PvpHealedRate = 313,
        
        XpInit = 320,
        InjuredXpRecoveryRate = 321,
        AtkExtraXpRecovery = 322,
        EnemyDeadXpRecovery = 323,
        FriendDeadXpRecovery = 324,
        KillSomeOneXpRecovery = 325,
        RoundBeginXpRecovery = 326,
        XpMax = 327,
        XpGainRate = 328,
        HitXpRecovery = 329,
        CastSkillXpRecovery = 330,
        HealXpRecovery = 331,
        therapySufferDamage = 332,
        therapyCauseBlock = 335,
        therapySufferStrike = 334,
        therapySufferBlock = 333,
        therapyCauseStrike = 336,
        ActorAttrCount = 337,
        
        DoubleHitAffect = 1000,
        DoubleHitRate = 1001,
        DoubleHitRateForceZero = 1002,
    };

    //event type
    enum EventType{
        GetDamage = 1,
        SetDamage = 2,
        DeadSelf = 3,
        DeadSomeone = 4,
        WhenCastSkill = 5,
        SpecialEffect = 6,
        GetSpecialEffect = 7,
        BattleStart = 8,
        SpecialRoundStart = 9,
        SpecialRoundEnd = 10,
        WhenDodge = 11,
        WhenStrike = 12,
        WhenBlock = 13,
        DamageHit = 14,
        GetDamageHit = 15,
        KillSomeone = 16,
        WhenHeal = 17,
        DeadEnemy = 18,
        DeadFriend = 19,
        WhenCastTypeSkill = 21,
    };


    //analysis enum
    enum AnalysisField{
        DEFAULT = 0,
        skill_default,
        skill_legend,
        skill_xp,
        skill_combine,
        hp_init,
        hp,
        damage,
        times_damage,
        times_damaged,
        heal,
        damage_default,
        times_default,
        damage_legend,
        times_legend,
        damage_xp,
        times_xp,
        damage_combine,
        times_combine,
        times_strike,
        times_dodge,
        times_block,
        times_lifesteal,
        times_resilience,
        times_kill,
        dead_round,
    };
}
#endif
