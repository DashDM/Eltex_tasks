#include "bitmap.h"

/**
 * @brief Retrieves the value of a specific bit in the bitmap.
 *
 * This function retrieves the value of a bit at a specified position in the
 * bitmap. The bit value is stored in the location pointed to by the `bit` parameter.
 * 
 * @param bm - Pointer to the bitmap structure.
 * @param pos - Position of the bit to retrieve (0-based index).
 * @param bit - Pointer to an integer where the bit value (0 or 1) will be stored.
 *
 * @return 0 on successful retrieval, -1 on error 
 *         (e.g., invalid bitmap, position out of bounds, or null pointer).
 */
int bmap_getbit(struct bitmap *bm, size_type pos, int *bit) {
  log_msg("Starting to retrieve the value of bit at position %zu.", pos);
  if (bm == NULL || bm->bits == NULL || pos >= bm->size || bit == NULL) {
    log_msg("Error: bitmap pointer or its bits are NULL, or position is out of bounds");
    return FAILURE_CODE;
  }
  int bits_in_one = sizeof(bit_type) * BITS_IN_BYTE;
  int outer_index = pos / bits_in_one;
  int inner_index = pos % bits_in_one;
  bit_type mask = 1 << (bits_in_one - 1);
  mask >>= inner_index;
  *bit = ((bm->bits[outer_index] & mask) > 0) ? 1 : 0;
  log_msg("Successfully retrieved bit value: %d at position %zu.", *bit, pos);
  return SUCCESS_CODE;
}
