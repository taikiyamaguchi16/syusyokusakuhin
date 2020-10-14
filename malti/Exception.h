#pragma once

#include <exception>
#include <string>

// �G���[�R�[�h
// �}�C�i�X: �v���I�ȃG���[�B�v���O�������I�����Ȃ��Ƃ����Ȃ��B
// �v���X�F �x���B�v���O�������I�����Ȃ��Ă������B
struct ErrorCode{
    enum Type{
        Fatal = -1,
        Warning = 1,
    };
};

class Exception : public std::exception{
private:
    typedef std::exception base;

    ErrorCode::Type m_ErrorCode;
    std::string m_Message;

public:
    Exception(ErrorCode::Type errorCode, const std::string& message) throw()
        : m_ErrorCode(errorCode), m_Message(message)
    {
    }

    // copy constructor
    Exception(const Exception& other) throw()
        : base(other),
        m_ErrorCode(other.m_ErrorCode),
        m_Message(other.m_Message)
    {
    }

    // copy assignment
    Exception& operator=(const Exception& rhs) throw(){
        base::operator=(rhs);
        m_ErrorCode = rhs.m_ErrorCode;
        m_Message = rhs.m_Message;
    }

    const char* what() const throw() override{
        return Message();
    }

    const char* Message() const throw(){
        return m_Message.c_str();
    }

    int ErrorCode() const throw(){
        return m_ErrorCode;
    }
};