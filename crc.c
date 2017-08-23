static inline unsigned reverse(unsigned x) {
   x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
   x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) << 4) | ((x >> 4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}

int crc32(const void* data, const unsigned polynomial, const int init, const int xorout) {
  const unsigned p0 = reverse(polynomial),
                 p1 = p0 >> 1,
                 p2 = p0 >> 2,
                 p3 = p0 >> 3,
                 p4 = p0 >> 4,
                 p5 = p0 >> 5,
                 p6 = (p0 >> 6) ^ p0,
                 p7 = ((p0 >> 6) ^ p0) >> 1;
  const unsigned char* current = (const unsigned char*) data;
  int crc = init;
  unsigned byte;

  while ((byte = *current++) != 0) {
      crc ^= byte;
      crc = (
        ((crc<<31>>31) & p7) ^
        ((crc<<30>>31) & p6) ^
        ((crc<<29>>31) & p5) ^
        ((crc<<28>>31) & p4) ^
        ((crc<<27>>31) & p3) ^
        ((crc<<26>>31) & p2) ^
        ((crc<<25>>31) & p1) ^
        ((crc<<24>>31) & p0) ^
        ((unsigned)crc >> 8)
      );
   }
   return crc ^ xorout;
}

