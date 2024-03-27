#include "tsTransportStream.h"
#include <iostream>

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset()
{
  m_SB = m_TEI = m_PSI = m_TP = m_PID = m_TSC = m_AFC = m_CC = 0;
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t* Input)
{
  //i`m empty
  return NOT_VALID;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const
{
    std::cout << "TS: SB = " << m_SB << 
    " TEI = " << m_TEI << 
    " PSI = " << m_PSI << 
    " TP = " << m_TP << 
    " PID = " << m_PID << 
    " TSC = " << m_TSC << 
    " AFC = " << m_AFC <<  
    " CC = " << m_CC ;
}

//=============================================================================================================================================================================
