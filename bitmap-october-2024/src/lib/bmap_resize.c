#include "bitmap.h"

/**
 * @brief Resizes the bitmap to a new size.
 *
 * This function resizes the specified bitmap to the new size in bits.
 * It allocates new memory for the bits and copies the existing bits
 * from the old bitmap to the new one. If the new size is smaller than
 * the old size, only the bits that fit in the new size are copied.
 *
 * @param bm - Pointer to the bitmap structure to be resized.
 * @param size - New size of the bitmap in bits.
 *
 * @return 0 on successful resize, -1 on error (e.g., memory allocation failure).
 */
int bmap_resize(struct bitmap *bm, size_type size) {
  log_msg("Starting bitmap resize. Old size: %zu bits.", bm->size);
  if (bm == NULL)  {
    log_msg("Error: bitmap pointer is NULL.");
    return FAILURE_CODE;
  }
  int old_len = bmap_get_len(bm);
  int new_len = ceil((double)size / (double)sizeof(bit_type));
  bit_type *new_bits = calloc(new_len, sizeof(bit_type));
  if (new_bits == NULL) {
    log_msg("Error: memory allocation for new bits failed.");
    return FAILURE_CODE;
  }
  for (int i = 0, n = new_len; n && old_len; ++i, --n, --old_len) {
    new_bits[i] = bm->bits[i];
  }
  free(bm->bits);
  bm->bits = new_bits;
  bm->size = size;
  log_msg("Bitmap resize completed successfully. New size: %zu bits.", bm->size);
  return SUCCESS_CODE;
}
