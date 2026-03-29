#include "appcontext.h"

AppContext* get_ctx()
{
    AppContext* ctx = (AppContext*)calloc(1, sizeof(AppContext));
    ctx->filename = NULL;
    ctx->lines = (Vector){0};
    ctx->errors_count = 0;
    return ctx;
}

void del_ctx(AppContext* ctx)
{
    if(!ctx) return;
    // if(ctx->filename)  free(ctx->filename);
    if(ctx->lines.inited) v_destroy(&ctx->lines);
}
