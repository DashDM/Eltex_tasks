#include "bitmap.h"

/**
  * @brief Initializes a bitmap structure.
  * 
  * Allocates memory for the structure and its bits, sets the size.
  * 
  * @param bm - pointer to a pointer of the bitmap structure to be initialized.
  * @param size - size of the bitmap in bits.
  *
  * @return 0 on successful initialization, -1 on error.
  */

int bmap_init(struct bitmap **bm, size_type size) {
  log_msg("Starting bitmap initialization with size: %zu bits.", size);
  if (size == 0) {
    log_msg("Error: bitmap size is zero.");
    return FAILURE_CODE;
  }
  *bm = calloc(1, sizeof(struct bitmap));
  if (*bm == NULL) {
    log_msg("Error: failed to allocate memory for bitmap structure.");
    return FAILURE_CODE;
  }
  int len = ceil((double)size / ((double)sizeof(bit_type) * BITS_IN_BYTE));
  (*bm)->bits = calloc(len, sizeof(bit_type));
  if ((*bm)->bits == NULL) {
    free(*bm);
    log_msg("Error: failed to allocate memory for bits.");
    return FAILURE_CODE;
  }
  (*bm)->size = size;
  log_msg("Bitmap initialization completed successfully. Bitmap size: %zu bits, "
          "allocated memory for bits: %d bytes.", size, len * sizeof(bit_type));
  return SUCCESS_CODE;
}
