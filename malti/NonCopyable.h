#pragma once

// �h���N���X���R�s�[�֎~�ɂ���B
// private�p�����Ďg���B
class NonCopyable{
protected:
    NonCopyable(){}
    ~NonCopyable(){}

private:
	void operator=(const NonCopyable& src);
	NonCopyable(const NonCopyable& src);
   // const NonCopyable& operator=(const NonCopyable&);
};