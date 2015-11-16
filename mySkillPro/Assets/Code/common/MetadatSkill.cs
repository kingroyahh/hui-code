using UnityEngine;
using System.Collections;


public class MetadatSkill : MetadataLoader<Skill>
{
    public void Awake()
    {
        Path = "Metadata/arpg_skill";
    }

    public Skill GetSkillData(int index)
    {
        return GetData(index);
    }
}
