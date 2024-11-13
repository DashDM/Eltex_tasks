#include "bitmap.h"

/**
 * @brief Sets a specific bit in the bitmap.
 *
 * This function sets or clears a bit at the specified position in the
 * bitmap. If the specified bit is set to 1, it will be turned on; if
 * it is set to 0, it will be turned off.
 *
 * @param bm - Pointer to the bitmap structure.
 * @param pos - Position of the bit to be set (0-based index).
 * @param bit - Value to set (1 to set the bit, 0 to clear it).
 *
 * @return 0 on successful operation, -1 on error (e.g., invalid bitmap,
 *         position out of bounds).
 */
int bmap_setbit(struct bitmap *bm, size_type pos, int bit) {
  log_msg("Starting to set bit at position %zu to value %d", pos, bit);
  if (bm == NULL || bm->bits == NULL || pos >= bm->size) {
    log_msg("Error: bitmap pointer or its bits are NULL, "
            "or position is out of bounds");
    return FAILURE_CODE;
  }
  int bits_in_one = sizeof(bit_type) * BITS_IN_BYTE;
  int outer_index = pos / bits_in_one;
  int inner_index = pos % bits_in_one;
  bit_type mask = 1 << (bits_in_one - 1);
  mask >>= inner_index;
  if (bit) {
    bm->bits[outer_index] |= mask;
    log_msg("Successfully set bit at position %zu to 1.", pos);
  } else {
    bm->bits[outer_index] &= ~mask;
    log_msg("Successfully set bit at position %zu to 0.", pos);
  }
  return SUCCESS_CODE;
}
