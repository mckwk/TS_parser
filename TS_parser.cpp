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
  xPES_Assembler PES_Assembler;
  xPES_PacketHeader PES_PacketHeader;

  const uint8_t size = 188;
  uint8_t  buf[size];
  int32_t TS_PacketId = 0;


  //while(!feof(stream))
  while(TS_PacketId<=40)
  {
printf("\n");
    fread(buf, 1, 188, stream);

    TS_PacketHeader.Reset();
    TS_AdaptationField.Reset();
    TS_PacketHeader.Parse(buf);

    printf("%010d ", TS_PacketId);
  if(TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPID() == 136)
  {
    if(TS_PacketHeader.hasAdaptationField()){
      TS_AdaptationField.Parse(buf);
    }
    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    if(TS_PacketHeader.hasAdaptationField()) { TS_AdaptationField.Print(); }
    if (TS_PacketHeader.getPayload() == 1) {
        PES_PacketHeader.Parse(buf, TS_AdaptationField.getAdaptationFieldLength());
        PES_PacketHeader.Print();}
    if (TS_PacketHeader.getContinuity() == 15) {
                PES_PacketHeader.PrintLen();
            }
            PES_Assembler.Parse(buf, TS_PacketHeader, TS_AdaptationField);
  }
    TS_PacketId++;
  }

  fclose(stream);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
