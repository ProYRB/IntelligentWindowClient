/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cencrypt.h"

CEncrypt::CEncrypt()
{
    ;
}

CEncrypt::~CEncrypt()
{
    ;
}

QByteArray CEncrypt::XOR(QByteArray byteArray, CEncryptModel encryptMode)
{
    switch (encryptMode) {
    case Model_XOR:
    {
        QByteArray KEY = "8A3500F5BBD2A41";
        int keyIndex;
        for(int index = 0; index < byteArray.size(); ++index)
        {
            keyIndex = index % KEY.size();
            byteArray[index] = byteArray[index] ^ KEY[keyIndex];
        }
        break;
    }
    }
    return byteArray;
}
