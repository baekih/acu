/*
 * nmea2000.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"
#include "fastpacketdata.h"

#include "pgn_059904.h"
#include "pgn_060160.h"
#include "pgn_060416.h"
#include "pgn_060416_rts.h"
#include "pgn_060416_bam.h"
#include "pgn_060928.h"
#include "pgn_061184_stgf.h"
#include "pgn_065240.h"
#include "pgn_065285.h"
#include "pgn_065286.h"
#include "pgn_065287.h"
#include "pgn_126208.h"
#include "pgn_126993.h"
#include "pgn_126720.h"
#include "pgn_127250.h"
#include "pgn_128259.h"
#include "pgn_128267.h"
#include "pgn_129029.h"
#include "pgn_130306.h"
#include "pgn_130310.h"
#include "pgn_130816.h"

#define PGN_COUNT_MAX		64

uint8_t g_access_level = 0; // temp.

RxProtocol g_RxCan[CAN_RX_BUF_MAX];   // Rx array

uint16_t  rxCanLastIndex = 0;
uint16_t  rxCanFirstIndex = 0;

TxProtocol g_TxCan[CAN_TX_BUF_MAX];	// Tx array
uint8_t txCanBufferCount = 0;

/* Private variables ---------------------------------------------------------*/
PGNCounter g_PGNCount[PGN_COUNT_MAX];	// Rx array

uint32_t MAX_HIGH_SOURCE_ADDR = 252;
uint32_t ADDRESS_CLAIM_FAIL_ADDR = 254;

uint32_t localSourceAddr = 0;
uint32_t savedSourceAddr = 0;

uint32_t mIsNoAddress = false;

uint32_t mDataLastReceivedTime = 0;

uint32_t MaxPGNSequenceCounters = 0;
uint32_t *PGNSequenceCounters = 0;

uint8_t g_hwver_str[7];
uint8_t g_bootver_str[7];
uint8_t g_appver_str[7];

uint32_t uniquenum = 1048577;

uint8_t nmea2000_addr = 100;

uint8_t	DevInstance = 1;
uint8_t	SysInstance = 1;

uint8_t g_switch_bank[6];
uint8_t g_lcd_img_idx;

/* Private functions ---------------------------------------------------------*/
void NMEA2000_Open(void)
{
    savedSourceAddr = nmea2000_addr;
    localSourceAddr = savedSourceAddr;

    sprintf((char*)&mManufacturersModelVersion[0], "%s", g_hwver_str);
    sprintf((char*)&mManufacturersSoftwareVersionCode[0], "%s:%s", g_appver_str, g_bootver_str);

    mUnique_Number = uniquenum & 0x1FFFFF;
    mDevice_Intance = DevInstance & 0x7F;
    mSystem_Instance = SysInstance & 0x0F;

    InitializeMyNMEAData();
    InitMultiPacketArray();

    PGN060928_SetInitialField();
    PGN060928_SendAddressClaim();
}

void NMEA2000_126993_heartbeat(void)
{
    static uint8_t ucHeartBeatSequence = 0;

    PGN126993_ProcessNameField(ucHeartBeatSequence,
                               PGN126993_CAN_CTLR_STATE_CLASS1_ERR_ACTIVE,
                               PGN126993_EQUIP_STATUS_OPERAIONAL);

    if(ucHeartBeatSequence == PGN126993_HEARTBEAT_CNT_MAX)  ucHeartBeatSequence = 0;
    else                                                    ucHeartBeatSequence++;

    return;
}


uint32_t FastPacketSequenceCounter(uint32_t pgnNumber)
{
    for (uint8_t i = 0; i < PGN_COUNT_MAX; i++) {
        if (g_PGNCount[i].PGN == pgnNumber){
            g_PGNCount[i].value = g_PGNCount[i].value + 1;

            return (g_PGNCount[i].value % 8);
        }
        else {
            if (g_PGNCount[i].PGN == 0) {
                g_PGNCount[i].PGN = pgnNumber;
                g_PGNCount[i].value = 0;

                return 0;
            }
        }
    }

    return 0;
}

#define SEND_DELAY_TIME         1 // ms

void NMEA2000_SendParseMessages(NmeaPgn* pgnId, uint32_t len, uint8_t *buf, uint8_t isFastPacket)
{
    if ((mAddress_Claiming == true) && (pgnId->mPGN != 60928)){
        return;
    }

    if(mIsNoAddress == true){
        return;
    }

    if (pgnId->mSA == ADDRESS_CLAIM_FAIL_ADDR){
        mIsNoAddress = true;
    }

#if 0
    printf("[Send %06ld,%03ld,%03ld]", pgnId->mPGN, pgnId->mSA, pgnId->mPS);

    for (uint8_t i = 0; i < len; i++){
      printf("%02X ", buf[i]);
    }
    printf(" (%03ld)\n", len);
#endif

    if (isFastPacket == 0) {
        CAN1_SendFrame(pgnId->mCanNumericID, buf, 8);
    }
    else{
        uint8_t  firstFastPacket[8];
        uint8_t  otherFastPacket[8];
        uint32_t seqNumber = 0;

        seqNumber = FastPacketSequenceCounter(pgnId->mPGN);

//      printf("PGN : %d, seqNumber : %d\n", pgnId->mPGN, seqNumber);

        firstFastPacket[0] = (uint8_t) (seqNumber << 5);
        firstFastPacket[1] = (uint8_t) len;

        memcpy(firstFastPacket + 2, buf, 6);

        CAN1_SendFrame(pgnId->mCanNumericID, firstFastPacket, 8);

        uint32_t over2ndPacketSize = len - 6;

        if(over2ndPacketSize > 0) {
            uint8_t packetNumber = (uint8_t)((over2ndPacketSize / 7) + (((over2ndPacketSize%7) == 0) ? 0 : 1));

            for (uint8_t packetIndex = 0; packetIndex < packetNumber; packetIndex++) {

                memset(otherFastPacket, 0xFF, 8);
                otherFastPacket[0] = (uint8_t) ((uint8_t) (seqNumber << 5) | (uint8_t) ((packetIndex + 1) & 0x1F));

                uint32_t remainCopySize = len - (6 + (packetIndex * 7));

                memcpy(otherFastPacket + 1, buf + 6 + (packetIndex * 7), (remainCopySize >= 7) ? 7 : remainCopySize);

                HAL_Delay(SEND_DELAY_TIME);

                CAN1_SendFrame(pgnId->mCanNumericID, otherFastPacket, 8);
            }
        }
    }
}

void SendNonSingleFrame(NmeaPgn* pgnId, uint32_t len, uint8_t *buf, uint32_t messagetype)
{
    uint32_t totalNumberOfFrametoTransmit = 0;
    uint8_t  Multipacket[7];
    uint8_t  NonSingleFrameBuf[1785] = {0,};

    if (messagetype == REQUEST_MESSAGE_TYPE_FASTPACKET) {
        NMEA2000_SendParseMessages(pgnId, len, buf, 1) ;
    }
    else if (messagetype == REQUEST_MESSAGE_TYPE_RTSCTS_PACKET) {
        totalNumberOfFrametoTransmit = (len / 7) + (((len % 7) == 0) ? 0 : 1);

        InitializeSendMultiPacketData(len, totalNumberOfFrametoTransmit, pgnId->mPGN, pgnId->mSA, pgnId->mPS, true);

        memcpy(sendMultiPacket.mMergedMultiPacket, buf, len);

        PGN060416RTS_SetFieldValue(FunctionCodeRTS, len, totalNumberOfFrametoTransmit, 0xFF, pgnId->mPGN);
        PGN060416RTS_SendNameField(getCanId(PGN060416RTS_priority, 60416, pgnId->mPS, localSourceAddr));

    }
    else if (messagetype == REQUEST_MESSAGE_TYPE_BAM_PACKET) {
        memset(&NonSingleFrameBuf[0], 0xff, len);
        memcpy(&NonSingleFrameBuf[0], buf, len);

        totalNumberOfFrametoTransmit = (len / 7) + (((len % 7) == 0) ? 0 : 1);

        PGN060416BAM_SetFieldValue(FunctionCodeBAM, len, totalNumberOfFrametoTransmit, 0xFF, pgnId->mPGN);
        PGN060416BAM_SendNameField(getCanId(PGN060416BAM_priority, 60416, BROADCAST_DESTINATION_ADDR, localSourceAddr));

        for (uint8_t packetIndex = 0; packetIndex < totalNumberOfFrametoTransmit; packetIndex++) {
            memset(&Multipacket[0], 0xff, 7);
            if ((len - (packetIndex * 7)) >= 7) {
                for (uint8_t i = 0; i < 7; i++) {
                    Multipacket[i] = NonSingleFrameBuf[i + (packetIndex * 7)];
                }
            } else {
                for (uint8_t i = 0; i < (len - (packetIndex * 7)); i++) {
                    Multipacket[i] = NonSingleFrameBuf[i + (packetIndex * 7)];
                }
            }

            HAL_Delay(SEND_DELAY_TIME);

            PGN060160_SetFieldValue(packetIndex + 1, &Multipacket[0]);
            PGN060160_SendNameField(BROADCAST_DESTINATION_ADDR, localSourceAddr);
        }
    }
}

void ProcessNMEA2000MultiPacket(uint32_t proc_pgn_number, NmeaPgn* pgnId, uint16_t size, uint8_t *buf, uint32_t messagetype)
{
    switch(proc_pgn_number) {
        case 126208:
            PGN126208_GetFieldValue(pgnId, size, buf,  messagetype);
            PGN126208_ProcessNameField(pgnId, size, buf, messagetype);
            break;
        case 65240:
            PGN065240_GetFieldValue(pgnId, size, buf);
            PGN065240_ProcessNameField();
            break;
    }
}

void ProcessNMEA2000SinglePacket(NmeaPgn* pgnId, uint32_t len, uint8_t *buf)
{
    switch(pgnId->mPGN)
    {
        case 59392: // ISO Acknowledgement for Certification
            break;
        case 59904:	// ISO Request
        {
            PGN059904_GetFieldValue(pgnId, len, buf);
            PGN059904_ProcessNameField(pgnId, len, buf);
        }
            break;
        case 60160: // ISO Transport Protocol, Data Transfer
        {
            PGN060160_GetFieldValue(pgnId, len, buf);

            if (PGN060160_ProcessNameField(pgnId, len, buf) == PACKET_PROC_RESULT_MULTIPACKET_DONE) {

                if (completeMultiPacketData.mRTSCTS == true) {
                    ProcessNMEA2000MultiPacket(completeMultiPacketData.mPGNMultiPacketMessage, pgnId,
                            completeMultiPacketData.mTotalMessageByteSize, completeMultiPacketData.mMergedMultiPacket, REQUEST_MESSAGE_TYPE_RTSCTS_PACKET);
                }
                else {
                    ProcessNMEA2000MultiPacket(completeMultiPacketData.mPGNMultiPacketMessage, pgnId,
                            completeMultiPacketData.mTotalMessageByteSize, completeMultiPacketData.mMergedMultiPacket, REQUEST_MESSAGE_TYPE_BAM_PACKET);
                }
            }
        }
            break;
        case 60416:	// ISO Transport Protocol, Connection Management - BAM group function
        {
            PGN060416_GetFieldValue(pgnId, len, buf);
            PGN060416_ProcessNameField(pgnId, len, buf);
        }
            break;
        case 60928:	// ISO Address Claim
            if(pgnId->mSA == localSourceAddr) {  // Address crash.
                PGN060928_GetFieldValue(pgnId, len, buf, &g_PGN060928NAME);

                if (g_PGN060928NAME.mUnique_Number <= mMyPGN060928.mUnique_Number) {
                    if (localSourceAddr != ADDRESS_CLAIM_FAIL_ADDR) {
                        localSourceAddr++;

//                      Warning!! Do not direct access EEPROM in Processing to Receive NMEA2000 Data.
//                      EEPROM_Write_Byte(EEPROM_NMEA2000ADDR_ADDR, localSourceAddr, 0);

                        nmea2000_addr = localSourceAddr;

                        printf("New NMEA2000 addr:%ld\r\n", localSourceAddr);
                    }

                    if (localSourceAddr >= MAX_HIGH_SOURCE_ADDR && localSourceAddr < ADDRESS_CLAIM_FAIL_ADDR) {
                        localSourceAddr = 0;

//                      Warning!! Do not direct access EEPROM in Processing to Receive NMEA2000 Data.
//                      EEPROM_Write_Byte(EEPROM_NMEA2000ADDR_ADDR, localSourceAddr, 0);

                        nmea2000_addr = localSourceAddr;

                        printf("New NMEA2000 addr:%ld\r\n", localSourceAddr);
                    } else if (localSourceAddr == savedSourceAddr) {
                        localSourceAddr = ADDRESS_CLAIM_FAIL_ADDR;
                    }
                }

                PGN060928_SetInitialField();
                PGN060928_SendAddressClaim();

                break;
            }

            // check and change HIDHostAddress
/*          if(g_hidhost_canname_req == true){
                if(g_hidhost_addr == BROADCAST_DESTINATION_ADDR && g_spayload_curr.hidhost.canhostnamefield == *((uint64_t *)buf)){
                    g_hidhost_addr = pgnId->mSA;
                    g_hidhost_canname_req = false;
                }
                else if(g_hidhost_addr == pgnId->mSA && g_spayload_curr.hidhost.canhostnamefield == 0xFFFFFFFFFFFFFFFF){
                    g_spayload_curr.hidhost.canhostnamefield = *((uint64_t *)buf);
                    g_hidhost_canname_req = false;
                }

                if(g_hidhost_canname_req == false){
                    printf("Set HIDHost Addr[%d] CANName[0x%lx%lx]\r\n", g_hidhost_addr,
                            (uint32_t)(g_spayload_curr.hidhost.canhostnamefield >> 32),
                            (uint32_t)g_spayload_curr.hidhost.canhostnamefield);
                }
            }*/

            break;
        case 61184:	// Self Test Group Function(Proprietary PGN)
        {
            PGN061184STGF_GetFieldValue(pgnId, len, buf);

            if(g_PGN061184STGFNAME.m61184Manufacturer_Code == mApp_FEC_Manufacturer_Code &&
                    g_PGN061184STGFNAME.m61184Industry_Group == mIndustry_Group &&
                    g_PGN061184STGFNAME.m61184Identification_Code == 0 &&
                    (g_PGN061184STGFNAME.m61184Control_Function == 0 || g_PGN061184STGFNAME.m61184Control_Function == 1)){
                PGN061184STGF_ProcessNameField(pgnId->mSA);
            }
        }
            break;
        case 65240 : // ISO Commanded Address
        {
            ;
        }
            break;
        case 65287 : // HID Target Status (Proprietary PGN)
        {
/*          if(isFECPPGN(buf)){
                if(canhostnamefield == 0xFFFFFFFFFFFFFFFF){
                    PGN065287_FEC_GetFieldValue(pgnId, buf, len);
                }
            }*/
        }
            break;
        case 126208:											// InOutput (For Certification)
        {
            if(ProcessFastPacketData(pgnId, len, buf) == FASTPACKET_PROC_RESULT_DONE) {
                uint8_t* completePacket = GetCompletedFastPacket();
                uint16_t packetSize = GetCompletedFastPacketSize();

                PGN126208_GetFieldValue(pgnId, packetSize, completePacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
                PGN126208_ProcessNameField(pgnId, packetSize, completePacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
            }
        }
            break;
        case 126720 : // (Proprietary PGN)
            if(ProcessFastPacketData(pgnId, len, buf) == FASTPACKET_PROC_RESULT_DONE)
            {
                uint8_t* completePacket = GetCompletedFastPacket();
                uint16_t packetSize = GetCompletedFastPacketSize();

                PGN126720_GetFieldValue(pgnId, packetSize, completePacket);

//              printf("PGN126720:MfgCode[%d] INDCode[%d] CMDFunc[%d]\r\n",
//              pgn126720.mManufacturer_Code, pgn126720.IdentifyCode, pgn126720.ControlFunction);
            }
            break;
        case 127250 :
            {
                PGN127250_GetFieldValue(pgnId, len, buf);
            }
            break;
        case 128259 :
            {
                PGN128259_GetFieldValue(pgnId, len, buf);
            }
            break;
        case 128267 :
            {
                PGN128267_GetFieldValue(pgnId, len, buf);
            }
            break;
        case 129029 :
            {
                if(ProcessFastPacketData(pgnId, len, buf) == FASTPACKET_PROC_RESULT_DONE){
                    uint8_t* completePacket = GetCompletedFastPacket();
                    uint16_t packetSize = GetCompletedFastPacketSize();

                    PGN129029_GetFieldValue(pgnId, packetSize, completePacket);
                }
            }
            break;
        case 130306 :
            {
                PGN130306_GetFieldValue(pgnId, len, buf);
            }
            break;
        case 130310:
            {
                PGN130310_GetFieldValue(pgnId, len, buf);
            }
            break;
        case 130816: // (Proprietary PGN)
            if(ProcessFastPacketData(pgnId, len, buf) == FASTPACKET_PROC_RESULT_DONE)
            {
                PGN130816_ProcessNameField(pgnId);
            }
            break;
        case 65286 : // (Proprietary PGN) for Boot-loader - AIRMAR: Boot State Request
        {
            if(isAirmarPPGN(buf)){
                PGN065286_ProcessNameField();
            }
        }
            break;
        default:
            break;
    }
}

void NMEA2000_ReceiveParseMessages(uint32_t canId, uint8_t *buf, uint8_t len)
{
    NmeaPgn* pgnId = CanIDToNmeaPGN(canId);

#if 0
    printf("[ RCV %06ld,%03ld,%03ld]", pgnId->mPGN, pgnId->mSA, pgnId->mPS);

    for (uint8_t i = 0; i < len; i++){
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
#endif

    if(pgnId->mPF <= 239)
    {
        if(pgnId->mPS == localSourceAddr || pgnId->mPS == 255)
        {
            ProcessNMEA2000SinglePacket(pgnId, len, buf);
        }
    }
    else
    {
        ProcessNMEA2000SinglePacket(pgnId, len, buf);
    }
}

