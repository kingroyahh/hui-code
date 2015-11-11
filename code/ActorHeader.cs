using UnityEngine;
using System.Collections;
using UnityEngine.UI;

using DG.Tweening;
using Adlib.Utils;

using Babeltime.Battle.Shared;
using Babeltime.Battle.Framework;

namespace Babeltime.Battle.Client
{
    public class ActorHeader : LogMonoBehaviour
    {
        public Actor Target;
        public int MaxHp;
        public int Hp;

        private Transform sliderTransHp = null;
        private Slider sliderCtrlHp = null;
        private Transform sliderTransMp = null;
        private Slider sliderCtrlMp = null;
        private Text textName = null;
        private Text textBlood = null;
        private bool isPlayer = false;
        private Vector3 offsetOffine = new Vector3(0, 2.3f, 0);
        private Vector3 offsetOnLine = new Vector3(0, 2.3f, 0);
        private Transform layer_pos;

        public ActorHeader()
        {
        }

        private void Awake()
        {
            layer_pos = gameObject.transform.FindChild("layer_pos");

            sliderTransHp = layer_pos.FindChild("slider_hp").transform;
            sliderCtrlHp = sliderTransHp.GetComponent<Slider>();

            sliderTransMp = layer_pos.FindChild("slider_mp").transform;
            sliderCtrlMp = sliderTransMp.GetComponent<Slider>();

            textName = layer_pos.FindChild("name").GetComponent<Text>();
            textBlood = layer_pos.FindChild("blood").GetComponent<Text>();
            textBlood.gameObject.SetActive(false);
        }

        // Use this for initialization
        private void Start()
        {
            sliderCtrlHp.value = 1.0f;
            sliderCtrlMp.value = 1.0f;
        }

        // Update is called once per frame
        private void Update()
        {
            if (Target != null)
            {
                Vector3 pos;
                if (Target.IsOfflineMode)
                {
                    pos = Target.transform.position + offsetOffine;
                }
                else
                {
                    pos = Target.transform.position + offsetOnLine;
                }


                Vector2 position = Camera.main.WorldToScreenPoint(pos);

                if (Target.networkView.isMine || Target.IsOfflineMode)
                {
                    layer_pos.DOMove(new Vector3(position.x, position.y, -Camera.main.fieldOfView * 10f), 0.1f, true);//位置
                }
                else
                {
                    layer_pos.transform.position = position;
                }


                if (Target.GetComponent<ActorAttribute>().Hp != Hp)
                {
                    ChangeHp(Target.GetComponent<ActorAttribute>().Hp, Hp - Target.GetComponent<ActorAttribute>().Hp);
                    Hp = Target.GetComponent<ActorAttribute>().Hp;
                }
            }
        }


        public void SetName(string name)
        {
            textName.text = name;
        }

        //change hp
        private void ChangeHp(int now, int deltaHp)
        {
            sliderCtrlHp.value = 1.0f * now / MaxHp;
            BloodMove(deltaHp);
        }

        //blood move
        private void BloodMove(int lose)
        {
            textBlood.gameObject.SetActive(true);
            textBlood.text = "-" + lose;
            Tweener tweener = textBlood.rectTransform.DOLocalMoveY(200f, 1f, false);
            tweener.SetUpdate(true);
            //设置移动类型
            tweener.SetEase(Ease.Linear);
            tweener.onComplete = delegate()
            {
                //Debug.Log("移动完毕事件");
                textBlood.rectTransform.localPosition = new Vector3(0f, 70f, 0f);
                textBlood.gameObject.SetActive(false);
            };
        }

        //change mp
        private void ChangeMp(int now, int max)
        {
            sliderCtrlMp.value = 1.0f * now / max;
        }
    }
}
