#include <limits>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

#include "lua.hpp"

#include "json/document-wrapper.h"
#include "json/writer.h"

// using namespace rapidjson;

void basic_conversion_lua_push_json_value(lua_State* L, const rapidjson::Value& v) {
    if (v.IsObject()) {
        lua_newtable(L);
        for (auto it = v.MemberBegin(); it != v.MemberEnd(); ++it)
        {
            const char* s = it->name.GetString();
            lua_pushstring(L, s);
            basic_conversion_lua_push_json_value(L, it->value);
            lua_settable(L, -3);
        }
    }
    else if (v.IsArray()) {
        lua_newtable(L);
        auto arr = v.GetArray();
        int i = 0;
        for (auto it = arr.Begin(); it != arr.End(); ++it) {
            lua_pushnumber(L, ++i);
            basic_conversion_lua_push_json_value(L, *it);
            lua_settable(L, -3);
        }
    }
    else if (v.IsNumber()) {
        v.IsInt() ? lua_pushinteger(L, v.GetInt()) : lua_pushnumber(L, v.GetDouble());
    }
    else if (v.IsString()) {
        lua_pushstring(L, v.GetString());
    }
    else if (v.IsBool()) {
        lua_pushboolean(L, v.GetBool());
    }
    else {
        lua_pushnil(L);
    }
}

int basic_conversion_lua_push_json_str(lua_State * L, const char * str, size_t len) {
    rapidjson::Document bodyJsonReader;

    if (len == 0) {
        len = strlen(str);
    }
    size_t size = len + 1;

    char* s = new char[size];
    s[size - 1] = '\0';
    memcpy(s, str, size);
    int ret = 0;
    if (bodyJsonReader.ParseInsitu<0>(s).HasParseError()) {
        lua_pushnil(L);
        lua_pushlstring(L, str, len);
        ret = 2;
    }
    else {
        basic_conversion_lua_push_json_value(L, bodyJsonReader);
        ret = 1;
    }
    delete s;
    return ret;
}

static int _getTableCount(lua_State* L, int t) {
    if (!lua_istable(L, t)) {
        return -1;
    }

    int top = lua_gettop(L);
    int ret = 0;
    if (t < 0) {
        t = top + t + 1;
    }
    lua_pushnil(L);
    while (lua_next(L, t) != 0) {
        ++ret;
        lua_pop(L, 1);
    }
    lua_settop(L, top);
    return ret;
}

static rapidjson::Value _getJsonValue(lua_State* L, rapidjson::Document& doc);
static void _setJsonKeyValue(rapidjson::Value& v, const char* key, rapidjson::Document& doc, lua_State* L) {
    rapidjson::Value k;
    k.SetString(key, doc.GetAllocator());
    v.AddMember(k, _getJsonValue(L, doc), doc.GetAllocator());
}

// pop top stack value and return its json value
static rapidjson::Value _getJsonValue(lua_State* L, rapidjson::Document& doc) {
    using rapidjson::Value;
    using rapidjson::Type;
    Value ret;
    int type = lua_type(L, -1);
    if (type == LUA_TTABLE) {
        int tCnt = _getTableCount(L, -1);
        if (lua_rawlen(L, -1) == tCnt) {
            ret.SetArray();
            for (int i = 1; i <= tCnt; ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                ret.PushBack(_getJsonValue(L, doc), doc.GetAllocator());
            }
        }
        else {
            ret.SetObject();
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (lua_isstring(L, -2)) {
                    const char* key = lua_tostring(L, -2);
                    _setJsonKeyValue(ret, key, doc, L);
                }
                else {
                    lua_pop(L, 1);
                }
            }
        }
    }
    else if (type == LUA_TBOOLEAN) {
        ret.SetBool(lua_toboolean(L, -1));
    }
    else if (type == LUA_TNUMBER) {
        lua_Number n = lua_tonumber(L, -1);
        if (n == int(n)) {
            ret.SetInt((int)n);
        }
        else if (n == (lua_Number)int64_t(n)) {
            ret.SetInt64((int64_t)n);
        }
        else {
            ret.SetDouble(n);
        }
    }
    else if (type == LUA_TSTRING) {
        const char* s = lua_tostring(L, -1);
        ret.SetString(s, strlen(s));
    }
    else {
        ret.SetNull();
    }

    lua_pop(L, 1);
    return ret;
}

int basic_conversion_lua_push_json_obj(lua_State * L, int tableIdx) {
    using rapidjson::Document;
    using rapidjson::Value;
    using rapidjson::Type;
    using rapidjson::StringBuffer;
    using rapidjson::Writer;

    Document doc;
    int type = lua_type(L, tableIdx);
    if (type == LUA_TTABLE) {
        int tCnt = _getTableCount(L, tableIdx);
        if (lua_rawlen(L, tableIdx) == tCnt) {
            doc.SetArray();
            for (int i = 1; i <= tCnt; ++i) {
                lua_pushinteger(L, i);
                lua_gettable(L, tableIdx);
                doc.PushBack(_getJsonValue(L, doc), doc.GetAllocator());
            }
        }
        else {
            doc.SetObject();
            lua_pushnil(L);
            while (lua_next(L, tableIdx) != 0) {
                if (lua_isstring(L, -2)) {
                    const char* key = lua_tostring(L, -2);
                    _setJsonKeyValue(doc, key, doc, L);
                }
                else {
                    lua_pop(L, 1);
                }
            }
        }
    }
    else if (type == LUA_TBOOLEAN) {
        doc.SetBool(lua_toboolean(L, tableIdx));
    }
    else if (type == LUA_TNUMBER) {
        lua_Number n = lua_tonumber(L, tableIdx);
        if (n == int(n)) {
            doc.SetInt((int)n);
        }
        else if (n == (lua_Number)int64_t(n)) {
            doc.SetInt64((int64_t)n);
        }
        else {
            doc.SetDouble(n);
        }
    }
    else if (type == LUA_TSTRING) {
        const char* s = lua_tostring(L, tableIdx);
        doc.SetString(s, strlen(s));
    }
    else {
        doc.SetNull();
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer); // in rapidjson/writer.h
    doc.Accept(writer);
    const char* ss = buffer.GetString();
    lua_pushstring(L, ss);
    return 1;
}


static int _encode(lua_State* L) {
	basic_conversion_lua_push_json_obj(L, 1);
	return 1;
}

static int _decode(lua_State* L) {
	size_t len = 0;
	const char *jsonStr = luaL_checklstring(L, 1, &len);
	basic_conversion_lua_push_json_str(L, jsonStr, len);
	return 1;
}

extern "C" {

LUALIB_API int luaopen_rapidjson(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, _encode);
	lua_setfield(L, -2, "encode");
	lua_pushcfunction(L, _decode);
	lua_setfield(L, -2, "decode");

	return 1;
}

}
