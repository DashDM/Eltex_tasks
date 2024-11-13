#include "bitmap.h"


/**
 * @brief Compares two bitmaps to determine if the first is greater than the second.
 *
 * This function compares two bitmap structures and returns:
 * - 1 if the first bitmap is greater than the second,
 * - 0 if the first bitmap is less than or equal to the second,
 * - -1 if either bitmap is NULL.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 *
 * @return 1 if first > second, 0 if first <= second, -1 on error (NULL input).
 */
int bmap_is_greater(struct bitmap *first, struct bitmap *second) {
  log_msg("Comparing bitmaps. [First: %p] > [Second: %p]", 
          (void*)first, (void*)second);
  if (first == NULL || second == NULL) {
    log_msg("Error: one of the bitmaps is NULL.");
    return -1;
  }
  int bit1 = 0, bit2 = 0;
  size_type max_len = (first->size > second->size) ? first->size : second->size;
  size_type min_len = (first->size > second->size) ? second->size : first->size;
  size_type offset1 = max_len - first->size;
  size_type offset2 = max_len - second->size;
  for (int i = 0; i < (int)first->size - (int)second->size; i++) {
    bmap_getbit(first, i, &bit1);
    if (bit1 > 0) {
      log_msg("First bitmap is greater at index %d.", i);
      return 1;
    }
  }
  for (int i = 0; i < (int)second->size - (int)first->size; i++) {
    bmap_getbit(second, i, &bit1);
    if (bit1 > 0) {
      log_msg("Second bitmap is greater at index %d.", i);
      return 0;
    }
  }
  for (int i = 0; i < (int)min_len; i++) {
    bmap_getbit(first, i + offset2, &bit1);
    bmap_getbit(second, i + offset1, &bit2);
    if (bit1 > bit2) {
      log_msg("First bitmap is greater at index %d.", i + offset2);
      return 1;
    } else if (bit1 < bit2) {
      log_msg("Second bitmap is greater at index %d.", i + offset1);
      return 0;
    }
  }
  log_msg("Comparasion completed.");
  return 0;
}

/**
 * @brief Compares two bitmaps to determine if they are equal.
 *
 * This function checks if two bitmap structures are equal by comparing
 * their bits. It returns:
 * - 1 if the bitmaps are equal,
 * - 0 if they are not equal,
 * - -1 if either bitmap is NULL.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 *
 * @return 1 if first == second, 0 if first != second, -1 on error (NULL input).
 */
int bmap_is_eq(struct bitmap *first, struct bitmap *second) {
  log_msg("Comparing bitmaps. [First: %p] = [Second: %p]", 
          (void*)first, (void*)second);
  if (first == NULL || second == NULL) {
    log_msg("Error: one of the bitmaps is NULL.");
    return -1;
  }
  int bit1 = 0, bit2 = 0;
  size_type max_len = (first->size > second->size) ? first->size : second->size;
  size_type min_len = (first->size > second->size) ? second->size : first->size;
  size_type offset1 = max_len - first->size;
  size_type offset2 = max_len - second->size;
  for (int i = 0; i < (int)first->size - (int)second->size; i++) {
    bmap_getbit(first, i, &bit1);
    if (bit1 > 0) { 
      log_msg("First bitmap has extra bit set at index %d, not equal.", i);
      return 0;
    }
  }
  for (int i = 0; i < (int)second->size - (int)first->size; i++) {
    bmap_getbit(second, i, &bit1);
    if (bit1 > 0) {
      log_msg("Second bitmap has extra bit set at index %d, not equal.", i);
      return 0;
    }
  }
  for (int i = 0; i < (int)min_len; i++) {
    bmap_getbit(first, i + offset2, &bit1);
    bmap_getbit(second, i + offset1, &bit2);

    if (bit1 != bit2) {
      log_msg("Bit mismatch at index %d. First bit: %d, Second bit: %d",
              i + offset2, bit1, bit2);
      return 0;
    }
  }
  log_msg("Comparasion completed.");
  return 1;
}

/**
 * @brief Compares two bitmaps to determine if the first is less than the second.
 *
 * This function checks if the first bitmap is less than the second bitmap.
 * It returns:
 * - 1 if the first bitmap is less than the second,
 * - 0 if the first bitmap is greater than or equal to the second,
 * - -1 if either bitmap is NULL.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 *
 * @return 1 if first < second, 0 if first >= second, -1 on error (NULL input).
 */
int bmap_is_less(struct bitmap *first, struct bitmap *second) {
  log_msg("Comparing bitmaps. [First: %p] < [Second: %p]", 
          (void*)first, (void*)second);
  if (first == NULL || second == NULL) {
    log_msg("Error: one of the bitmaps is NULL.");
    return -1;
  }
  log_msg("Comparasion completed.");
  return !bmap_is_greater(first, second) && !bmap_is_eq(first, second);
}

/**
 * @brief Compares two bitmaps to determine if the first is greater than or 
 * equal to the second.
 *
 * This function checks if the first bitmap is greater than or equal
 * to the second bitmap.
 * It returns:
 * - 1 if the first bitmap is greater than or equal to the second,
 * - 0 if the first bitmap is less than the second,
 * - -1 if either bitmap is NULL.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 *
 * @return 1 if first >= second, 0 if first < second, -1 on error (NULL input).
 */
int bmap_is_greater_or_eq(struct bitmap *first, struct bitmap *second) {
  log_msg("Comparing bitmaps. [First: %p] >= [Second: %p]", 
          (void*)first, (void*)second);
  if (first == NULL || second == NULL) {
    log_msg("Error: one of the bitmaps is NULL.");
    return -1;
  }
  log_msg("Comparasion completed.");
  return bmap_is_greater(first, second) || bmap_is_eq(first, second);
}

/**
 * @brief Compares two bitmaps to determine if the first is less than or 
 * equal to the second.
 *
 * This function checks if the first bitmap is less than or equal
 * to the second bitmap.
 * It returns:
 * - 1 if the first bitmap is less than or equal to the second,
 * - 0 if the first bitmap is greater than the second,
 * - -1 if either bitmap is NULL.
 *
 * @param first - Pointer to the first bitmap.
 * @param second - Pointer to the second bitmap.
 *
 * @return 1 if first <= second, 0 if first > second, -1 on error (NULL input).
 */
int bmap_is_less_or_eq(struct bitmap *first, struct bitmap *second) {
  log_msg("Comparing bitmaps. [First: %p] <= [Second: %p]", 
          (void*)first, (void*)second);
  if (first == NULL || second == NULL) {
    log_msg("Error: one of the bitmaps is NULL.");
    return -1;
  }
  log_msg("Comparasion completed.");
  return bmap_is_less(first, second) || bmap_is_eq(first, second);
}
