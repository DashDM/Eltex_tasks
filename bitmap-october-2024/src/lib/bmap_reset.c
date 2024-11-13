#include "bitmap.h"

/**
 * @brief Resets all bits in the bitmap to zero.
 *
 * This function sets all bits in the specified bitmap structure to zero,
 * effectively clearing the bitmap.
 *
 * @param bm - Pointer to the bitmap structure to be reset.
 *
 * @return None.
 */
void bmap_reset(struct bitmap *bm) {
  log_msg("Starting bitmap reset.");
  if (bm == NULL || bm->bits == NULL) {
    log_msg("Error: bitmap pointer or its bitss are NULL.");
    return;
  }
  int bm_len = bmap_get_len(bm);
  for (int i = 0; i < bm_len; i++) {
    bm->bits[i] = 0;
  }
  log_msg("Bitmap reset completed successfully. All bits set to 0.");
}
