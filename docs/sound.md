# Raylib Audio: Sound vs. Music

## Sound
Purpose: Short audio clips (clicks, explosions, UI effects, etc).
Format: Use `.wav`
Why: uncompressed, high quality, low CPU overhead, fast loading for frequent playback.

## Music
Purpose: Longer audio tracks (background music, ambient loops, etc).
Format: Use `.ogg`
Why: compressed, smaller file size, efficient for streaming.

## Others
Initialize audio with `InitAudioDevice()` before loading anything.
Unload sounds `UnloadSound()` and music `UnloadMusicStream()` when done.
`.mp3` less preferred due to decoding complexity and licensing in raylib.
