#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>
#include <cstdio>
using namespace std;

//=============================================================================================================================================================================

int main(int argc, char *argv[], char *envp[]) {
    const char* inputFilePath = "../example_new.ts";
    FILE* stream = fopen(inputFilePath, "rb");

    if (!stream) {
        cerr << "Failed to open input file " << inputFilePath << endl;
        return EXIT_FAILURE;
    }

    xTS_PacketHeader TS_PacketHeader;
    xTS_AdaptationField TS_AdaptationField;
    xPES_Assembler PES_Assembler;
    xPES_PacketHeader PES_PacketHeader;

    const uint8_t size = 188;
    uint8_t buf[size];
    int32_t TS_PacketId = 0;

    while (!feof(stream)) {
        printf("\n");
        fread(buf, 1, size, stream);
        
        TS_PacketHeader.Reset();
        TS_AdaptationField.Reset();
        TS_PacketHeader.Parse(buf);

        printf("%010d ", TS_PacketId);
        TS_PacketHeader.Print();

        if (TS_PacketHeader.getSyncByte() == 'G' && TS_PacketHeader.getPID() == 136) {
            if (TS_PacketHeader.hasAdaptationField()) {
                TS_AdaptationField.Parse(buf);
                TS_AdaptationField.Print();
            }

            if (TS_PacketHeader.getPayload() == 1) {
                PES_PacketHeader.Parse(buf, TS_AdaptationField.getAdaptationFieldLength());
                PES_PacketHeader.Print();
            }

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
