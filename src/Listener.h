//
//  Listener.h
//  grav_test
//
//  Created by kingroy on 16/10/17.
//  Copyright © 2016年 Babeltime. All rights reserved.
//

#ifndef _GRAV_LISTENER_H_
#define _GRAV_LISTENER_H_


#include <iostream>
#include <set>
#include <algorithm>
#include "EventData.h"
#include "ModifierMeta.h"

class IListener
{
public:
    virtual ~IListener()
    {

    }

    int regId;
    ModifierMeta *modMeta;
    virtual void HandleNotification(EventData* lData)
    {
    }
};


class Notifier
{
protected:
    virtual void Notify(EventData *data)
    {
//        std::for_each(m_listeners.begin(), m_listeners.end(), [&](IListener *l)
//        {
//            l->HandleNotification(data);
//        });
        
        std::set<IListener *>::const_iterator iter = m_listeners.begin();
        
        while (iter != m_listeners.end())
        {
            if ((*iter) != NULL_PTR)
            {
                (*iter)->HandleNotification(data);
            }
            iter++;
        }
    }
    
public:
    
    Notifier()
    {
        m_listeners.clear();
    }
    virtual ~Notifier()
    {
    }
    
    virtual void RegisterListener(IListener *l, int id, ModifierMeta *modMeta)
    {
        l->regId = id;
        l->modMeta = modMeta;
        m_listeners.insert(l);
    }
    
    virtual void UnregisterListener(IListener *l)
    {
        std::set<IListener *>::const_iterator iter = m_listeners.find(l);
        if (iter != m_listeners.end())
        {
            m_listeners.erase(iter);
        }
        else
        {
            // handle the case
            std::cout << "Could not unregister the specified listener object as it is not registered." << std::endl;
        }
    }
    
    virtual void ClearAll()
    {
        

        m_listeners.clear();
    }
    
private:
    std::set<IListener*> m_listeners;
};

#endif
