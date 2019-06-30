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

#include "core/elixir.h"

exrBEGIN_NAMESPACE

class ParamSet
{
public:
    template <class T>
    struct ParamSetItem {
        //! Constructor
        ParamSetItem(const exrString& key, const T* value, exrU32 numValues = 1);

        //! Unique name of the parameter item
        const exrString m_Key;

        //! Values of the parameter item
        const std::unique_ptr<T[]> m_Values;

        //! Number of values in this parameter item (arrays)
        const exrU32 m_NumValues;

        //! An internal flag to check if value have been used. Can be used for error checking.
        mutable exrBool m_LookedUp = false;
    };

public:
    void AddBool                    (const exrString& key, const exrBool*,    exrU32 numVal);
    void AddU32                     (const exrString& key, const exrU32*,     exrU32 numVal);
    void AddS32                     (const exrString& key, const exrS32*,     exrU32 numVal);
    void AddexrFloat                   (const exrString& key, const exrFloat*,   exrU32 numVal);
    void AddString                  (const exrString& key, const exrString*,  exrU32 numVal);
    void AddPoint2                  (const exrString& key, const exrPoint2*,  exrU32 numVal);
    void AddPoint3                  (const exrString& key, const exrPoint3*,  exrU32 numVal);
    void AddVector2                 (const exrString& key, const exrVector2*, exrU32 numVal);
    void AddVector3                 (const exrString& key, const exrVector3*, exrU32 numVal);

    exrBool RemoveBool              (const exrString& key);
    exrBool RemoveU32               (const exrString& key);
    exrBool RemoveS32               (const exrString& key);
    exrBool RemoveexrFloat             (const exrString& key);
    exrBool RemoveString            (const exrString& key);
    exrBool RemovePoint2            (const exrString& key);
    exrBool RemovePoint3            (const exrString& key);
    exrBool RemoveVector2           (const exrString& key);
    exrBool RemoveVector3           (const exrString& key);

public:
    exrBool     FindFirstBool       (const exrString& key, exrBool     d) const;
    exrU32      FindFirstU32        (const exrString& key, exrU32      d) const;
    exrS32      FindFirstS32        (const exrString& key, exrS32      d) const;
    exrFloat    FindFirstFloat      (const exrString& key, exrFloat    d) const;
    exrString   FindFirstString     (const exrString& key, exrString   d) const;
    exrPoint2   FindFirstPoint2     (const exrString& key, exrPoint2   d) const;
    exrPoint3   FindFirstPoint3     (const exrString& key, exrPoint3   d) const;
    exrVector2  FindFirstVector2    (const exrString& key, exrVector2  d) const;
    exrVector3  FindFirstVector3    (const exrString& key, exrVector3  d) const;

    exrBool*    FindBool            (const exrString& key, exrU32*   num) const;
    exrU32*     FindU32             (const exrString& key, exrU32*   num) const;
    exrS32*     FindS32             (const exrString& key, exrU32*   num) const;
    exrFloat*   FindFloat           (const exrString& key, exrU32*   num) const;
    exrString*  FindString          (const exrString& key, exrU32*   num) const;
    exrPoint2*  FindPoint2          (const exrString& key, exrU32*   num) const;
    exrPoint3*  FindPoint3          (const exrString& key, exrU32*   num) const;
    exrVector2* FindVector2         (const exrString& key, exrU32*   num) const;
    exrVector3* FindVector3         (const exrString& key, exrU32*   num) const;

public:
    void ReportUnused() const;
    void Clear();

private:
    std::vector<std::unique_ptr<ParamSetItem<exrBool>>>     m_Booleans;
    std::vector<std::unique_ptr<ParamSetItem<exrU32>>>      m_U32s;
    std::vector<std::unique_ptr<ParamSetItem<exrS32>>>      m_S32s;
    std::vector<std::unique_ptr<ParamSetItem<exrFloat>>>    m_exrFloats;
    std::vector<std::unique_ptr<ParamSetItem<exrString>>>   m_Strings;
    std::vector<std::unique_ptr<ParamSetItem<exrPoint2>>>   m_Point2s;
    std::vector<std::unique_ptr<ParamSetItem<exrPoint3>>>   m_Point3s;
    std::vector<std::unique_ptr<ParamSetItem<exrVector2>>>  m_Vector2s;
    std::vector<std::unique_ptr<ParamSetItem<exrVector3>>>  m_Vector3s;
};

exrEND_NAMESPACE