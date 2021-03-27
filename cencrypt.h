/******************************
 *  @Author:YRB
 *  @Date:2021Year
******************************/

#ifndef CENCRYPT_H
#define CENCRYPT_H

#include <QChar>
#include <QString>
#include <QByteArray>

class CEncrypt
{
public:
    enum CEncryptError
    {
        Error_None,         //无错误
    };
    enum CEncryptModel
    {
        Model_XOR,          //异或加密解密
    };

    CEncrypt();
    ~CEncrypt();

    QByteArray XOR(QByteArray, CEncryptModel);

protected:

private:

};

#endif // CENCRYPT_H
