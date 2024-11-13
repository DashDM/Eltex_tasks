#include "bitmap.h"

/**
 * @brief Copies a specified number of bits from the source bitmap
 * to the destination bitmap.
 *
 * This function copies `count` bits from the `src` bitmap starting
 * at the `start` position
 * to the `dest` bitmap at the specified `pos`. The destination bitmap
 * is resized if necessary to accommodate the copied bits.
 *
 * @param dest - Pointer to the destination bitmap structure where bits will be copied to.
 * @param src - Pointer to the source bitmap structure from which bits will be copied.
 * @param pos - Position in the destination bitmap where the bits will be copied.
 * @param start - Starting position in the source bitmap from which bits will be copied.
 * @param count - Number of bits to copy from the source to the destination bitmap.
 *
 * @return 0 on success, -1 on error (e.g., invalid bitmaps, out of bounds positions,
 *         or memory allocation failure).
 */
int bmap_copy(struct bitmap *dest, struct bitmap *src, size_type pos,
              size_type start, size_type count) {
  log_msg("Starting to copy %zu bits from source bitmap at position %zu to "
          "destination bitmap at position %zu.", count, start, pos);
  if (dest == NULL || src == NULL || dest->bits == NULL || src->bits == NULL ||
    pos > dest->size) {
    log_msg("Error: invalid parametrs for bmap_copy.");
    return FAILURE_CODE;
  }
  if (count) {
    size_type old_size = dest->size;
    log_msg("Resizing destination bitmap from size %zu to %zu.", 
            old_size, old_size + count);
    if (bmap_resize(dest, dest->size + count)) {
      log_msg("Error: failed to resize destination bitmap.");
      return FAILURE_CODE;
    }
    int bit = 0;
    if (pos != old_size) {
      for (size_type i = old_size - 1; i >= pos; --i) {
        bmap_getbit(dest, i, &bit);
        bmap_setbit(dest, i + count, bit);
        if (i == 0) break;
      }
    }
    log_msg("Copying %zu bits from src starting at %zu to dest at position %zu.",
            count, start, pos);
    for (size_type i = 0; i < count; ++i) {
      bmap_getbit(src, start + i, &bit);
      bmap_setbit(dest, pos + i, bit);
    }
  }
  log_msg("Successfully copied %zu bits from src to dest.", count);
  return SUCCESS_CODE;
}
