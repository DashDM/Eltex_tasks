#include "bitmap.h"

/**
 * @brief Deletes the bitmap and frees associated memory.
 *
 * This function deallocates the memory used by the bitmap structure and its
 * underlying bit array. After calling this function, the bitmap pointer is
 * set to NULL to avoid dangling pointers.
 *
 * @param bm Pointer to a pointer to the bitmap structure. This allows the
 *           function to modify the original pointer to NULL after deletion.
 *
 * @note The caller should ensure that the bitmap is not NULL before calling
 *       this function to avoid undefined behavior.
 */
void bmap_delete(struct bitmap **bm) {
  log_msg("Starting bitmap delete.");
  if (bm == NULL || *bm == NULL) {
    log_msg("Error: attempted to delete a NULL bitmap pointer.");
    return;
  }
  log_msg("Deleting bitmap at address: %p.", (void*)(*bm));
  free((*bm)->bits);
  free(*bm);
  log_msg("Bitmap deleted successfully");
}
