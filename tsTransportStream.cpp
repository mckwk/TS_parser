#include <iostream>
#include "tsTransportStream.h"

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

    uint32_t TEI_mask = 0b00000000100000000000000000000000;
    uint32_t PSI_mask = 0b00000000010000000000000000000000;
    uint32_t TP_mask = 0b00000000000100000000000000000000;
    uint32_t PID_mask = 0b00000000000111111111111100000000;
    uint32_t TSC_mask = 0b00000000000000000000000011000000;
    uint32_t AFC_mask = 0b00000000000000000000000000110000;
    uint32_t CC_mask = 0b00000000000000000000000000001111;

    m_SB = HeaderData >> 24;
    m_TEI = (HeaderData & TEI_mask) >> 23;
    m_PSI = (HeaderData & PSI_mask) >> 22;
    m_TP = (HeaderData & TP_mask) >> 21;
    m_PID = (HeaderData & PID_mask) >> 8;
    m_TSC = (HeaderData & TSC_mask) >> 6;
    m_AFC = (HeaderData & AFC_mask) >> 4;
    m_CC = HeaderData & CC_mask;

    return NOT_VALID;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const {
    printf("TS:");
    printf(" SB=%d", m_SB);
    printf(" E=%d", m_TEI);
    printf(" S=%d", m_PSI);
    printf(" T=%d", m_TP);
    printf(" PID=%d", m_PID);
    printf(" TSC=%d", m_TSC);
    printf(" AFC=%d", m_AFC);
    printf(" CC=%d", m_CC);
}

//=============================================================================================================================================================================
