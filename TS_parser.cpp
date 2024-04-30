#include "tsCommon.h"
#include "tsTransportStream.h"
using namespace std;

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  //TODO - open file
  FILE* stream = std::fopen("../example_new.ts", "rb");

  //TODO - check if file if opened
  if (!stream) {
      return EXIT_FAILURE;
  }

  xTS_PacketHeader TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;

  const uint8_t size = 188;
  uint8_t  buf[size];
  int32_t TS_PacketId = 0;


  //while(!feof(stream))
  while(TS_PacketId<=32)
  {
    //TODO - read from file
    fread(buf, 1, 188, stream);

    TS_PacketHeader.Reset();
    TS_AdaptationField.Reset();
    TS_PacketHeader.Parse(buf);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    if(TS_PacketHeader.hasAdaptationField()==true) {
      TS_AdaptationField.Parse(buf);
      TS_AdaptationField.Print();
    }
    printf("\n");

    TS_PacketId++;
  }

  //TODO - close file
  fclose(stream);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
