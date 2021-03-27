/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cstring.h"

CString::CString()
{
    ;
}

CString::~CString()
{
    ;
}

int CString::GetStringSize()
{
    return this->m_string.size();
}

QString CString::GetString()
{
    return this->m_string;
}

QList<int> CString::CheckCString(const int beginIndex, const int endIndex, const CStringModel model)
{
    QList<int> listAbortedResult;
    if(beginIndex < 0 || endIndex < 0 || beginIndex > this->m_string.size() || endIndex > this->m_string.size() || beginIndex > endIndex)
    {
        listAbortedResult.append(-Error_IndexOverRange);
        return listAbortedResult;
    }
    else
    {
        switch (model)
        {
        default:
        {
            listAbortedResult.append(-Error_ModelOverRange);
            return listAbortedResult;
        }
        case Model_Account:
        {
            if(beginIndex == endIndex)
            {
                if(CheckChar(beginIndex, Model_Account) == Error_None)
                {
                    return listAbortedResult;
                }
                else
                {
                    listAbortedResult.append(beginIndex);
                    return listAbortedResult;
                }
            }
            else
            {
                for(int i = beginIndex; i <= endIndex; ++i)
                {
                    if(CheckChar(beginIndex, Model_Account) != Error_None)
                    {
                        listAbortedResult.append(beginIndex);
                    }
                }
                return listAbortedResult;
            }
            break;
        }
        case Model_Password:
        {
            if(beginIndex == endIndex)
            {
                if(CheckChar(beginIndex, Model_Password) == Error_None)
                {
                    return listAbortedResult;
                }
                else
                {
                    listAbortedResult.append(beginIndex);
                    return listAbortedResult;
                }
            }
            else
            {
                for(int i = beginIndex; i <= endIndex; ++i)
                {
                    if(CheckChar(beginIndex, Model_Password) != Error_None)
                    {
                        listAbortedResult.append(beginIndex);
                    }
                }
                return listAbortedResult;
            }
            break;
        }
        }
    }
}

CString::CStringError CString::IfSet()
{
    if(this->m_string == "")
    {
        return Error_EmptyString;
    }
    else
    {
        return Error_None;
    }
}

CString::CStringError CString::DeleteChar(const int index)
{
    QString temporary = this->m_string;
    this->m_string = "";
    for(int i = 0; i < temporary.size(); ++i)
    {
        if(i != index)
        {
            this->m_string += temporary[i];
        }
    }
    if(this->m_string.size() == temporary.size() - 1)
    {
        return Error_None;
    }
    else
    {
        return Error_AbortedDelete;
    }
}

CString::CStringError CString::SetCString(const QString string, bool change)
{
    if(string.size() == 0)
    {
        return Error_EmptyString;
    }
    else if(this->m_string != "")
    {
        if(change)
        {
            this->m_string = string;
            return Error_None;
        }
        else
        {
            return Error_AbortedSet;
        }
    }
    else
    {
        this->m_string = string;
        return Error_None;
    }
}

CString::CStringError CString::CheckChar(const int index, const CStringModel model)
{
    if(index < 0 || index > this->m_string.size() - 1)
    {
        return Error_IndexOverRange;
    }
    QChar checkedChar = this->m_string[index];
    switch (model)
    {
    default:
    {
        return Error_ModelOverRange;
    }
    case Model_Account:
    {
        if(((47 < checkedChar) && (checkedChar < 58)) || ((64 < checkedChar) && (checkedChar < 91)) || ((96 < checkedChar) && (checkedChar < 123)))
        {
            return Error_None;
        }
        else
        {
            return Error_CharOverRange;
        }
        break;
    }
    case Model_Password:
    {
        if(((32 < checkedChar) && (checkedChar < 91)) || ((96 < checkedChar) && (checkedChar < 123)))
        {
            return Error_None;
        }
        else
        {
            return Error_CharOverRange;
        }
        break;
    }
    }
}
