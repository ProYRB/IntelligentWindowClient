/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CSTRING_H
#define CSTRING_H

#include <QDebug>
#include <QList>
#include <QString>

class CString : public QString
{
public:
    enum CStringError
    {
        Error_AbortedEliminate,     //删除失败
        Error_AbortedInset,         //插入失败
        Error_AbortedReplace,       //替换失败
        Error_AbortedSet,           //设置失败
        Error_None,                 //无错误
        Error_NullChar,             //无效的字符
        Error_NullIndex,            //无效的下标
        Error_NullModel,            //无效的模式
        Error_NullString,           //无效字符串
    };
    enum CStringModel
    {
        Model_None,             //针对空白类型
        Model_Account,          //针对账号类型
        Model_Code,             //针对代码类型
        Model_Password,         //针对密码类型
    };

    CString();
    CString(QString qstring, CStringModel checkModel = Model_None) : m_qstring(qstring), m_checkModel(checkModel) {};   //构造函数的重载
    ~CString();

    bool isEmpty();     //处理对象是否为空
    qint16 getSize();           //返回当前字符串的大小
    QString getString();        //返回当前字符串

    CStringError check(const int index);        //检查指定对象
    CStringError check(const int beginIndex, const int endIndex);       //检查指定范围内的对象
    CStringError eliminate(const int index);    //删除指定对象
    CStringError eliminate(const int beginIndex, const int endIndex);   //删除指定对象
    CStringError insert(const int index, const QChar targetChar);       //在指定位置插入指定对象
    CStringError insert(const int index, const QString targetString);   //在指定位置插入指定对象
    CStringError replace(const int index, const QChar targetChar);      //替换指定对象
    CStringError replace(const int beginIndex, const int endIndex, const QString targetString);     //替换指定对象
    CStringError set(const QString string);     //设置处理对象

protected:

private:
    QString m_qstring;          //处理对象
    CStringModel m_checkModel = Model_None;     //检查模式

};

#endif // CSTRING_H
