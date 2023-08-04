#ifndef PLUG_H
#define PLUG_H

extern "C" {
    typedef struct {

    } Plug;

    void plug_hello(void);
    void plug_frame(Plug *plug);
}

#endif // PLUG_H
