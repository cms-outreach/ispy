#ifndef IG_TOKENIZER_H
#define IG_TOKENIZER_H

#include <iostream>

bool
fgettoken(std::istream &in, char **buffer, size_t *maxSize, 
          const char *separators, int *firstChar);

#endif