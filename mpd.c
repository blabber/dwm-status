/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#define _POSIX_C_SOURCE 199506

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mpd/connection.h>
#include <mpd/error.h>
#include <mpd/player.h>
#include <mpd/song.h>
#include <mpd/status.h>
#include <mpd/tag.h>

#include "buffers.h"
#include "mpd.h"
#include "tools.h"

enum {
	SLEEP = 5,
};

static const char *NOTAVAILABLE = "n/a";
static const char *NOARTIST = "unknown artist";
static const char *NOTITLE = "unknown title";

struct mpd_context {
	struct mpd_connection	*conn;
	char			 mpd_str[MPD_BUFFLEN];
	time_t			 last;
};

struct mpd_context *
mpd_context_open(void)
{
	struct mpd_context *ctx;

	if ((ctx = malloc(sizeof(*ctx))) == NULL)
		err(EXIT_FAILURE, "malloc mpd_context");

	ctx->conn = NULL;
	ctx->mpd_str[0] = '\0';
	ctx->last = time(NULL) - SLEEP;

	return (ctx);
}

void
mpd_context_close(struct mpd_context *ctx)
{
	assert(ctx != NULL);

	if (ctx->conn != NULL)
		mpd_connection_free(ctx->conn);

	free(ctx);
}

char           *
mpd_str(struct mpd_context *ctx)
{
	struct mpd_song		*song = NULL;
        struct mpd_status	*status = NULL;
        const char		*artist, *title;
        time_t			 now;

	assert(ctx != NULL);

	now = time(NULL);
	if ((now - ctx->last) < SLEEP)
		goto exit;
	ctx->last = now;

	if (ctx->conn == NULL) {
		if ((ctx->conn = mpd_connection_new(NULL, 0, 0)) == NULL) {
			warnx("mpd_connection_new");
			goto exit;
		}
	}
	assert(ctx->conn != NULL);

	if ((status = mpd_run_status(ctx->conn)) == NULL) {
		warnx("mpd_run_status: %s", mpd_connection_get_error_message(ctx->conn));
		strncpy(ctx->mpd_str, NOTAVAILABLE, sizeof(ctx->mpd_str) - 1);
		ctx->mpd_str[sizeof(ctx->mpd_str) - 1] = '\0';

		mpd_connection_free(ctx->conn);
		ctx->conn = NULL;
		goto exit;
	}
	if (mpd_status_get_state(status) != MPD_STATE_PLAY) {
		ctx->mpd_str[0] = '\0';
		goto exit;
	}
	if ((song = mpd_run_current_song(ctx->conn)) == NULL) {
		strncpy(ctx->mpd_str, NOTAVAILABLE, sizeof(ctx->mpd_str) - 1);
		ctx->mpd_str[sizeof(ctx->mpd_str) - 1] = '\0';
		goto exit;
	}
	if ((artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) == NULL)
		artist = NOARTIST;

	if ((title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) == NULL)
		title = NOTITLE;

	if (tools_catitems(ctx->mpd_str, sizeof(ctx->mpd_str),
	    artist, " - ", title, NULL) == -1)
		errx(EXIT_FAILURE, "tools_catitems");

exit:
	if (song != NULL)
		mpd_song_free(song);
	if (status != NULL)
		mpd_status_free(status);

	return (ctx->mpd_str);
}
