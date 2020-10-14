#pragma once

// 派生クラスをコピー禁止にする。
// private継承して使う。
class NonCopyable{
protected:
    NonCopyable(){}
    ~NonCopyable(){}

private:
	void operator=(const NonCopyable& src);
	NonCopyable(const NonCopyable& src);
   // const NonCopyable& operator=(const NonCopyable&);
};