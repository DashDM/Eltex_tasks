#include "../lib/bitmap.h"
/**
 * @brief Performs a bitwise NOT operation on a bitmap.
 *
 * This function inverts all bits in the given bitmap. Each bit that is
 * 0 becomes 1, and each bit that is 1 becomes 0. The operation is performed
 * in place, meaning the original bitmap is modified.
 *
 * @param bm - Pointer to the bitmap to be inverted.
 * 
 * @return SUCCESS_CODE on success, or FAILURE_CODE if the input bitmap
 *         is NULL or its bits are NULL.
 */
int bmap_not(struct bitmap *bm) {
  log_msg("Starting NOT operation. Bm: %p", (void*)bm);
  if (bm == NULL || bm->bits == NULL) {
    log_msg("Error: input bitmap or its bits are NULL.");
    return FAILURE_CODE;
  }
  int bit = 0;
  log_msg("Performing bitwise NOT operation.");
  for (int pos = bm->size - 1; pos >= 0; --pos) {
    bmap_getbit(bm, pos, &bit);
    log_msg("Bit at position %d before inversion: %d", pos, bit);
    bmap_setbit(bm, pos, ~bit & 1);
    log_msg("Bit at position %d after inversion: %d", pos, ~bit & 1);
  }
  log_msg("Bitwise NOT operation completed successfully.");
  return SUCCESS_CODE;
}
