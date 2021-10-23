#include "stdFont.h"

#include "stdPlatform.h"
#include "General/stdBitmap.h"
#include "Win95/stdDisplay.h"
#include "Win95/std.h"

stdFont* stdFont_Load(char *fpath, int a2, int a3)
{
    stdFont *result; // eax
    stdFile_t fd; // ebx
    int16_t charLast; // bp
    unsigned int totalAlloc; // esi
    stdFont *fontAlloc; // edx
    stdFont *fontAlloc_; // ebp
    const char *fname; // eax
    stdFontEntry *pEntries; // edx
    int charMin; // ecx
    uint16_t charMax; // ax
    int16_t charLast_1; // bx
    stdFontCharset *lastCharset; // esi
    stdFontCharset *i; // ecx
    int v16; // edi
    stdFontCharset *charset; // eax
    size_t entries_readsize; // esi
    stdBitmap *bitmap; // eax
    stdFontCharset *v20; // esi
    stdFontCharset *v21; // edi
    struct common_functions *v22; // ecx
    int marginY; // [esp+14h] [ebp-40h]
    int marginX; // [esp+18h] [ebp-3Ch]
    int16_t header_field_10; // [esp+1Ch] [ebp-38h]
    int16_t charFirst; // [esp+20h] [ebp-34h]
    int16_t charFirsta; // [esp+20h] [ebp-34h]
    stdFontExtHeader extHeader; // [esp+24h] [ebp-30h] BYREF
    stdFontHeader header; // [esp+2Ch] [ebp-28h] BYREF
    int fpatha; // [esp+58h] [ebp+4h]

    fd = std_pHS->fileOpen(fpath, "rb");

    if (!fd)
        return NULL;

    if ( std_pHS->fileRead(fd, &header, 0x28) != 40 )
        goto LABEL_28;
    if ( _memcmp(&header, "SFNT", 4u) )
        return 0;
    if ( header.version != 10 )
        return 0;
    if ( std_pHS->fileRead(fd, &extHeader, 4) != 4 )
        goto LABEL_28;
    charLast = extHeader.characterLast;
    charFirst = extHeader.characterFirst;
    header_field_10 = header.field_10;
    marginX = header.marginX;
    totalAlloc = sizeof(stdFontEntry) * (extHeader.characterLast - extHeader.characterFirst) + sizeof(stdFont);
    marginY = header.marginY;
    fontAlloc = (stdFont *)std_pHS->alloc(totalAlloc);
    if ( fontAlloc )
    {
        _memset(fontAlloc, 0, totalAlloc);
        fontAlloc->charsetHead.pEntries = &fontAlloc->charsetHead.entries;
        fontAlloc->marginY = marginY;
        fontAlloc->marginX = marginX;
        fontAlloc->charsetHead.charLast = charLast;
        fontAlloc->field_28 = header_field_10;
        fontAlloc->charsetHead.charFirst = charFirst;
        fontAlloc_ = fontAlloc;
    }
    else
    {
        fontAlloc_ = 0;
    }
    fname = stdFileFromPath(fpath);
    _strncpy(fontAlloc_->name, fname, 0x1Fu);
    pEntries = fontAlloc_->charsetHead.pEntries;
    charMin = fontAlloc_->charsetHead.charFirst;
    charMax = fontAlloc_->charsetHead.charLast;
    fontAlloc_->name[31] = 0;
    if ( std_pHS->fileRead(fd, pEntries, sizeof(stdFontEntry) * (charMax - charMin + 1)) != sizeof(stdFontEntry) * (charMax - charMin + 1) )
        goto LABEL_28;
    fpatha = 1;
    if ( header.numCharsets > 1 )
    {
        while ( std_pHS->fileRead(fd, &extHeader, 4) == 4 )
        {
            charLast_1 = extHeader.characterLast;
            charFirsta = extHeader.characterFirst;
            lastCharset = &fontAlloc_->charsetHead;
            for ( i = fontAlloc_->charsetHead.previous; i; i = i->previous )
                lastCharset = i;
            v16 = sizeof(stdFontEntry) * (extHeader.characterLast - extHeader.characterFirst + 1) + (sizeof(stdFontCharset*) + sizeof(uint16_t)*2 + sizeof(stdFontEntry*));
            charset = (stdFontCharset *)std_pHS->alloc(v16*2);
            if ( charset )
            {
                lastCharset->previous = charset;
                _memset(charset, 0, v16);
                charset->pEntries = &charset->entries;
                charset->charFirst = charFirsta;
                charset->charLast = charLast_1;
            }
            if ( !charset )
            {
                std_pHS->fileClose(fd);
                return 0;
            }
            entries_readsize = sizeof(stdFontEntry) * (charset->charLast - charset->charFirst + 1);
            if ( std_pHS->fileRead(fd, charset->pEntries, entries_readsize) != entries_readsize )
            {
                break;
            }
            if ( ++fpatha >= header.numCharsets )
                goto LABEL_21;
        }
LABEL_28:
        std_pHS->fileClose(fd);
        return 0;
    }
LABEL_21:
    bitmap = stdBitmap_LoadFromFile(fd, a2, a3);
    fontAlloc_->bitmap = bitmap;
    if ( bitmap )
    {
        _strncpy((char *)bitmap, "FONTSTRIP", 0x1Fu);
        v22 = std_pHS;
        fontAlloc_->bitmap->field_1F = 0;
        v22->fileClose(fd);
        result = fontAlloc_;
    }
    else
    {
        v20 = fontAlloc_->charsetHead.previous;
        std_pHS->free(fontAlloc_);
        if ( v20 )
        {
            do
            {
                v21 = v20->previous;
                std_pHS->free(v20);
                v20 = v21;
            }
            while ( v21 );
        }
        result = 0;
    }
    return result;
}

unsigned int stdFont_Draw1(stdVBuffer *vbuf, stdFont *font, unsigned int blit_x, int blit_y, int a5, wchar_t *a6, int alpha_maybe)
{
    unsigned int v8; // edx
    unsigned int v9; // esi
    wchar_t *v10; // ebx
    wchar_t v11; // ax
    int v12; // ecx
    int v13; // eax
    uint16_t v14; // cx
    stdFontCharset *v15; // eax
    signed int v16; // ecx
    int v18; // [esp+10h] [ebp-14h]
    rdRect a5a; // [esp+14h] [ebp-10h] BYREF
    stdVBuffer *a2a; // [esp+2Ch] [ebp+8h]

    v8 = blit_x;
    v9 = blit_x;
    v18 = 0;
    a2a = *font->bitmap->mipSurfaces;
    a5a.y = 0;
    a5a.height = a2a->format.height;
    if ( a5 >= (int)(vbuf->format.width - blit_x) )
        a5 = vbuf->format.width - blit_x;
    v10 = a6;
    v11 = *a6;
    if ( *a6 )
    {
        while ( 1 )
        {
            if ( v18 )
                return v9 - v8;
            if ( v11 == 9 )
            {
                v12 = 16;
                if ( 8 * font->marginX >= 16 )
                    v12 = 8 * font->marginX;
                v9 = blit_x + v12 * ((int)(v12 + v9 - v8) / v12);
                goto LABEL_27;
            }
            if ( _iswspace(v11) )
                break;
            v14 = *v10;
            v15 = &font->charsetHead;
            if ( font != (stdFont *)-48 )
            {
                do
                {
                    if ( v14 >= v15->charFirst && v14 <= v15->charLast )
                        break;
                    v15 = v15->previous;
                }
                while ( v15 );
                if ( v15 )
                    goto LABEL_22;
            }
            v14 = font->field_28;
            v15 = &font->charsetHead;
            a5a.x = 0;
            a5a.width = 0;
            if ( font == (stdFont *)-48 )
                goto LABEL_21;
            do
            {
                if ( v14 >= v15->charFirst && v14 <= v15->charLast )
                    break;
                v15 = v15->previous;
            }
            while ( v15 );
            if ( v15 )
            {
LABEL_22:
                a5a.x = v15->pEntries[v14 - v15->charFirst].field_0;
                v16 = v15->pEntries[v14 - v15->charFirst].field_4;
                a5a.width = v16;
            }
            else
            {
LABEL_21:
                v16 = 0;
                a5a.x = 0;
                a5a.width = 0;
            }
            if ( (int)(v9 + v16) < (int)(blit_x + a5) )
            {
                stdDisplay_VBufferCopy(vbuf, a2a, v9, blit_y, &a5a, alpha_maybe);
                v13 = a5a.width + font->marginY;
                goto LABEL_26;
            }
            v18 = 1;
LABEL_27:
            v11 = v10[1];
            v8 = blit_x;
            ++v10;
            if ( !v11 )
                return v9 - v8;
        }
        v13 = font->marginX;
LABEL_26:
        v9 += v13;
        goto LABEL_27;
    }
    return v9 - v8;
}

void stdFont_Draw2(stdVBuffer *a1, stdFont *a2, unsigned int a3, int a4, rdRect *a5, wchar_t *a6, int a7)
{
    wchar_t *v8; // ebx
    unsigned int v9; // edi
    stdVBuffer *v10; // ebp
    wchar_t *v11; // eax
    int v12; // ecx
    int v13; // eax
    uint16_t v14; // cx
    stdFontCharset *v15; // eax
    int v16; // eax
    signed int v17; // eax
    int v18; // [esp+10h] [ebp-18h] BYREF
    wchar_t *v19; // [esp+14h] [ebp-14h]
    rdRect rect; // [esp+18h] [ebp-10h] BYREF
    stdVBuffer *a2a; // [esp+30h] [ebp+8h]
    int a6a; // [esp+40h] [ebp+18h]

    v8 = a6;
    v9 = a3;
    v10 = *a2->bitmap->mipSurfaces;
    rect.y = 0;
    a2a = v10;
    rect.height = v10->format.height;
    if ( a6 )
    {
        do
        {
            v11 = stdFont_sub_4352C0(v8, a2, v9, a5, &v18);
            v19 = v11;
            a6a = 0;
            if ( v18 > 0 )
            {
                do
                {
                    if ( *v8 == 9 )
                    {
                        v12 = 16;
                        if ( 8 * a2->marginX >= 16 )
                            v12 = 8 * a2->marginX;
                        v9 = a3 + v12 * ((int)(v9 + v12 - a3) / v12);
                    }
                    else
                    {
                        if ( _iswspace(*v8) )
                        {
                            v13 = a2->marginX;
                        }
                        else
                        {
                            v14 = *v8;
                            v15 = &a2->charsetHead;
                            if ( a2 != (stdFont *)-48 )
                            {
                                do
                                {
                                    if ( v14 >= v15->charFirst && v14 <= v15->charLast )
                                        break;
                                    v15 = v15->previous;
                                }
                                while ( v15 );
                                if ( v15 )
                                    goto LABEL_20;
                            }
                            v14 = a2->field_28;
                            rect.x = 0;
                            rect.width = 0;
                            v15 = &a2->charsetHead;
                            if ( a2 == (stdFont *)-48 )
                                goto LABEL_19;
                            do
                            {
                                if ( v14 >= v15->charFirst && v14 <= v15->charLast )
                                    break;
                                v15 = v15->previous;
                            }
                            while ( v15 );
                            if ( v15 )
                            {
LABEL_20:
                                v10 = a2a;
                                rect.x = v15->pEntries[v14 - v15->charFirst].field_0;
                                rect.width = v15->pEntries[v14 - v15->charFirst].field_4;
                            }
                            else
                            {
LABEL_19:
                                rect.x = 0;
                                rect.width = 0;
                            }
                            v16 = a5->y + a5->height;
                            rect.height = v10->format.height;
                            v17 = v16 - a4;
                            if ( rect.height >= v17 )
                                rect.height = v17;
                            stdDisplay_VBufferCopy(a1, v10, v9, a4, &rect, a7);
                            v13 = rect.width + a2->marginY;
                        }
                        v9 += v13;
                    }
                    ++v8;
                    ++a6a;
                }
                while ( a6a < v18 );
                v11 = v19;
            }
            if ( v11 )
            {
                v9 = a3;
                a4 += (*a2->bitmap->mipSurfaces)->format.height + a2->marginY;
            }
            v8 = v11;
        }
        while ( v11 );
    }
}

void stdFont_Draw3(stdVBuffer *paintSurface, stdFont *font, int a3, rdRect *a4, int a5, wchar_t *a6, int a7)
{
    char v7; // bl
    int v8; // esi
    stdFont *v9; // edi
    rdRect *v10; // ebp
    stdVBuffer *v11; // eax
    wchar_t *v12; // ecx
    wchar_t *v13; // eax
    int v14; // eax
    WCHAR *v15; // eax
    wchar_t *v16; // ebx
    wchar_t *v17; // ecx
    int v18; // esi
    int v19; // ecx
    WCHAR v20; // cx
    stdFontCharset *v21; // eax
    signed int v22; // ecx
    int v23; // ebx
    unsigned int v24; // esi
    int v25; // ecx
    stdFontCharset *v26; // eax
    wchar_t v27; // cx
    signed int v28; // eax
    stdVBuffer *vbuf2; // [esp+10h] [ebp-18h]
    wchar_t *v30; // [esp+14h] [ebp-14h]
    rdRect rect; // [esp+18h] [ebp-10h] BYREF

    v7 = a5;
    v8 = 0;
    if ( a5 )
    {
        v9 = font;
        v10 = a4;
        v11 = *font->bitmap->mipSurfaces;
        v12 = a6;
        rect.y = 0;
        vbuf2 = v11;
        rect.height = v11->format.height;
        if ( (a5 & 2) == 0 )
            goto LABEL_9;
        v13 = a6;
        if ( a6 )
        {
            do
            {
                v13 = stdFont_sub_4352C0(v13, v9, v10->x, v10, &font);
                ++v8;
            }
            while ( v13 );
            v12 = a6;
        }
        v14 = v10->y + (v10->height - v8 * ((*v9->bitmap->mipSurfaces)->format.height + v9->marginY)) / 2;
        a3 = v14;
        if ( (v7 & 1) != 0 )
        {
LABEL_9:
            if ( v12 )
            {
                while ( 1 )
                {
                    v15 = stdFont_sub_4352C0(v12, v9, v10->x, v10, &font);
                    v16 = a6;
                    v17 = v15;
                    v18 = 0;
                    v30 = v15;
                    a5 = 0;
                    if ( (int)font > 0 )
                    {
                        do
                        {
                            if ( *v16 == 9 )
                            {
                                v19 = 16;
                                if ( 8 * v9->marginX >= 16 )
                                    v19 = 8 * v9->marginX;
                                v18 = v19 * ((v19 + v18) / v19);
                            }
                            else if ( _iswspace(*v16) )
                            {
                                v18 += v9->marginX;
                            }
                            else
                            {
                                v20 = *v16;
                                v21 = &v9->charsetHead;
                                if ( v9 != (stdFont *)-48 )
                                {
                                    do
                                    {
                                        if ( v20 >= v21->charFirst && v20 <= v21->charLast )
                                            break;
                                        v21 = v21->previous;
                                    }
                                    while ( v21 );
                                    if ( v21 )
                                        goto LABEL_30;
                                }
                                v20 = v9->field_28;
                                rect.x = 0;
                                rect.width = 0;
                                v21 = &v9->charsetHead;
                                if ( v9 == (stdFont *)-48 )
                                    goto LABEL_29;
                                do
                                {
                                    if ( v20 >= v21->charFirst && v20 <= v21->charLast )
                                        break;
                                    v21 = v21->previous;
                                }
                                while ( v21 );
                                if ( v21 )
                                {
LABEL_30:
                                    v10 = a4;
                                    rect.x = v21->pEntries[v20 - v21->charFirst].field_0;
                                    v22 = v21->pEntries[v20 - v21->charFirst].field_4;
                                    rect.width = v22;
                                }
                                else
                                {
LABEL_29:
                                    v22 = 0;
                                    rect.x = 0;
                                    rect.width = 0;
                                }
                                v18 += v22 + v9->marginY;
                            }
                            ++v16;
                            ++a5;
                        }
                        while ( a5 < (int)font );
                        v17 = v30;
                    }
                    v23 = 0;
                    v24 = v10->x + (v10->width - v18) / 2;
                    if ( (int)font > 0 )
                    {
                        do
                        {
                            if ( *a6 == 9 )
                            {
                                v25 = 16;
                                if ( 8 * v9->marginX >= 16 )
                                    v25 = 8 * v9->marginX;
                                v24 = v25 * ((int)(v25 + v24) / v25);
                            }
                            else if ( _iswspace(*a6) )
                            {
                                v24 += v9->marginX;
                            }
                            else
                            {
                                v26 = &v9->charsetHead;
                                v27 = *a6;
                                if ( v9 != (stdFont *)-48 )
                                {
                                    do
                                    {
                                        if ( v27 >= v26->charFirst && v27 <= v26->charLast )
                                            break;
                                        v26 = v26->previous;
                                    }
                                    while ( v26 );
                                    if ( v26 )
                                        goto LABEL_52;
                                }
                                v27 = v9->field_28;
                                rect.x = 0;
                                rect.width = 0;
                                v26 = &v9->charsetHead;
                                if ( v9 == (stdFont *)-48 )
                                    goto LABEL_51;
                                do
                                {
                                    if ( v27 >= v26->charFirst && v27 <= v26->charLast )
                                        break;
                                    v26 = v26->previous;
                                }
                                while ( v26 );
                                if ( v26 )
                                {
LABEL_52:
                                    v10 = a4;
                                    rect.x = v26->pEntries[v27 - v26->charFirst].field_0;
                                    rect.width = v26->pEntries[v27 - v26->charFirst].field_4;
                                }
                                else
                                {
LABEL_51:
                                    rect.x = 0;
                                    rect.width = 0;
                                }
                                v28 = v10->y + v10->height - a3;
                                rect.height = vbuf2->format.height;
                                if (rect.height >= v28 )
                                    rect.height = v28;
                                stdDisplay_VBufferCopy(paintSurface, vbuf2, v24, a3, &rect, a7);
                                v24 += rect.width + v9->marginY;
                            }
                            ++v23;
                            ++a6;
                        }
                        while ( v23 < (int)font );
                        v17 = v30;
                    }
                    if ( v17 )
                        a3 += (*v9->bitmap->mipSurfaces)->format.height + v9->marginY;
                    a6 = v17;
                    if ( !v17 )
                        break;
                    v12 = a6;
                }
            }
        }
        else
        {
            stdFont_Draw2(paintSurface, v9, v10->x, v14, v10, v12, a7);
        }
    }
    else
    {
        stdFont_Draw2(paintSurface, font, a4->x, a3, a4, a6, a7);
    }
}

int stdFont_Draw4(stdVBuffer *a1, stdFont *font, int xPos, int yPos, int a5, int a6, int a7, wchar_t *text, int alpha_maybe)
{
    int v9; // ebp
    wchar_t *v10; // edi
    int i; // ebx
    uint16_t v12; // cx
    stdFontCharset *v13; // eax
    int v14; // ecx
    int v15; // ecx
    int v17; // [esp+10h] [ebp-8h]
    int v18; // [esp+14h] [ebp-4h]

    v9 = 0;
    v17 = 0;
    v18 = 0;
    if ( a7 )
    {
        v10 = text;
        for ( i = 0x7FFFFFFF; *v10; --i )
        {
            if ( i <= 0 )
                break;
            if ( _iswspace(*v10) )
            {
                v9 += font->marginX;
            }
            else
            {
                v12 = *v10;
                v13 = &font->charsetHead;
                if ( font != (stdFont *)-48 )
                {
                    do
                    {
                        if ( v12 >= v13->charFirst && v12 <= v13->charLast )
                            break;
                        v13 = v13->previous;
                    }
                    while ( v13 );
                    if ( v13 )
                        goto LABEL_17;
                }
                v12 = font->field_28;
                v13 = &font->charsetHead;
                if ( font == (stdFont *)-48 )
                    goto LABEL_16;
                do
                {
                    if ( v12 >= v13->charFirst && v12 <= v13->charLast )
                        break;
                    v13 = v13->previous;
                }
                while ( v13 );
                if ( v13 )
LABEL_17:
                    v14 = v13->pEntries[v12 - v13->charFirst].field_4;
                else
LABEL_16:
                    v14 = 0;
                v9 += v14 + font->marginY;
            }
            ++v10;
        }
        v15 = a5;
        if ( (a7 & 1) != 0 )
        {
            v17 = (a5 - v9) / 2;
            if ( v17 < 0 )
                v17 = 0;
        }
        if ( (a7 & 2) != 0 )
        {
            v18 = (a6 - (*font->bitmap->mipSurfaces)->format.height) / 2;
            if ( v18 < 0 )
                v18 = 0;
        }
    }
    else
    {
        v15 = a5;
    }
    return stdFont_Draw1(a1, font, xPos + v17, yPos + v18, v15 - v17, text, alpha_maybe);
}

WCHAR* stdFont_sub_4352C0(WCHAR *a1, stdFont *a2, int a3, rdRect *a4, int *a5)
{
    WCHAR *v5; // ebp
    int v6; // eax
    WCHAR v7; // si
    int v8; // edi
    int v9; // zf
    WCHAR v10; // si
    int v11; // ecx
    uint16_t v12; // cx
    stdFontCharset *v13; // eax
    int v14; // ecx
    int v15; // edx
    WCHAR v16; // ax
    int v17; // ecx
    uint16_t v18; // cx
    stdFontCharset *v19; // eax
    int v20; // ecx
    int v22; // [esp+10h] [ebp-Ch]
    int v23; // [esp+14h] [ebp-8h]
    WCHAR *v24; // [esp+18h] [ebp-4h]
    int v25; // [esp+20h] [ebp+4h]

    v5 = a1;
    v6 = 0;
    v7 = *a1;
    v8 = 0;
    v9 = *a1 == 0;
    v24 = a1;
    v22 = 0;
    v23 = 0;
    v25 = 0;
    if ( !v9 )
    {
        while ( 1 )
        {
            if ( _iswspace(v7) || v7 == 8232 || v7 == 8233 )
            {
                if ( v5 )
                {
                    while ( 1 )
                    {
                        if ( v8 + a3 + v22 >= a4->width + a4->x )
                        {
                            v6 = v23;
                            if ( v23 )
                                break;
                        }
                        v10 = *v5;
                        if ( _iswspace(*v5) || v10 == 8232 || v10 == 8233 )
                        {
                            if ( *v5 == '\t' )
                            {
                                v11 = 8 * a2->marginX;
                                if ( v11 < 16 )
                                    v11 = 16;
                                v8 += a3 + v11 * ((v8 + v11 + v22) / v11);
                            }
                            else if ( _iswspace(*v5) )
                            {
                                v8 += a2->marginX;
                            }
                            else
                            {
                                v12 = *v5;
                                v13 = &a2->charsetHead;
                                if ( a2 != (stdFont *)-48 )
                                {
                                    do
                                    {
                                        if ( v12 >= v13->charFirst && v12 <= v13->charLast )
                                            break;
                                        v13 = v13->previous;
                                    }
                                    while ( v13 );
                                    if ( v13 )
                                        goto LABEL_28;
                                }
                                v12 = a2->field_28;
                                v13 = &a2->charsetHead;
                                if ( a2 == (stdFont *)-48 )
                                    goto LABEL_27;
                                do
                                {
                                    if ( v12 >= v13->charFirst && v12 <= v13->charLast )
                                        break;
                                    v13 = v13->previous;
                                }
                                while ( v13 );
                                if ( v13 )
LABEL_28:
                                    v14 = v13->pEntries[v12 - v13->charFirst].field_4;
                                else
LABEL_27:
                                    v14 = 0;
                                v8 += v14 + a2->marginY;
                            }
                            v9 = ++v5 == 0;
                            ++v25;
                            if ( !v9 )
                                continue;
                        }
                        goto LABEL_31;
                    }
LABEL_57:
                    *a5 = v6;
                    return v24;
                }
LABEL_31:
                v15 = v8 + v22;
                v8 = 0;
                v22 = v15;
                v23 += v25;
                v24 = v5;
                v25 = 0;
            }
            v16 = *v5;
            if ( *v5 )
            {
                if ( v16 == 9 )
                {
                    v17 = 8 * a2->marginX;
                    if ( v17 < 16 )
                        v17 = 16;
                    v8 += a3 + v17 * ((v8 + v17 + v22) / v17);
                }
                else if ( _iswspace(v16) )
                {
                    v8 += a2->marginX;
                }
                else
                {
                    v18 = *v5;
                    v19 = &a2->charsetHead;
                    if ( a2 != (stdFont *)-48 )
                    {
                        do
                        {
                            if ( v18 >= v19->charFirst && v18 <= v19->charLast )
                                break;
                            v19 = v19->previous;
                        }
                        while ( v19 );
                        if ( v19 )
                            goto LABEL_50;
                    }
                    v18 = a2->field_28;
                    v19 = &a2->charsetHead;
                    if ( a2 == (stdFont *)-48 )
                        goto LABEL_49;
                    do
                    {
                        if ( v18 >= v19->charFirst && v18 <= v19->charLast )
                            break;
                        v19 = v19->previous;
                    }
                    while ( v19 );
                    if ( v19 )
LABEL_50:
                        v20 = v19->pEntries[v18 - v19->charFirst].field_4;
                    else
LABEL_49:
                        v20 = 0;
                    v8 += v20 + a2->marginY;
                }
                ++v5;
                ++v25;
            }
            v7 = *v5;
            if ( !*v5 )
            {
                v6 = v23;
                break;
            }
        }
    }
    if ( a3 + v8 + v22 >= a4->x + a4->width && v6 )
        goto LABEL_57;
    *a5 = v25 + v6;
    return 0;
}

int stdFont_sub_4357C0(stdFont *a1, wchar_t *a2, rdRect *a4)
{
    wchar_t *v3; // eax
    int v4; // edi
    rdRect *v5; // esi

    v3 = a2;
    v4 = 0;
    if ( a2 )
    {
        v5 = a4;
        do
        {
            v3 = stdFont_sub_4352C0(v3, a1, v5->x, v5, (int *)&a2);
            ++v4;
        }
        while ( v3 );
    }
    return v4 * ((*a1->bitmap->mipSurfaces)->format.height + a1->marginY);
}

int stdFont_sub_435810(stdFont *a1, wchar_t *a2, int a3)
{
    wchar_t *v3; // edi
    int v4; // ebp
    wchar_t i; // ax
    uint16_t v7; // cx
    stdFontCharset *v8; // eax
    int v9; // ecx

    v3 = a2;
    v4 = 0;
    for ( i = *a2; i; --a3 )
    {
        if ( a3 <= 0 )
            break;
        if ( _iswspace(i) )
        {
            v4 += a1->marginX;
        }
        else
        {
            v7 = *v3;
            v8 = &a1->charsetHead;
            if ( a1 != (stdFont *)-48 )
            {
                do
                {
                    if ( v7 >= v8->charFirst && v7 <= v8->charLast )
                        break;
                    v8 = v8->previous;
                }
                while ( v8 );
                if ( v8 )
                    goto LABEL_16;
            }
            v7 = a1->field_28;
            v8 = &a1->charsetHead;
            if ( a1 == (stdFont *)-48 )
                goto LABEL_15;
            do
            {
                if ( v7 >= v8->charFirst && v7 <= v8->charLast )
                    break;
                v8 = v8->previous;
            }
            while ( v8 );
            if ( v8 )
LABEL_16:
                v9 = v8->pEntries[v7 - v8->charFirst].field_4;
            else
LABEL_15:
                v9 = 0;
            v4 += v9 + a1->marginY;
        }
        i = v3[1];
        ++v3;
    }
    return v4;
}

int stdFont_sub_434EC0(stdVBuffer *a1, stdFont *a2, int a3, int a4, int a5, int *a6, wchar_t *a7, int a8)
{
    int *v8; // ebp
    int v9; // edi
    wchar_t *v11; // ebx
    wchar_t *v12; // eax
    wchar_t *v13; // esi
    wchar_t v14; // cx
    int v15; // ecx
    int i; // eax
    int v17; // eax
    int v18; // [esp+10h] [ebp-4h]
    uint16_t a7_;

    v8 = a6;
    v9 = 0;
    v18 = 0;
    if ( !a6 )
        return stdFont_Draw1(a1, a2, a3, a4, a5, a7, a8);
    v11 = a7;
    while ( *v11 )
    {
        v12 = __wcsrchr(v11, 9u);
        v13 = v12;
        if ( v12 )
        {
            v14 = *v12;
            *v12 = 0;
            a7_ = v14;
        }
        if ( v9 + a3 >= (signed int)a1->format.width )
            break;
        v9 += stdFont_Draw1(a1, a2, v9 + a3, a4, a5 - v9, v11, a8);
        if ( v13 )
        {
            v11 = v13 + 1;
            *v13 = a7_;
            v15 = v18;
        }
        else
        {
            v15 = 1;
            v18 = 1;
        }
        if ( v15 )
            break;
        if ( *v11 )
        {
            for ( i = *v8; i; ++v8 )
            {
                if ( v9 <= i )
                    break;
                i = v8[1];
            }
            if ( *v8 )
            {
                v9 = *v8;
            }
            else
            {
                v17 = 8 * a2->marginX;
                if ( v17 <= 16 )
                    v17 = 16;
                v9 += v17;
            }
        }
    }
    return v9;
}

void stdFont_Free(stdFont *font)
{
    stdFontCharset *v1; // esi
    stdFontCharset *v2; // edi

    // Added
    if (!font)
        return;

    if ( font->bitmap )
        stdBitmap_Free(font->bitmap);
    v1 = font->charsetHead.previous;
    std_pHS->free(font);
    if ( v1 )
    {
        do
        {
            v2 = v1->previous;
            std_pHS->free(v1);
            v1 = v2;
        }
        while ( v2 );
    }
}
