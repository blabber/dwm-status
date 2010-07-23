/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <dev/acpica/acpiio.h>
#include <sys/ioctl.h>

#include "battery.h"

static const char *ACPIDEV = "/dev/acpi";

enum {
        STRLEN = 32
};

struct battery_context {
        int             fd;
        char            battery_str[STRLEN];
};

struct battery_context *
battery_context_open()
{
        struct battery_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EX_SOFTWARE, "malloc(%d) battery_context", sizeof(*ctx));
        if ((ctx->fd = open(ACPIDEV, O_RDONLY)) == -1)
                err(EX_OSFILE, "open(%s)", ACPIDEV);

        return (ctx);
}

void
battery_context_close(struct battery_context *ctx)
{
        assert(ctx != NULL);

        if (close(ctx->fd) == -1)
                err(EX_OSFILE, "close(%s)", ACPIDEV);
        free(ctx);
}

char           *
battery_str(struct battery_context *ctx)
{
        union acpi_battery_ioctl_arg battio;
        const char     *state;

        assert(ctx != NULL);

        battio.unit = ACPI_BATTERY_ALL_UNITS;
        if (ioctl(ctx->fd, ACPIIO_BATT_GET_BATTINFO, &battio) == -1)
                err(EX_IOERR, "ioctl(ACPIIO_BATT_GET_BATTINFO)");

        if (battio.battinfo.state == 0)
                state = "=";
        else if (battio.battinfo.state & ACPI_BATT_STAT_CRITICAL)
                state = "!";
        else if (battio.battinfo.state & ACPI_BATT_STAT_DISCHARG)
                state = "-";
        else if (battio.battinfo.state & ACPI_BATT_STAT_CHARGING)
                state = "+";
        else
                state = "?";

        snprintf(ctx->battery_str, sizeof(ctx->battery_str), "%d%% [%s]", battio.battinfo.cap, state);

        return (ctx->battery_str);
}
