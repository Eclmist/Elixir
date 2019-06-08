/*
    This file is part of Elixir, an open - source cross platform physically
    based renderer.

    Copyright(c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#pragma once

#include "paramset.h"

exrBEGIN_NAMESPACE

template <class T>
ParamSet::ParamSetItem<T>::ParamSetItem(const exrString& key, const T* value, exrU32 numValues /*= 1*/)
    : m_Key(key)
    , m_Values(std::make_unique<T[]>(numValues))
    , m_NumValues(numValues)
{
    std::copy(value, value + numValues, m_Values.get());
}

void ParamSet::AddBool(const exrString& key, const exrBool* val, exrU32 numVals)
{
    RemoveBool(key);
    m_Booleans.emplace_back(std::make_unique<ParamSetItem<exrBool>>(key, std::move(val), numVals));
}

void ParamSet::AddU32(const exrString& key, const exrU32* val, exrU32 numVals)
{
    RemoveU32(key);
    m_U32s.emplace_back(std::make_unique<ParamSetItem<exrU32>>(key, std::move(val), numVals));
}

void ParamSet::AddS32(const exrString& key, const exrS32* val, exrU32 numVals)
{
    RemoveS32(key);
    m_S32s.emplace_back(std::make_unique<ParamSetItem<exrS32>>(key, std::move(val), numVals));
}

void ParamSet::AddFloat(const exrString& key, const exrFloat* val, exrU32 numVals)
{
    RemoveFloat(key);
    m_Floats.emplace_back(std::make_unique<ParamSetItem<exrFloat>>(key, std::move(val), numVals));
}

void ParamSet::AddString(const exrString& key, const exrString* val, exrU32 numVals)
{
    RemoveString(key);
    m_Strings.emplace_back(std::make_unique<ParamSetItem<exrString>>(key, std::move(val), numVals));
}

void ParamSet::AddPoint2(const exrString& key, const exrPoint2* val, exrU32 numVals)
{
    RemovePoint2(key);
    m_Point2s.emplace_back(std::make_unique<ParamSetItem<exrPoint2>>(key, std::move(val), numVals));
}

void ParamSet::AddPoint3(const exrString& key, const exrPoint3* val, exrU32 numVals)
{
    RemovePoint3(key);
    m_Point3s.emplace_back(std::make_unique<ParamSetItem<exrPoint3>>(key, std::move(val), numVals));
}

void ParamSet::AddVector2(const exrString& key, const exrVector2* val, exrU32 numVals)
{
    RemoveVector2(key);
    m_Vector2s.emplace_back(std::make_unique<ParamSetItem<exrVector2>>(key, std::move(val), numVals));
}

void ParamSet::AddVector3(const exrString& key, const exrVector3* val, exrU32 numVals)
{
    RemoveVector3(key);
    m_Vector3s.emplace_back(std::make_unique<ParamSetItem<exrVector3>>(key, std::move(val), numVals));
}

#define REMOVE_PARAM(item, key)                     \
    for (exrU32 i = 0; i < item.size(); i++)        \
    {                                               \
        if (item[i]->m_Key == key)                  \
        {                                           \
            item.erase(item.begin() + i);           \
            return true;                            \
        }                                           \
    }                                               \
    return false;

exrBool ParamSet::RemoveBool(const exrString& key)
{
    REMOVE_PARAM(m_Booleans, key)
}

exrBool ParamSet::RemoveU32(const exrString& key)
{
    REMOVE_PARAM(m_U32s, key)
}

exrBool ParamSet::RemoveS32(const exrString& key)
{
    REMOVE_PARAM(m_S32s, key)
}

exrBool ParamSet::RemoveFloat(const exrString& key)
{
    REMOVE_PARAM(m_Floats, key)
}

exrBool ParamSet::RemoveString(const exrString& key)
{
    REMOVE_PARAM(m_Strings, key)
}

exrBool ParamSet::RemovePoint2(const exrString& key)
{
    REMOVE_PARAM(m_Point2s, key)
}

exrBool ParamSet::RemovePoint3(const exrString& key)
{
    REMOVE_PARAM(m_Point3s, key)
}

exrBool ParamSet::RemoveVector2(const exrString& key)
{
    REMOVE_PARAM(m_Vector2s, key)
}

exrBool ParamSet::RemoveVector3(const exrString& key)
{
    REMOVE_PARAM(m_Vector3s, key)
}

#undef REMOVE_PARAM

#define FIND_FIRST_ITEM(T, item, key, d)            \
    for (exrU32 i = 0; i < item.size(); i++)        \
    {                                               \
        if (item[i]->m_Key == key)                  \
        {                                           \
            item[i]->m_LookedUp = true;             \
            return item[i]->m_Values[0];            \
        }                                           \
    }                                               \
    return d;


exrBool ParamSet::FindFirstBool(const exrString& key, exrBool d) const
{
    FIND_FIRST_ITEM(exrBool, m_Booleans, key, d)
}

exrU32 ParamSet::FindFirstU32(const exrString& key, exrU32 d) const
{
    FIND_FIRST_ITEM(exrU32, m_U32s, key, d)
}

exrS32 ParamSet::FindFirstS32(const exrString& key, exrS32 d) const
{
    FIND_FIRST_ITEM(exrS32, m_S32s, key, d)
}

exrFloat ParamSet::FindFirstFloat(const exrString& key, exrFloat d) const
{
    FIND_FIRST_ITEM(exrFloat, m_Floats, key, d)
}

exrString ParamSet::FindFirstString(const exrString& key, exrString d) const
{
    FIND_FIRST_ITEM(exrString, m_Strings, key, d)
}

exrPoint2 ParamSet::FindFirstPoint2(const exrString& key, exrPoint2 d) const
{
    FIND_FIRST_ITEM(exrPoint2, m_Point2s, key, d)
}

exrPoint3 ParamSet::FindFirstPoint3(const exrString& key, exrPoint3 d) const
{
    FIND_FIRST_ITEM(exrPoint3, m_Point3s, key, d)
}

exrVector2 ParamSet::FindFirstVector2(const exrString& key, exrVector2 d) const
{
    FIND_FIRST_ITEM(exrVector2, m_Vector2s, key, d)
}

exrVector3 ParamSet::FindFirstVector3(const exrString& key, exrVector3 d) const
{
    FIND_FIRST_ITEM(exrVector3, m_Vector3s, key, d)
}

#undef FIND_FIRST_ITEM

#define FIND_ITEM(T, item, key, num)                \
    for (exrU32 i = 0; i < item.size(); i++)        \
    {                                               \
        if (item[i]->m_Key == key)                  \
        {                                           \
            item[i]->m_LookedUp = true;             \
            *num = item[i]->m_NumValues;            \
            return item[i]->m_Values.get();         \
        }                                           \
    }                                               \
    return nullptr;

exrBool* ParamSet::FindBool(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrBool, m_Booleans, key, num)
}

exrU32* ParamSet::FindU32(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrU32, m_U32s, key, num)
}

exrS32* ParamSet::FindS32(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrS32, m_S32s, key, num)
}

exrFloat* ParamSet::FindFloat(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrFloat, m_Floats, key, num)
}

exrString* ParamSet::FindString(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrString, m_Strings, key, num)
}

exrPoint2* ParamSet::FindPoint2(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrPoint2, m_Point2s, key, num)
}

exrPoint3* ParamSet::FindPoint3(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrPoint3, m_Point3s, key, num)
}

exrVector2* ParamSet::FindVector2(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrVector2, m_Vector2s, key, num)
}

exrVector3* ParamSet::FindVector3(const exrString& key, exrU32* num) const
{
    FIND_ITEM(exrVector3, m_Vector3s, key, num)
}

#undef FIND_ITEM

void ParamSet::ReportUnused() const
{
#define CHECK_UNUSED(v)                            \
    for (exrU32 i = 0; i < (v).size(); ++i)        \
        if (!(v)[i]->m_LookedUp)                   \
            exrWarningLine("Parameter "<< (v)[i]->m_Key.c_str() << " not used");

    CHECK_UNUSED(m_Booleans);
    CHECK_UNUSED(m_U32s);
    CHECK_UNUSED(m_S32s);
    CHECK_UNUSED(m_Floats);
    CHECK_UNUSED(m_Strings);
    CHECK_UNUSED(m_Point2s);
    CHECK_UNUSED(m_Point3s);
    CHECK_UNUSED(m_Vector2s);
    CHECK_UNUSED(m_Vector3s);

#undef CHECK_UNUSED
}

void ParamSet::Clear()
{
#define DEL_PARAMS(key) (key).erase((key).begin(), (key).end())

    DEL_PARAMS(m_Booleans);
    DEL_PARAMS(m_U32s);
    DEL_PARAMS(m_S32s);
    DEL_PARAMS(m_Floats);
    DEL_PARAMS(m_Strings);
    DEL_PARAMS(m_Point2s);
    DEL_PARAMS(m_Point3s);
    DEL_PARAMS(m_Vector2s);
    DEL_PARAMS(m_Vector3s);

#undef DEL_PARAMS
}

exrEND_NAMESPACE
