#ifndef VSH_H
#define VSH_H
/**
 * Handles vsh's main loop logic. This should probably be the only function
 * called by the client. It's modular and self-contained, so there's no need
 * for parameters or return values.
 */
void vsh_mainLoop();

#endif
