/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <sys/ioctl.h>

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include <dev/acpica/acpiio.h>

#include "battery.h"
#include "buffers.h"

static const char *ACPIDEV = "/dev/acpi";
static const wchar_t *NOTAVAILABLE = L"n/a";

struct battery_context {
	int	fd;
	wchar_t	battery_str[BATTERY_BUFFLEN];
};

struct battery_context *
battery_context_open(void)
{
	struct battery_context *ctx;

	if ((ctx = malloc(sizeof(*ctx))) == NULL)
		err(EXIT_FAILURE, "malloc battery_context");
	if ((ctx->fd = open(ACPIDEV, O_RDONLY)) == -1)
		err(EXIT_FAILURE, "open(%s)", ACPIDEV);

	return (ctx);
}

void
battery_context_close(struct battery_context *ctx)
{
	assert(ctx != NULL);

	if (close(ctx->fd) == -1)
		err(EXIT_FAILURE, "close(%s)", ACPIDEV);
	free(ctx);
}

wchar_t *
battery_str(struct battery_context *ctx)
{
	union acpi_battery_ioctl_arg	battio;
	wchar_t				state;

	assert(ctx != NULL);

	battio.unit = ACPI_BATTERY_ALL_UNITS;
	if (ioctl(ctx->fd, ACPIIO_BATT_GET_BATTINFO, &battio) == -1)
		err(EXIT_FAILURE, "ioctl(ACPIIO_BATT_GET_BATTINFO)");

	if (battio.battinfo.cap == -1) {
		wcsncpy(ctx->battery_str, NOTAVAILABLE,
		    WCSLEN(ctx->battery_str) - 1);
		ctx->battery_str[WCSLEN(ctx->battery_str) - 1] = '\0';
		goto exit;
	}
	if (battio.battinfo.state == 0)
		state = L'=';
	else if (battio.battinfo.state & ACPI_BATT_STAT_CRITICAL)
		state = L'!';
	else if (battio.battinfo.state & ACPI_BATT_STAT_DISCHARG)
		state = L'-';
	else if (battio.battinfo.state & ACPI_BATT_STAT_CHARGING)
		state = L'+';
	else
		state = L'?';

	assert(battio.battinfo.cap >= 0 && battio.battinfo.cap <= 100);

	if (swprintf(ctx->battery_str, WCSLEN(ctx->battery_str), L"%d%% [%C]",
	    battio.battinfo.cap, state) == -1)
		errx(EXIT_FAILURE, "swprintf");

exit:
	return (ctx->battery_str);
}
