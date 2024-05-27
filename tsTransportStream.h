#pragma once
#include "tsCommon.h"
#include <string>

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS
{
public:
  static constexpr uint32_t TS_PacketLength  = 188;
  static constexpr uint32_t TS_HeaderLength  = 4;

  static constexpr uint32_t PES_HeaderLength = 6;

  static constexpr uint32_t BaseClockFrequency_Hz         =    90000; //Hz
  static constexpr uint32_t ExtendedClockFrequency_Hz     = 27000000; //Hz
  static constexpr uint32_t BaseClockFrequency_kHz        =       90; //kHz
  static constexpr uint32_t ExtendedClockFrequency_kHz    =    27000; //kHz
  static constexpr uint32_t BaseToExtendedClockMultiplier =      300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
  enum class ePID : uint16_t
  {
    PAT  = 0x0000,
    CAT  = 0x0001,
    TSDT = 0x0002,
    IPMT = 0x0003,
    NIT  = 0x0010, //DVB specific PID
    SDT  = 0x0011, //DVB specific PID
    NuLL = 0x1FFF,
  };

protected:
  uint8_t   m_SB;   //sync byte 8b
  uint8_t   m_TEI;  //transport error indicator 1b
  uint8_t   m_PSI;  //payload unit start indicator 1b
  uint8_t   m_TP;   //transport priority 1b
  uint16_t  m_PID;  //13b
  uint8_t   m_TSC;  //transport scrambling control 2b
  uint8_t   m_AFC;  //adaptation field control 2b
  uint8_t   m_CC;   //continuity counter 3b

public:
  void     Reset();
  int32_t  Parse(const uint8_t* Input);
  void     Print() const;

public:
  uint8_t  getSyncByte() const { return m_SB; }  

public:
  bool     hasAdaptationField() const {
    if (m_AFC == 2 || m_AFC ==3) { return true;
    } else return false;}
  uint32_t getPayload() const { return m_PSI; }
  int getPID() {return m_PID;}
  uint32_t getAdaptationFieldControl() const { return m_AFC; }
  uint32_t getContinuity() const { return m_CC; }
};

//=============================================================================================================================================================================

class xTS_AdaptationField {

  protected:
    uint8_t m_AdaptationFieldControl;
    uint8_t m_AdaptationFieldLength;

    uint8_t m_DI;
    uint8_t m_RAI;
    uint32_t m_ESPI;
    uint32_t m_PC;
    uint32_t m_OPC;
    uint32_t m_SPF;
    uint32_t m_TPDF;
    uint32_t m_AFEF;

  public:
  void Reset();
  int32_t Parse(const uint8_t* PacketBuffer);
  void Print() const;

  uint8_t getAdaptationFieldLength() const {
    return m_AdaptationFieldLength;
  }

  uint8_t getAdaptationFieldControl() const {
    return m_AdaptationFieldControl;
  }

};

//=============================================================================================================================================================================

class xPES_PacketHeader
{
public:
    enum eStreamId : uint8_t {
        eStreamId_program_stream_map = 0xBC,
        eStreamId_padding_stream = 0xBE,
        eStreamId_private_stream_2 = 0xBF,
        eStreamId_ECM = 0xF0,
        eStreamId_EMM = 0xF1,
        eStreamId_program_stream_directory = 0xFF,
        eStreamId_DSMCC_stream = 0xF2,
        eStreamId_ITUT_H222_1_type_E = 0xF8,
    };
protected:
    uint32_t m_PacketStartCodePrefix;
    uint8_t m_StreamId;
    uint16_t m_PacketLength;
public:
    xPES_PacketHeader();
    void Reset();
    int32_t Parse(const uint8_t* Input, uint32_t GlobalAFL);
    void Print() const;
    void PrintLen() const;
public:
    //PES packet header
    uint32_t getPacketStartCodePrefix() const { return m_PacketStartCodePrefix; }
    uint8_t getStreamId() const { return m_StreamId; }
    uint16_t getPacketLength() const { return m_PacketLength; }
};

//=============================================================================================================================================================================

class xPES_Assembler
{
public:
    enum class eResult : int32_t {
        UnexpectedPID = 1,
        StreamPackedLost,
        AssemblingStarted,
        AssemblingContinue,
        AssemblingFinished,
    };
protected:
    int32_t m_PID;

    uint8_t* m_Buffer;
    uint32_t m_BufferSize;
    uint32_t m_DataOffset;

    int8_t m_LastContinuityCounter;
    bool m_Started;
    xPES_PacketHeader m_PESH;
public:
    int32_t Parse(const uint8_t* Input, const xTS_PacketHeader PacketHeader, const xTS_AdaptationField AdaptationField);
    uint8_t* getPacket() { return m_Buffer; }
    int32_t getNumPacketBytes() const { return m_DataOffset; }
};