#include <iostream>
#include "tsTransportStream.h"
using namespace std;

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset() {
    m_SB = m_TEI = m_PSI = m_TP = m_PID = m_TSC = m_AFC = m_CC = 0;
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t *Input) {
    uint32_t *HeaderPtr = (uint32_t *) Input;
    uint32_t HeaderData = xSwapBytes32(*HeaderPtr);

    uint32_t b_TEI =  0b00000000100000000000000000000000;
    uint32_t b_PSI =  0b00000000010000000000000000000000;
    uint32_t b_TP =   0b00000000001000000000000000000000;
    uint32_t b_PID =  0b00000000000111111111111100000000;
    uint32_t b_TSC =  0b00000000000000000000000011000000;
    uint32_t b_AFC =  0b00000000000000000000000000110000;
    uint32_t b_CC =   0b00000000000000000000000000001111;

    m_SB = HeaderData >> 24;
    m_TEI = (HeaderData & b_TEI) >> 23;
    m_PSI = (HeaderData & b_PSI) >> 22;
    m_TP = (HeaderData & b_TP) >> 21;
    m_PID = (HeaderData & b_PID) >> 8;
    m_TSC = (HeaderData & b_TSC) >> 6;
    m_AFC = (HeaderData & b_AFC) >> 4;
    m_CC = HeaderData & b_CC;

    return NOT_VALID;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const {
  cout << "TS: SB=" << to_string(m_SB) << " E=" << to_string(m_TEI) 
       << " S=" << to_string(m_PSI) << " T=" << to_string(m_TP) 
       << " PID=" << to_string(m_PID) << " TSC=" << to_string(m_TSC) 
       << " AFC=" << to_string(m_AFC) << " CC=" << to_string(m_CC);
}

//=============================================================================================================================================================================


//Adaptation field methods

void xTS_AdaptationField::Reset() {
  m_AdaptationFieldControl = 0;
  m_AdaptationFieldLength = 0;

  m_DI = m_RAI = m_ESPI = m_PC = m_OPC = m_SPF = m_TPDF = m_AFEF = 0;
}

int32_t xTS_AdaptationField::Parse(const uint8_t* PacketBuffer) {
  const int HeaderLength = 4;
  uint32_t *AFPtr = (uint32_t *)(PacketBuffer + HeaderLength); // Skip the header
  uint32_t AFData = xSwapBytes32(*AFPtr);

    uint32_t b_DI =     0b00000000100000000000000000000000;
    uint32_t b_RAI =    0b00000000010000000000000000000000;
    uint32_t b_ESPI =   0b00000000001000000000000000000000;
    uint32_t b_PC =     0b00000000000100000000000000000000;
    uint32_t b_OPC =    0b00000000000010000000000000000000;
    uint32_t b_SPF =    0b00000000000001000000000000000000;
    uint32_t b_TPDF =   0b00000000000000100000000000000000;
    uint32_t b_AFEF =   0b00000000000000010000000000000000;

    m_AdaptationFieldLength = AFData >> 24;
    m_DI = (AFData & b_DI) >> 23;
    m_RAI = (AFData & b_RAI) >> 22;
    m_ESPI = (AFData & b_ESPI) >> 21;
    m_PC = (AFData & b_PC) >> 20;
    m_OPC = (AFData & b_OPC) >> 19;
    m_SPF = (AFData & b_SPF) >> 18;
    m_TPDF = (AFData & b_TPDF) >> 17;
    m_AFEF = (AFData & b_AFEF) >> 16;


    return NOT_VALID;
}

void xTS_AdaptationField::Print() const {
  cout << " AF: L=" << to_string(m_AdaptationFieldLength) <<
  " DC=" << to_string(m_DI) << " RA=" << to_string(m_RAI) <<
  " SP=" << to_string(m_ESPI) << " PR=" << to_string(m_PC) <<
  " OR=" << to_string(m_OPC) << " SF=" << to_string(m_SPF) <<
  " TP=" << to_string(m_TPDF) << " EX=" << to_string(m_AFEF)
  ;
}