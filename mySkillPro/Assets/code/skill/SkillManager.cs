using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class SkillManager : MonoBehaviour 
{

    public Actor mActor = null;

    public List<SkillLogic> mSkillLogicList = null;

    public SkillLogic mCurActiveSkillLogic = null;

	void Start () 
    {
        if (mActor == null)
        {
            Debug.LogError("no actor!!!");
        }
        mCurActiveSkillLogic = null;
	}
	
	void Update () 
    {
	
	}

    public void SkillUpdate()
    {
        if (mCurActiveSkillLogic != null)
        {
            mCurActiveSkillLogic.SkillUpdate();
        }
    }
}
