#include <cstdint>
#include "servermsg.hpp"
#include "processcreateaccount.hpp"

void ProcessCreateAccount::on_SM_CREATEACCOUNTOK(const uint8_t *, size_t)
{
    setInfoStr(u8"注册成功", 2);
    m_boxID.setFocus(false);
    m_boxPwd.setFocus(false);
    m_boxPwdConfirm.setFocus(false);
}

void ProcessCreateAccount::on_SM_CREATEACCOUNTERROR(const uint8_t *buf, size_t)
{
    const auto smCAE = ServerMsg::conv<SMCreateAccountError>(buf);
    switch(smCAE.error){
        case CRTACCERR_ACCOUNTEXIST:
            {
                setInfoStr(u8"账号已存在", 2);
                clearInput();

                m_boxID.setFocus(true);
                m_boxPwd.setFocus(false);
                m_boxPwdConfirm.setFocus(false);
                return;
            }
        case CRTACCERR_BADACCOUNT:
            {
                setInfoStr(u8"无效的账号", 2);
                clearInput();

                m_boxID.setFocus(true);
                m_boxPwd.setFocus(false);
                m_boxPwdConfirm.setFocus(false);
                return;
            }
        case CRTACCERR_BADPASSWORD:
            {
                setInfoStr(u8"无效的密码", 2);
                clearInput();

                m_boxID.setFocus(true);
                m_boxPwd.setFocus(false);
                m_boxPwdConfirm.setFocus(false);
                return;
            }
        default:
            {
                throw fflreach();
            }
    }
}
