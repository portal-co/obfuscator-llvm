#ifndef _OBFUSCATION_UTILS_H_
#define _OBFUSCATION_UTILS_H_

#include "CryptoUtils.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/ConvertUTF.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
//#include "unicodelib.h"
//#include "unicodelib_encodings.h"
#include <codecvt>
#include <exception>
#include <locale>
#include <sstream>
#include <string>

void fixStack(llvm::Function *f);
llvm::StringRef readAnnotate(llvm::Function *f, llvm::StringRef attr);
bool toObfuscate(bool flag, llvm::Function *f, llvm::StringRef attribute);
inline std::u32string from(char32_t a, int sz) {
  std::u32string b;
  while (sz--) {
    b += a;
    a++;
  };
  return b;
}
// inline std::u32string remap_ =
//     std::u32string(U"ⵔⴱⵀⴷⴸⴹⴺⴽⴿⴼⵊⵏⵍⵐⵌ") + from(U'𑃐', 16) + from(0x1352, 32) +
//     from(0x10c00, 64) + from(0xabc0, 32) + from(0x10920, 16) +
//     from(0x10000, 64) + from(0x16a40, 16) + std::u32string(U"_");
inline char s(char x) {
  if (x < 0)
    return ((-x) - 1) * 2 + 1;
  return x * 2;
}
inline std::string remap(const char *x) {
  std::string res;
  char n = 0;
  while (*x) {
    auto y = *x - n;
    n = *x;
    x++;
    y = y * 31 + 33;
    res += (y % 16 + 65);
    res += (y / 16 + 65);
  };
  return res;
}
inline llvm::Function &aFn(llvm::Module &m) {
x:
  for (auto &fn : m.functions())
    if (llvm::cryptoutils->get_uint8_t() == 0)
      return fn;
  goto x;
// throw std::exception();
}
bool &_g_isCringe();
#endif
