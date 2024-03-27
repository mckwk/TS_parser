

#include "tsCommon.h"
#include "tsTransportStream.h"
#include <vector>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  // TODO - open file
  int is_ok = EXIT_FAILURE;
  FILE* stream = fopen("/home/student/Downloads/parser/tsTransportStream.h", "rb");
  // TODO - check if file if opened
  if (!stream)
    {
        std::perror("File opening failed");
        return is_ok;
    }

  xTS_PacketHeader    TS_PacketHeader;

  int32_t TS_PacketId = 0;
  while(!feof(stream)){
    
    // TODO - read from file
    const uint8_t size = 188;
    uint8_t buf[size]; 
    fread(buf, 1, 188, stream);
    

    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(/*TS_PacketBuffer*/ nullptr);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf("\n");

    TS_PacketId++;
  }
  
  // TODO - close file
  fclose(stream);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================

