#include "bitmap.h"

/**
 * @brief Clones a bitmap, creating a new copy of the source bitmap.
 *
 * This function creates a new bitmap that is a copy of the provided source bitmap.
 * It initializes the new bitmap and copies the bits from the source bitmap
 * to the new one.
 *
 * @param src - Pointer to the source bitmap structure to be cloned.
 * 
 * @return Pointer to the newly created bitmap on success, or NULL on error
 *         (e.g., if the source bitmap is NULL, has no bits,
 *         or if initialization fails).
 */
struct bitmap *bmap_clone(struct bitmap *src) {
  log_msg("Starting to clone bitmap. Source bitmap: %p.", (void*)src);
  if (src == NULL || src->bits == NULL) {
    log_msg("Error: source bitmap or its bits are NULL.");
    return NULL;
  }
  struct bitmap *bm;
  if (bmap_init(&bm, src->size)) {
    log_msg("Error: failed to initialize new bitmap.");
    return NULL;
  }
  log_msg("Copying bits from source bitmap to new bitmap. Size: %zu.", src->size);
  memcpy(bm->bits, src->bits, bmap_get_len(src) * sizeof(bit_type));
  bm->size = src->size;
  log_msg("Bitmap cloned successfully. New bitmap: %p.", (void*)bm);
  return bm;
}
