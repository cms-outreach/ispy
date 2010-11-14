#include "Framework/IgTokenizer.h"
#include <cstring>
#include <cstdlib>

/** Helper function which gets a token delimited by @a separator from the 
    @a file and write it, 0 terminated in the buffer found in @a buffer.
    
    Notice that if the token is larger than @a maxSize, the buffer is
    reallocated and @a maxSize is updated to the new size.

    The trailing separator after a token is not put in the token and is left 
    in the buffer. If @a nextChar is not 0, the delimiter is put there.
    
    @a in the input stream to be parsed.
    
    @a buffer a pointer to the buffer where to put the tokens. The buffer will
     be redimensioned accordingly, if the token is larger of the buffer.
     
    @a maxSize, a pointer to the size of the buffer. Notice that in case the 
     buffer is reallocated to have more space, maxSize is updated with the new 
     size.
     
    @a firstChar a pointer with the first character in the buffer, notice
                 that the first charater in the stream must be obtained 
                 separately!!!
    
    @return whether or not we were able to get a (possibly empty) token from
            the file.
  */
bool
fgettoken(std::istream &in, char **buffer, size_t *maxSize, 
          const char *separators, int *firstChar)
{
  // if the passed first character is EOF or a separator,
  // return an empty otherwise use it as first character
  // of the buffer. 
  if (*firstChar == EOF || (int) separators[0] == *firstChar || strchr(separators + 1, *firstChar))
  {
    (*buffer)[0] = 0;
    return true;
  }
  else
    (*buffer)[0] = (char) *firstChar;

  size_t i = 1;

  while (true)
  {
    if (i >= *maxSize)
    {
      *maxSize += 1024;
      *buffer = (char*) realloc(*buffer, *maxSize);
      if (!*buffer)
         return false;
    }

    int c = in.get();

    if (c == EOF)
    {
       (*buffer)[i] = 0;
       *firstChar = c;
       return false;
    }
    
    if (separators[0] == c || strchr(separators + 1, c))
    {
      (*buffer)[i] = 0;
      *firstChar = c;
      return true;
    }

    (*buffer)[i++] = (char) c;
  }
}
