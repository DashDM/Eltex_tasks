#include "bitmap.h"

/**
 * @brief Concatenates two bitmaps into a new bitmap.
 *
 * This function creates a new bitmap that is the result of concatenating
 * the `left` and `right` bitmaps. The bits from the `left` bitmap are followed
 * by the bits from the `right` bitmap in the resulting bitmap.
 *
 * @param left - Pointer to the first bitmap to be concatenated.
 * @param right - Pointer to the second bitmap to be concatenated.
 * 
 * @return Pointer to the newly created concatenated bitmap on success,
 *         or NULL on error (e.g., if either input bitmap is NULL, if
 *         any of the bitmaps have no bits, or if initialization fails).
 */
struct bitmap *bmap_cat(struct bitmap *left, struct bitmap *right) {
  log_msg("Starting to concatenate two bitmaps. Left: %p, Right: %p",
          (void*)left, (void*)right);
  if (left == NULL || right == NULL || left->bits == NULL ||
      right->bits == NULL) {
    log_msg("Error: one of the input bitmaps or its bits are NULL.");
    return NULL;
  }
  log_msg("Initializing new bitmap for concatenation. Total size: %zu", 
          left->size + right->size);
  struct bitmap *result = NULL;
  if (bmap_init(&result, left->size + right->size) != 0) {
    log_msg("Error: failed to initialize new bitmap for concatenation.");
    return NULL;
  }
  size_type idx = 0;
  int bit = 0;
  log_msg("Copying bits from left bitmap to result bitmap.");
  for (size_type i = 0; i < left->size; ++i, ++idx) {
    bmap_getbit(left, i, &bit);
    bmap_setbit(result, idx, bit);
  }
  log_msg("Copying bits from right bitmap to result bitmap.");
  for (size_type i = 0; i < right->size; ++i, ++idx) {
    bmap_getbit(right, i, &bit);
    bmap_setbit(result, idx, bit);
  }
  log_msg("Successfully concatenated bitmaps. Result bitmap: %p", (void*)result);
  return result;
}
