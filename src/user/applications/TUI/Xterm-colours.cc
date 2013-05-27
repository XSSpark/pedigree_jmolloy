/*
 * Copyright (c) 2013 James Molloy, Jörg Pfähler, Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdint.h>

uint32_t g_Colours[] =
{
    /*
    0x000000,
    0xB02222,
    0x22B022,
    0xB0B022,
    0x2222B0,
    0xB022B0,
    0x22B0B0,
    0xF0F0F0,
    0xFFFFFF,
    */

    0x000000,
    0x800000,
    0x008000,
    0x808000,
    0x000080,
    0x800080,
    0x008080,
    0xc0c0c0,
    0x000000,
    0x800000,
    0x008000,
    0x808000,
    0x000080,
    0x800080,
    0x008080,
    0xc0c0c0,
    0x808080,
    0xff0000,
    0x00ff00,
    0xffff00,
    0x0000ff,
    0xff00ff,
    0x00ffff,
    0xffffff,

    // Begin 256-colour set
    0x000000,
    0x00005F,
    0x000087,
    0x0000AF,
    0x0000D7,
    0x0000FF,
    0x005F00,
    0x005F5F,
    0x005F87,
    0x005FAF,
    0x005FD7,
    0x005FFF,
    0x008700,
    0x00875F,
    0x008787,
    0x0087AF,
    0x0087D7,
    0x0087FF,
    0x00AF00,
    0x00AF5F,
    0x00AF87,
    0x00AFAF,
    0x00AFD7,
    0x00AFFF,
    0x00D700,
    0x00D75F,
    0x00D787,
    0x00D7AF,
    0x00D7D7,
    0x00D7FF,
    0x00FF00,
    0x00FF5F,
    0x00FF87,
    0x00FFAF,
    0x00FFD7,
    0x00FFFF,
    0x5FFF00,
    0x5FFF5F,
    0x5FFF87,
    0x5FFFAF,
    0x5FFFD7,
    0x5FFFFF,
    0x5FD700,
    0x5FD75F,
    0x5FD787,
    0x5FD7AF,
    0x5FD7D7,
    0x5FD7FF,
    0x5FAF00,
    0x5FAF5F,
    0x5FAF87,
    0x5FAFAF,
    0x5FAFD7,
    0x5FAFFF,
    0x5F8700,
    0x5F875F,
    0x5F8787,
    0x5F87AF,
    0x5F87D7,
    0x5F87FF,
    0x5F5F00,
    0x5F5F5F,
    0x5F5F87,
    0x5F5FAF,
    0x5F5FD7,
    0x5F5FFF,
    0x5F0000,
    0x5F005F,
    0x5F0087,
    0x5F00AF,
    0x5F00D7,
    0x5F00FF,
    0x8700FF,
    0x8700D7,
    0x8700AF,
    0x870087,
    0x87005F,
    0x870000,
    0x875FFF,
    0x875FD7,
    0x875FAF,
    0x875F87,
    0x875F5F,
    0x875F00,
    0x8787FF,
    0x8787D7,
    0x8787AF,
    0x878787,
    0x87875F,
    0x878700,
    0x87AFFF,
    0x87AFD7,
    0x87AFAF,
    0x87AF87,
    0x87AF5F,
    0x87AF00,
    0x87D7FF,
    0x87D7D7,
    0x87D7AF,
    0x87D787,
    0x87D75F,
    0x87D700,
    0x87FFFF,
    0x87FFD7,
    0x87FFAF,
    0x87FF87,
    0x87FF5F,
    0x87FF00,
    0xAFFFFF,
    0xAFFFD7,
    0xAFFFAF,
    0xAFFF87,
    0xAFFF5F,
    0xAFFF00,
    0xAFD7FF,
    0xAFD7D7,
    0xAFD7AF,
    0xAFD787,
    0xAFD75F,
    0xAFD700,
    0xAFAFFF,
    0xAFAFD7,
    0xAFAFAF,
    0xAFAF87,
    0xAFAF5F,
    0xAFAF00,
    0xAF87FF,
    0xAF87D7,
    0xAF87AF,
    0xAF8787,
    0xAF875F,
    0xAF8700,
    0xAF5FFF,
    0xAF5FD7,
    0xAF5FAF,
    0xAF5F87,
    0xAF5F5F,
    0xAF5F00,
    0xAF00FF,
    0xAF00D7,
    0xAF00AF,
    0xAF0087,
    0xAF005F,
    0xAF0000,
    0xD70000,
    0xD7005F,
    0xD70087,
    0xD700AF,
    0xD700D7,
    0xD700FF,
    0xD75F00,
    0xD75F5F,
    0xD75F87,
    0xD75FAF,
    0xD75FD7,
    0xD75FFF,
    0xD78700,
    0xD7875F,
    0xD78787,
    0xD787AF,
    0xD787D7,
    0xD787FF,
    0xDFAF00,
    0xDFAF5F,
    0xDFAF87,
    0xDFAFAF,
    0xDFAFDF,
    0xDFAFFF,
    0xDFDF00,
    0xDFDF5F,
    0xDFDF87,
    0xDFDFAF,
    0xDFDFDF,
    0xDFDFFF,
    0xDFFF00,
    0xDFFF5F,
    0xDFFF87,
    0xDFFFAF,
    0xDFFFDF,
    0xDFFFFF,
    0xFFFF00,
    0xFFFF5F,
    0xFFFF87,
    0xFFFFAF,
    0xFFFFDF,
    0xFFFFFF,
    0xFFDF00,
    0xFFDF5F,
    0xFFDF87,
    0xFFDFAF,
    0xFFDFDF,
    0xFFDFFF,
    0xFFAF00,
    0xFFAF5F,
    0xFFAF87,
    0xFFAFAF,
    0xFFAFDF,
    0xFFAFFF,
    0xFF8700,
    0xFF875F,
    0xFF8787,
    0xFF87AF,
    0xFF87DF,
    0xFF87FF,
    0xFF5F00,
    0xFF5F5F,
    0xFF5F87,
    0xFF5FAF,
    0xFF5FDF,
    0xFF5FFF,
    0xFF0000,
    0xFF005F,
    0xFF0087,
    0xFF00AF,
    0xFF00DF,
    0xFF00FF
};

uint32_t g_BrightColours[] =
{
    0x333333,
    0xFF3333,
    0x33FF33,
    0xFFFF33,
    0x3333FF,
    0xFF33FF,
    0x33FFFF,
    0xFFFFFF,

    // Begin 256-colour set (NOT ACTUALLY BRIGHT COLOURS)
    0x000000,
    0x00005F,
    0x000087,
    0x0000AF,
    0x0000D7,
    0x0000FF,
    0x005F00,
    0x005F5F,
    0x005F87,
    0x005FAF,
    0x005FD7,
    0x005FFF,
    0x008700,
    0x00875F,
    0x008787,
    0x0087AF,
    0x0087D7,
    0x0087FF,
    0x00AF00,
    0x00AF5F,
    0x00AF87,
    0x00AFAF,
    0x00AFD7,
    0x00AFFF,
    0x00D700,
    0x00D75F,
    0x00D787,
    0x00D7AF,
    0x00D7D7,
    0x00D7FF,
    0x00FF00,
    0x00FF5F,
    0x00FF87,
    0x00FFAF,
    0x00FFD7,
    0x00FFFF,
    0x5FFF00,
    0x5FFF5F,
    0x5FFF87,
    0x5FFFAF,
    0x5FFFD7,
    0x5FFFFF,
    0x5FD700,
    0x5FD75F,
    0x5FD787,
    0x5FD7AF,
    0x5FD7D7,
    0x5FD7FF,
    0x5FAF00,
    0x5FAF5F,
    0x5FAF87,
    0x5FAFAF,
    0x5FAFD7,
    0x5FAFFF,
    0x5F8700,
    0x5F875F,
    0x5F8787,
    0x5F87AF,
    0x5F87D7,
    0x5F87FF,
    0x5F5F00,
    0x5F5F5F,
    0x5F5F87,
    0x5F5FAF,
    0x5F5FD7,
    0x5F5FFF,
    0x5F0000,
    0x5F005F,
    0x5F0087,
    0x5F00AF,
    0x5F00D7,
    0x5F00FF,
    0x8700FF,
    0x8700D7,
    0x8700AF,
    0x870087,
    0x87005F,
    0x870000,
    0x875FFF,
    0x875FD7,
    0x875FAF,
    0x875F87,
    0x875F5F,
    0x875F00,
    0x8787FF,
    0x8787D7,
    0x8787AF,
    0x878787,
    0x87875F,
    0x878700,
    0x87AFFF,
    0x87AFD7,
    0x87AFAF,
    0x87AF87,
    0x87AF5F,
    0x87AF00,
    0x87D7FF,
    0x87D7D7,
    0x87D7AF,
    0x87D787,
    0x87D75F,
    0x87D700,
    0x87FFFF,
    0x87FFD7,
    0x87FFAF,
    0x87FF87,
    0x87FF5F,
    0x87FF00,
    0xAFFFFF,
    0xAFFFD7,
    0xAFFFAF,
    0xAFFF87,
    0xAFFF5F,
    0xAFFF00,
    0xAFD7FF,
    0xAFD7D7,
    0xAFD7AF,
    0xAFD787,
    0xAFD75F,
    0xAFD700,
    0xAFAFFF,
    0xAFAFD7,
    0xAFAFAF,
    0xAFAF87,
    0xAFAF5F,
    0xAFAF00,
    0xAF87FF,
    0xAF87D7,
    0xAF87AF,
    0xAF8787,
    0xAF875F,
    0xAF8700,
    0xAF5FFF,
    0xAF5FD7,
    0xAF5FAF,
    0xAF5F87,
    0xAF5F5F,
    0xAF5F00,
    0xAF00FF,
    0xAF00D7,
    0xAF00AF,
    0xAF0087,
    0xAF005F,
    0xAF0000,
    0xD70000,
    0xD7005F,
    0xD70087,
    0xD700AF,
    0xD700D7,
    0xD700FF,
    0xD75F00,
    0xD75F5F,
    0xD75F87,
    0xD75FAF,
    0xD75FD7,
    0xD75FFF,
    0xD78700,
    0xD7875F,
    0xD78787,
    0xD787AF,
    0xD787D7,
    0xD787FF,
    0xDFAF00,
    0xDFAF5F,
    0xDFAF87,
    0xDFAFAF,
    0xDFAFDF,
    0xDFAFFF,
    0xDFDF00,
    0xDFDF5F,
    0xDFDF87,
    0xDFDFAF,
    0xDFDFDF,
    0xDFDFFF,
    0xDFFF00,
    0xDFFF5F,
    0xDFFF87,
    0xDFFFAF,
    0xDFFFDF,
    0xDFFFFF,
    0xFFFF00,
    0xFFFF5F,
    0xFFFF87,
    0xFFFFAF,
    0xFFFFDF,
    0xFFFFFF,
    0xFFDF00,
    0xFFDF5F,
    0xFFDF87,
    0xFFDFAF,
    0xFFDFDF,
    0xFFDFFF,
    0xFFAF00,
    0xFFAF5F,
    0xFFAF87,
    0xFFAFAF,
    0xFFAFDF,
    0xFFAFFF,
    0xFF8700,
    0xFF875F,
    0xFF8787,
    0xFF87AF,
    0xFF87DF,
    0xFF87FF,
    0xFF5F00,
    0xFF5F5F,
    0xFF5F87,
    0xFF5FAF,
    0xFF5FDF,
    0xFF5FFF,
    0xFF0000,
    0xFF005F,
    0xFF0087,
    0xFF00AF,
    0xFF00DF,
    0xFF00FF
};
