#ifndef _SHELL_H_INCLUDED_
#define _SHELL_H_INCLUDED_

namespace fsh
{
    void PushWord(char *w);
    void PushBar();
    void PushLT();
    void PushGT();
    void PushGTGT();
    void PushAmp();
    void PushSemi();
    void PushNL();
}

#endif
