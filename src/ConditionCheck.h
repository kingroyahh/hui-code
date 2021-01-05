//
//  ConditionCheck.h
//  grav_test
//
//  Created by kingroy on 16/10/18.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#ifndef _GRAV_CONDITION_CHECK_H_
#define _GRAV_CONDITION_CHECK_H_
#include "Type.h"

bool CheckMoreHp(ActorPtr actor, int param);
bool CheckLessHp(ActorPtr actor, int param);
bool CheckMoreXp(ActorPtr actor, int param);
bool CheckLessXp(ActorPtr actor, int param);
bool CheckHaveSpecialBuff(ActorPtr actor, int param);


#endif
