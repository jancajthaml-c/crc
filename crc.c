#include "crc.h"

int crc32(const char* data, const unsigned int polynomial, unsigned long length) {
  int crc = ~0;

  const unsigned int p7 = polynomial >> 1;
  const unsigned int p6 = polynomial >> 2;
  const unsigned int p5 = polynomial >> 3;
  const unsigned int p4 = polynomial >> 4;
  const unsigned int p3 = polynomial >> 5;
  const unsigned int p2 = (polynomial >> 6) ^ polynomial;
  const unsigned int p1 = (polynomial >> 7) ^ p7;

  goto *((*data == '\0') ? &&result : &&loop);

loop:
  crc ^= *data++;
  crc = (
      (((crc << 31) >> 31) & p1) ^
      (((crc << 30) >> 31) & p2) ^
      (((crc << 29) >> 31) & p3) ^
      (((crc << 28) >> 31) & p4) ^
      (((crc << 27) >> 31) & p5) ^
      (((crc << 26) >> 31) & p6) ^
      (((crc << 25) >> 31) & p7) ^
      (((crc << 24) >> 31) & polynomial)
  ) ^ ((unsigned int)crc >> 8);

  goto *(--length ? &&loop : &&result);

result:
  return ~crc;
}
