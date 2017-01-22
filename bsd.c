/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include<stdlib.h>

#include "bsd.h"

int
bsd_getloadavg(double loadavg[], int nelem)
{
	/* getloadavg(3) is a non-standard bsdism. */
	return getloadavg(loadavg, nelem);
}
