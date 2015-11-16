using UnityEngine;
using System.Collections;


public class MetadataConfig : MonoBehaviour
{
    public string Path = "Metadata/arpg_skill";
    public MetadatSkill mSkill;

    private void Awake()
    {
        gameObject.AddComponent<MetadatSkill>();
    }

    public void Click()
    {
        Debug.Log(gameObject.GetComponent<MetadatSkill>().GetData(0).Id); 
    }
}
