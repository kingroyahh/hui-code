using UnityEngine;
using System.Collections;

public class Actor : MonoBehaviour {

    public enum ActorState
    {
        BORN,
        SKILL,
        HURT,
        DIE,
        CONTROL
    }
    private ActorState m_state = ActorState.BORN;
    private SkillManager mSkillManager = null;


	void Start () 
    {
	
	}
	
	void Update () 
    {
        switch (m_state)
        {
            case ActorState.SKILL:
                SkillUpdate();
                break;
            default:
                break;
        }	
	}

    private void SkillUpdate()
    {
        if (mSkillManager)
        {
            mSkillManager.SkillUpdate();
        }
    }
}
