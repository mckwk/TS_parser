#include "tsCommon.h"
#include "tsTransportStream.h"
using namespace std;

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  //TODO - open file
  FILE* stream = std::fopen("/home/user/repos/TS_parser/tsTransportStream.h", "rb");

  //TODO - check if file if opened
  if (!stream) {
      return EXIT_FAILURE;
  }

  xTS_PacketHeader TS_PacketHeader;

  const uint8_t size = 188;
  uint8_t  buf[size];
  int32_t TS_PacketId = 0;


  while(!feof(stream))
  {
    //TODO - read from file
    fread(buf, 1, 188, stream);

    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(buf);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf("\n");

    TS_PacketId++;
  }

  //TODO - close file
  fclose(stream);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
