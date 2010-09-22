/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include <mpd/connection.h>
#include <mpd/error.h>
#include <mpd/player.h>
#include <mpd/song.h>
#include <mpd/status.h>
#include <mpd/tag.h>

#include "mpd.h"

enum {
        STRLEN = 64,
};

const char     *NOTAVAILABLE = "n/a";
const char     *NOARTIST = "unknown artist";
const char     *NOTITLE = "unknown title";

struct mpd_context {
        struct mpd_connection *conn;
        char           *mpd_utf;
        char           *mpd_str;
        iconv_t         cd;
};

struct mpd_context *
mpd_context_open()
{
        struct mpd_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EX_SOFTWARE, "malloc(%d) mpd_context", sizeof(*ctx));

        if ((ctx->mpd_utf = malloc(STRLEN)) == NULL)
                err(EX_SOFTWARE, "malloc(%d)", STRLEN);

        if ((ctx->mpd_str = malloc(STRLEN)) == NULL)
                err(EX_SOFTWARE, "malloc(%d)", STRLEN);

        if ((ctx->cd = iconv_open("", "UTF-8")) == (iconv_t) (-1))
                err(EX_SOFTWARE, "iconv_open");

        if ((ctx->conn = mpd_connection_new(NULL /* host */ , 0 /* port */ , 0 /* timeout */ )) == NULL)
                err(EX_SOFTWARE, "mpd_connection_new");

        if (mpd_connection_get_error(ctx->conn) != MPD_ERROR_SUCCESS)
                err(EX_SOFTWARE, "mpd_connection_new: %s", mpd_connection_get_error_message(ctx->conn));

        return (ctx);
}

void
mpd_context_close(struct mpd_context *ctx)
{
        assert(ctx != NULL);

        free(ctx->mpd_utf);
        free(ctx->mpd_str);

        if (iconv_close(ctx->cd) == -1)
                err(EX_SOFTWARE, "iconv_close");

        assert(ctx->conn != NULL);
        mpd_connection_free(ctx->conn);

        free(ctx);
}

char           *
mpd_str(struct mpd_context *ctx)
{
        struct mpd_song *song = NULL;
        struct mpd_status *status = NULL;
        const char     *artist, *title;
        char           *in, *out;
        int             inleft, outleft;

        assert(ctx != NULL);

        if ((status = mpd_run_status(ctx->conn)) == NULL) {
                strncpy(ctx->mpd_str, NOTAVAILABLE, STRLEN - 1);
                ctx->mpd_str[STRLEN] = '\0';
                goto exit;
        }
        if (mpd_status_get_state(status) != MPD_STATE_PLAY) {
                ctx->mpd_str[0] = '\0';
                goto exit;
        }
        if ((song = mpd_run_current_song(ctx->conn)) == NULL) {
                strncpy(ctx->mpd_str, NOTAVAILABLE, STRLEN - 1);
                ctx->mpd_str[STRLEN] = '\0';
                goto exit;
        }
        if ((artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) == NULL)
                artist = NOARTIST;

        if ((title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) == NULL)
                title = NOTITLE;

        inleft = snprintf(ctx->mpd_utf, STRLEN, "%s - %s", artist, title);

        inleft = inleft < STRLEN ? inleft : STRLEN;
        outleft = STRLEN;
        in = ctx->mpd_utf;
        out = ctx->mpd_str;

        while (inleft > 0) {
                if (iconv(ctx->cd, (const char **)&in, &inleft, &out, &outleft) == (size_t) (-1)) {
                        if (errno == E2BIG)
                                break;
                }
        }
        *out = '\0';

exit:
        if (song != NULL)
                mpd_song_free(song);
        if (status != NULL)
                mpd_status_free(status);

        return (ctx->mpd_str);
}
