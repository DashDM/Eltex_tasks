#include "bitmap.h"

/**
 * @brief Retrieves the length of the bitmap in terms of the number of bit_type 
 * elements.
 *
 * This function calculates the number of `bit_type` elements required to store
 * the bits in the bitmap based on its size in bits. It returns the length of the
 * bitmap in terms of the number of `bit_type` elements.
 *
 * @param bm - Pointer to the bitmap structure.
 *
 * @return The length of the bitmap in `bit_type` elements, or -1 if the bitmap
 *         pointer or its bits are NULL.
 */
int bmap_get_len(struct bitmap *bm) {
  log_msg("Retrieving the length of the bitmap.");
  if (bm == NULL || bm->bits == NULL) {
    log_msg("Error: bitmap pointer or its bits are NULL.");
    return -1;
  }
  int len = ceil((double)bm->size / (sizeof(bit_type) * BITS_IN_BYTE));
  log_msg("Successfully retrieved the length of the bitmap: %d.", len);
  return len;
}
