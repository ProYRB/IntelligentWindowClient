/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
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

bool CString::isEmpty()
{
    if(this->m_qstring == "")
    {
        return true;
    }
    return false;
}

qint16 CString::getSize()
{
    return this->m_qstring.size();
}

QString CString::getString()
{
    return this->m_qstring;
}

CString::CStringError CString::check(const int index)
{
    if(index < 0 || index > this->m_qstring.size() - 1)
    {
        return Error_NullIndex;
    }
    QChar targetChar = this->m_qstring[index];
    switch (m_checkModel)
    {
    default:
    {
        return Error_NullModel;
    }
    case Model_Account:
    {
        if(((47 < targetChar) && (targetChar < 58)) || ((64 < targetChar) && (targetChar < 91)) || ((96 < targetChar) && (targetChar < 123)))
        {
            return Error_None;
        }
        break;
    }
    case Model_Password:
    {
        if(((32 < targetChar) && (targetChar < 91)) || ((96 < targetChar) && (targetChar < 123)))
        {
            return Error_None;
        }
        break;
    }
    case Model_Code:
    {
        if((31 < targetChar) && (targetChar < 127))
        {
            return Error_None;
        }
        break;
    }
    }
    return Error_NullChar;
}

CString::CStringError CString::check(const int beginIndex, const int endIndex)
{
    if(beginIndex < 0 || endIndex > this->m_qstring.size() - 1 || beginIndex > endIndex)
    {
        return Error_NullIndex;
    }
    for(int i = beginIndex; i <= endIndex; ++i)
    {
        if(check(beginIndex, endIndex) != Error_None)
        {
            return Error_NullString;
        }
    }
    return Error_None;
}

CString::CStringError CString::eliminate(const int index)
{
    return this->eliminate(index, index);
}

CString::CStringError CString::eliminate(const int beginIndex, const int endIndex)
{
    if(beginIndex < 0 || endIndex > this->m_qstring.size() - 1 || beginIndex > endIndex)
    {
        return Error_NullIndex;
    }
    QString temporaryString;
    for(int i = 0; i < beginIndex; ++i)
    {
        temporaryString += this->m_qstring[i];
    }
    for(int i = endIndex + 1; i > endIndex && i < this->m_qstring.size(); ++i)
    {
        temporaryString += this->m_qstring[i];
    }
    if(this->m_qstring.size() == temporaryString.size() + (endIndex - beginIndex + 1))
    {
        this->m_qstring = temporaryString;
        return Error_None;
    }
    return Error_AbortedEliminate;
}

CString::CStringError CString::insert(const int index, const QChar targetChar)
{
    QString targetString = QString(targetChar);
    return this->insert(index, targetString);
}

CString::CStringError CString::insert(const int index, const QString target)
{
    if(index < 0 || index > this->m_qstring.size() - 1)
    {
        return Error_NullIndex;
    }
    else if(target == "")
    {
        return Error_NullString;
    }
    QString temporaryString;
    for(int i = 0; i < index; ++i)
    {
        temporaryString += this->m_qstring[i];
    }
    temporaryString += target;
    for(int i = index; i < this->m_qstring.size(); ++i)
    {
        temporaryString += this->m_qstring[i];
    }
    if(this->m_qstring.size() == temporaryString.size() - target.size())
    {
        this->m_qstring = temporaryString;
        return Error_None;
    }
    return Error_AbortedInset;
}

CString::CStringError CString::replace(const int index, const QChar targetChar)
{
    QString targetString = QString(targetChar);
    return this->replace(index, index, targetString);
}

CString::CStringError CString::replace(const int beginIndex, const int endIndex, const QString targetString)
{
    CStringError abc = this->eliminate(beginIndex, endIndex);
    if(abc == Error_None)
    {
        return this->insert(beginIndex, targetString);
    }
    return Error_AbortedReplace;
}

CString::CStringError CString::set(const QString setString)
{
    this->m_qstring = setString;
    if(this->m_qstring == setString)
    {
        return Error_None;
    }
    return Error_AbortedSet;
}
