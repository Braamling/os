#include "admin.h"

long make_admin(long next_index, int used) {
	long used_mask, admin;

	/* Destroy unwanted bits from the 'used' int and Move the 'used' bit to
	 * the 31st place. */
	used_mask = (used & 1) << 31;

	/* Combine the used mask with the index of the next block. */
	admin = next_index | used_mask;

	return admin;
}
