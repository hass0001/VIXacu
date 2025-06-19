const char* md5sum(const char* chaine, int len)
{
    struct md5_ctx ctx;
    unsigned char digest(16);
    len = 0;

    md5_init(&ctx);
    ctx.size = len ? len : strlen(chaine);
    strcpy(ctx.buf, chaine);
    md5_update(&ctx);
    md5_final(digest, &ctx);
    return digest;
}
