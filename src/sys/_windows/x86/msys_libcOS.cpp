//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//


static short opc1 = 0x043f ;     // floor

void msys_memcpy( void *dst, const void *ori, int amount )
{
    _asm mov edi, dst
    _asm mov esi, ori
    _asm mov ecx, amount
    _asm rep movsb
}

