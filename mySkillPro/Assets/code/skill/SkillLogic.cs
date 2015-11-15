using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class SkillLogic : MonoBehaviour {

    public enum STATE
    {
        ENABLE,
        DISALBE,
        EXECUTE,
        PASSIVE,
        OVER,
    }

    private STATE mState = STATE.ENABLE;

    public List<Skill> mSkillData = new List<Skill>();


	void Start () {
	
	}
	
	void Update () {
	
	}

    public void SkillUpdate()
    {
        switch (mState)
        {
            case STATE.ENABLE:
                break;
            case STATE.DISALBE:
                break;
            case STATE.EXECUTE:
                break;
            case STATE.PASSIVE:
                break;
            default:
                break;
        }
    }
}
