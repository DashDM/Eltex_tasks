#include "../lib/bitmap.h"

/**
 * @brief Performs a bitwise OR operation on two bitmaps.
 *
 * This function creates a new bitmap that is the result of performing
 * a bitwise OR operation on the bits of the `first` and `second`
 * bitmaps. The resulting bitmap will have bits set to 1 where at least
 * one of the input bitmaps has bits set to 1.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 * 
 * @return Pointer to the newly created bitmap on success,
 *         or NULL on error (e.g., if either input bitmap is NULL,
 *         if any of the bitmaps have no bits, or if initialization fails).
 */
struct bitmap *bmap_or(struct bitmap *first, struct bitmap *second) {
  log_msg("Starting OR operation. First_bm: %p, Second_bm: %p",
          (void*)first, (void*)second);
  if (first == NULL || second == NULL || first->bits == NULL ||
      second->bits == NULL) {
    log_msg("Error: one of the input bitmaps or its bits are NULL.");
    return NULL;
  }
  struct bitmap *result = NULL;
  size_type max_size =
      (first->size > second->size) ? first->size : second->size;
  if (bmap_init(&result, max_size)) {
    log_msg("Error: failed to initialize result bitmap.");
    return NULL;
  }
  int bit1 = 0;
  int bit2 = 0;
  int idx1 = first->size - 1;
  int idx2 = second->size - 1;
  int idx_res = max_size - 1;
  log_msg("Performing bitwise OR operation.");
  for (; idx1 >= 0 && idx2 >= 0; --idx2, --idx1, --idx_res) {
    bmap_getbit(first, idx1, &bit1);
    bmap_getbit(second, idx2, &bit2);
    bmap_setbit(result, idx_res, bit1 | bit2);
    log_msg("Setting result bitmap at index %d to %d (bit1: %d, bit2: %d)",
            idx_res, bit1 | bit2, bit1, bit2);
  }
  for (; idx1 >= 0; --idx1, --idx_res) {
    bmap_getbit(first, idx1, &bit1);
    bmap_setbit(result, idx_res, bit1 | 0);
    log_msg("Setting result bitmap at index %d to %d (bit1: %d)",
            idx_res, bit1 | 0, bit1);
  }
  for (; idx2 >= 0; --idx2, --idx_res) {
    bmap_getbit(second, idx2, &bit2);
    bmap_setbit(result, idx_res, bit2 | 0);
    log_msg("Setting result bitmap at index %d to %d (bit2: %d)",
            idx_res, bit2 | 0, bit2);
  }
  log_msg("Bitwise OR operation completed successfully.");
  return result;
}
