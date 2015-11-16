using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;

public class MetadataLoader<T> : MonoBehaviour 
{
    public string Path;

    private static MetadataLoader<T> instance;
    public static MetadataLoader<T> Instance
    {
        get
        {
            return instance;
        }
    }
    private List<T> dataList = new List<T>();
    private Dictionary<string, T> dataDic = new Dictionary<string, T>();

    private void Start()
    {
        instance = this;
        var data = Resources.Load<TextAsset>(Path);
        string textString = data.text;
        Debug.Log(textString);
        T dataT = JsonConvert.DeserializeObject<T>(textString);
        dataList.Add(dataT);
    }

    public T GetData(int key)
    {
        return dataList[key];
    }

}
