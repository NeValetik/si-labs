#pragma once
#include "interfaces/IStream.h"

// IStream implementation that routes:
//   write() -> LCD (via LcdDriver)
//   read()  -> Keypad (via KeypadDriver)
class LcdStream : public IStream {
public:
    virtual void write(char c) override;
    virtual char read()        override;
    virtual bool available()   override;
};
