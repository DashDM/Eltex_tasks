/**
 * @file bitmap.h
 * @brief Header file for bitmap operations.
 *
 * This file contains definitions and functions for working with bitmaps,
 * which are arrays of bits.
 *
 * @defgroup bitmap Bitmaps
 * @{
 */
#ifndef BITMAP_H
#define BITMAP_H

#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
/**
 * @brief Type for representing a bit.
 */
#define bit_type uint8_t
/**
 * @brief Type for representing size.
 */
#define size_type size_t

#define BITS_IN_BYTE 8

#define SUCCESS_CODE 0
#define FAILURE_CODE -1
/**
 * @brief Bitmap structure
 */
struct bitmap {
  bit_type *bits; /**< Pointer to the array of bits. */
  size_type size; /**< Size of the bitmap in bits. */
};

int bmap_init(struct bitmap **bm, size_type size);

void bmap_delete(struct bitmap **bm);

int bmap_resize(struct bitmap *bm, size_type size);

int bmap_get_len(struct bitmap *bm);

int bmap_is_greater(struct bitmap *first, struct bitmap *second);
int bmap_is_greater_or_eq(struct bitmap *first, struct bitmap *second);
int bmap_is_less(struct bitmap *first, struct bitmap *second);
int bmap_is_less_or_eq(struct bitmap *first, struct bitmap *second);
int bmap_is_eq(struct bitmap *first, struct bitmap *second);
int bmap_get_len(struct bitmap *bm);
struct bitmap *bmap_xor(struct bitmap *first, struct bitmap *second);
struct bitmap *bmap_or(struct bitmap *first, struct bitmap *second);
int bmap_not(struct bitmap *bm);

struct bitmap *bmap_clone(struct bitmap *src);
int bmap_copy(struct bitmap *dest, struct bitmap *src, size_type pos,
              size_type start, size_type count);

int bmap_getbit(struct bitmap *bm, size_type pos, int *bit);
int bmap_setbit(struct bitmap *bm, size_type pos, int bit);

struct bitmap *bmap_and(struct bitmap *first, struct bitmap *second);

void bmap_reset(struct bitmap *bm);
struct bitmap *bmap_cat(struct bitmap *left, struct bitmap *right);

#endif
