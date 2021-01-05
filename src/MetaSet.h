#ifndef _GRAV_META_SET_H_
#define _GRAV_META_SET_H_

#include "Define.h"
#include "Log.h"
#include "Meta.h"

#include <map>
#include <string>

#include "rapidjson/document.h"

#ifdef GRAV_UNITY_PLUGIN
#include "sqlite3.h"
#else
#include <sys/stat.h>
#include "rapidjson/filereadstream.h"
#endif
using namespace std;
using namespace rapidjson;

template<typename T>
class BaseMetaSet
{
public:
    map<int, T*> set;

protected:
    bool inited;
    bool valid;
    bool loadAll;
    string fpath;

    string dbfname;
    string dbtname;

    BaseMetaSet()
    {
        valid = false;
        inited = false;
        loadAll = false;
    };

    virtual ~BaseMetaSet()
    {
        if (!inited)
        {
            return;
        }

        for (typename map<int, T*>::iterator it = set.begin(); it != set.end(); ++it)
        {
            delete it->second;
        }
    };

    virtual void AssignPath() = 0;
    virtual T* ParseJObject(Value::Object &) = 0;

public:
    void Config(const char *db_fname, const char *db_tname)
    {
        dbfname = string(db_fname);
        dbtname = string(db_tname);
    }

    void Init()
    {
        if (inited)
        {
            debug("MetaSet already inited!");
            return;
        }
        valid = true;
        inited = true;
        AssignPath();
#ifdef GRAV_UNITY_PLUGIN
        return;
#else
        info("Load Meta Json [%s]",fpath.c_str());
        struct stat stat_buf;
        int rc = stat(fpath.c_str(), &stat_buf);
        int fsize = (int)(rc == 0 ? stat_buf.st_size : -1);
        if (fsize < 0)
        {
            valid = false;
            return;
        }
        FILE* fp = fopen((const char*)fpath.c_str(), "rb");
        char* buffer = new char[fsize];
        FileReadStream stream(fp, buffer, fsize);

        Document doc;
        doc.ParseStream(stream);

        if (!doc.IsArray())
        {
            error("Json Document is not in array format! %s\n", fpath.c_str());
            valid = false;
        }

        for (int i = 0; i < doc.Size(); ++i)
        {
            Value::Object obj = doc[i].GetObject();
            T* meta = ParseJObject(obj);
            if (meta != NULL_PTR)
            {
                set.insert(pair<int, T*>(meta->id, meta));
            }
        }
        fclose(fp);
        delete[] buffer;
#endif
    }

    void Close()
    {
        if (!inited)
        {
            debug("MetaSet not inited!");
            return;
        }
        for (typename map<int, T*>::iterator it = set.begin(); it != set.end(); ++it)
        {
            delete it->second;
        }

        set.clear();

        valid = false;
        inited = false;
        loadAll = false;
    }

    const T* Get(int id)
    {
        if (!inited)
        {
            Init();
        }
        if (!valid)
        {
            debug("not valid Get %s, %d\n", fpath.c_str(), id);
            return NULL_PTR;
        }
        typename map<int, T*>::iterator it = set.find(id);

        if (it != set.end())
        {
            return (const T*)it->second;
        }
#ifdef GRAV_UNITY_PLUGIN
        debug("Sqlite Get %s, %d\n", fpath.c_str(), id);
        
        sqlite3* db;
        int rc = sqlite3_open_v2(fpath.c_str(), &db, SQLITE_OPEN_READWRITE, 0);
        if (rc != SQLITE_OK)
        {
            error("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return NULL_PTR;
        }

        sqlite3_stmt* prepareStmt;
        string sql("select Json from ");
        sql.append(dbtname);
        sql.append(" where Id = @id");
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &prepareStmt, 0);
        if (rc != SQLITE_OK)
        {
            error("Failed to execute statement: %s , %s\n", fpath.c_str(), sqlite3_errmsg(db));
            return NULL_PTR;
        }
        int idx = sqlite3_bind_parameter_index(prepareStmt, "@id");
        sqlite3_bind_int(prepareStmt, idx, id);
        int step = sqlite3_step(prepareStmt);
        if (step == SQLITE_ROW)
        {
            const char *json = (const char *)sqlite3_column_text(prepareStmt, 0);
            Document doc;
            doc.Parse(json);
            Value::Object jobj = doc.GetObject();
            T* meta = ParseJObject(jobj);
            sqlite3_finalize(prepareStmt);
            sqlite3_close(db);
            if (meta != NULL_PTR)
            {
                set.insert(pair<int, T*>(meta->id, meta));
                return (const T*)meta;
            }
            error("parseJObject is null ID:[%d] %s", id, dbtname.c_str());
        }
        else
        {
            sqlite3_finalize(prepareStmt);
            sqlite3_close(db);
            error("sqlite unknown error! ID:[%d] %s", id, dbtname.c_str());
        }
#endif
        return NULL_PTR;
    };

    void LoadAll()
    {
        if (!inited)
        {
            Init();
        }
        if (!valid)
        {
            error("LoadAll, not valid, due to can't open db file %s\n", dbfname.c_str());
            return;
        }
        if (loadAll)
        {
            return;
        }
        loadAll = true;

#ifdef GRAV_UNITY_PLUGIN

        sqlite3* db;
        int rc = sqlite3_open_v2(fpath.c_str(), &db, SQLITE_OPEN_READWRITE, 0);
        if (rc != SQLITE_OK)
        {
            error("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        
        sqlite3_stmt* resall;
        string sql("select Json from ");
        sql.append(dbtname);
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &resall, 0);
        if (rc != SQLITE_OK)
        {
            error("Failed to execute statement: %s , %s\n", fpath.c_str(), sqlite3_errmsg(db));
            return;
        }
        while (sqlite3_step(resall) == SQLITE_ROW)
        {
            const char *json = (const char *)sqlite3_column_text(resall, 0);
            if (json[0] != '{')
            {
                continue;
            }
            Document doc;
            doc.Parse(json);
            Value::Object jobj = doc.GetObject();
            T* meta = ParseJObject(jobj);
            if (meta != NULL_PTR)
            {
                set.insert(pair<int, T*>(meta->id, meta));
            }
            else
            {
                error("Meta is null : %s\n", json);
            }
        }
        sqlite3_finalize(resall);
        sqlite3_close(db);
#endif
    };

    typename map<int, T*>::iterator Begin()
    {
        return set.begin();
    };

    typename map<int, T*>::iterator End()
    {
        return set.end();
    };
};

#endif
