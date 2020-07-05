#pragma once
namespace Base
{
    class NonCopyAble
    {
 protected:
  NonCopyAble() {}
  ~NonCopyAble() {}

 private:
  NonCopyAble(const NonCopyAble&);
  const NonCopyAble& operator=(const NonCopyAble&);
    };
} // namespace Base
