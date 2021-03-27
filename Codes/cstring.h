/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#ifndef CSTRING_H
#define CSTRING_H

#include <QList>
#include <QString>

class CString : public QString
{
public:
    enum CStringError
    {
        Error_None,                 //无错误
        Error_AbortedSet,           //字符串设置失败
        Error_EmptyString,          //空字符串
        Error_AbortedDelete,        //删除失败
        Error_CharOverRange,        //字符不符合要求
        Error_IndexOverRange,       //下标不符合要求
        Error_ModelOverRange,       //模式不存在
    };
    enum CStringModel
    {
        Model_Account,       //针对账号类型
        Model_Password,      //针对密码类型
    };

    CString();
    CString(QString string) : m_string(string) {};      //构造函数的重载
    ~CString();

    int GetStringSize();        //返回当前字符串的长度(大小)

    QString GetString();        //返回当前字符串

    QList<int> CheckCString(const int beginIndex, const int endIndex, const CStringModel model);

    CStringError IfSet();                           //返回当前要处理的的字符串是否为空
    CStringError DeleteChar(const int index);       //删除指定的字符
    CStringError SetCString(const QString string, bool change = false);     //设置要处理的字符串
    CStringError CheckChar(const int index, const CStringModel model);      //检查指定位置的字符

protected:

private:
    QString m_string = "";      //当前要处理的变量

};

#endif // CSTRING_H
