/*
 * nmea2000.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "fastpacketdata.h"

#include "pgn_060928.h"
#include "pgn_059904.h"
#include "pgn_126208.h"
#include "pgn_126720.h"

#include "pgn_065286.h"

RxProtocol g_RxCan[CAN_RX_BUF_MAX];   // Rx array

uint16_t  rxCanLastIndex = 0;
uint16_t  rxCanFirstIndex = 0;

TxProtocol g_TxCan[CAN_TX_BUF_MAX]; // Tx array
uint8_t txCanBufferCount = 0;

/* Private variables ---------------------------------------------------------*/
PGNCounter g_PGNCount[255];	// Rx array

uint32_t RxCanReceiveCount = 0;

uint32_t BROADCAST_DESTINATION_ADDR	= 255;
uint32_t MAX_HIGH_SOURCE_ADDR = 252;
uint32_t ADDRESS_CLAIM_FAIL_ADDR = 254;
uint32_t localSourceAddr = 0;
uint32_t savedSourceAddr = 0;

uint32_t mIsNoAddress = false;

uint32_t mDataLastReceivedTime = 0;

uint32_t MaxPGNSequenceCounters = 0;
uint32_t *PGNSequenceCounters = 0;

/* Private functions ---------------------------------------------------------*/
void NMEA2000_Open()
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

    savedSourceAddr = g_common_dat.nmea2k_adr;
    localSourceAddr = savedSourceAddr;

//	printf(" localSourceAddr[%d] \r\n", localSourceAddr);
    sprintf((char*)&mManufacturersModelVersion[0], "%s", g_hwver_str);
    sprintf((char*)&mManufacturersSoftwareVersionCode[0], "%s:%s", g_appver_str, g_bootver_str);

    mUnique_Number = (g_common_dat.uniquenum[2]<<16 | g_common_dat.uniquenum[1]<<8 | g_common_dat.uniquenum[0])&0x1FFFFF;
    mDevice_Intance = g_common_dat.adrclame.DevInstance & 0x7F;
    mSystem_Instance = g_common_dat.adrclame.SysInstance & 0x0F;

//  printf("%s() BOOT mUnique_Number[%ld]\r\n",__FUNCTION__, mUnique_Number);
    InitializeMyNMEAData();

    PGN060928_SetInitialField(&mMyPGN060928);
    PGN060928_SendAddressClaim();
}

uint32_t FastPacketSequenceCounter(uint32_t pgnNumber)
{
	for (uint8_t i = 0; i < 255; i++) {
		if (g_PGNCount[i].PGN == 0) {
			g_PGNCount[i].PGN = pgnNumber;
			g_PGNCount[i].value = 0;
//			printf(" [g_PGNCount[%d].value = %ld] !!\r\n", i, g_PGNCount[i].value);
			return 0;
		} else if (g_PGNCount[i].PGN != 0 && g_PGNCount[i].PGN == pgnNumber){
			g_PGNCount[i].value = g_PGNCount[i].value + 1;
//			printf(" [g_PGNCount[%d].value = %ld] !!\r\n", i, g_PGNCount[i].value);
			return (g_PGNCount[i].value % 8);
		}
	}

	return 0;
}

void NMEA2000_SendParseMessages(uint32_t canid, uint32_t len, uint8_t *buf, uint8_t isFastPacket)
{
	uint8_t  FastPacket[8];
	uint8_t  framesNumber = 0;
	uint32_t sequenceCount = 0;
	uint8_t  cursor = 0;
	uint32_t firstpacketIndex = 2;
	uint32_t otherpacketIndex = 1;

//  printf("%s:%d Enter.\r\n",__FUNCTION__,__LINE__);

	if ((mAddress_Claiming == true) && (get_R_SA() != 60928)){
//	  printf("%s:%d Address Claiming & R_SA is not 60928 Error.\r\n",__FUNCTION__,__LINE__);
		return;
	}

	if(mIsNoAddress == true){
//    printf("%s:%d Is No Address Error.\r\n",__FUNCTION__,__LINE__);
		return;
	}

	if (get_R_SA() == ADDRESS_CLAIM_FAIL_ADDR){
//    printf("%s:%d Address Claim FAIL.\r\n",__FUNCTION__,__LINE__);
		mIsNoAddress = true;
	}

	if (isFastPacket == 0) {
		CAN1_SendFrame(canid, len, buf);
	} else if (isFastPacket == 1) {
//		printf(" [get PGN = %ld] !!\r\n", CanID_NmeaPGN(canid));

		framesNumber = (len > 6 ? (len-6-1)/7+1+1 : 1);
		sequenceCount = FastPacketSequenceCounter(CanID_NmeaPGN(canid)) << 5;

#if 0
		printf(" [isFastPacket len = %ld] !!\r\n", len);
		printf(" [isFastPacket buf = ");		for (uint8_t i = 0; i < len; i++)	printf("0x%02x ", buf[i]);		printf("] !!\r\n");
		printf(" [framesNumber = %d] !!\r\n", framesNumber);
#endif

		for (uint32_t packetIndex = 0; packetIndex < framesNumber; packetIndex++) {
			FastPacket[0] = packetIndex | sequenceCount;
//			printf(" [FastPacket[0] = 0x%02lx] !!\r\n", packetIndex | sequenceCount);
			if (packetIndex == 0) {		//firstFastPacket
				FastPacket[1] = len;
				for (firstpacketIndex = 2; firstpacketIndex < 8; firstpacketIndex++) {
					FastPacket[firstpacketIndex] = buf[cursor];
					cursor++;
				}
			} else {					//otherFastPacket
				otherpacketIndex = 1;
				//send the next 7 data bytes
				for (; otherpacketIndex < 8 && cursor < len; otherpacketIndex++) {
					FastPacket[otherpacketIndex] = buf[cursor];
					cursor++;
				}
				for (; otherpacketIndex < 8; otherpacketIndex++) {
					FastPacket[otherpacketIndex] = 0xff;
				}
			}

			osDelay(1);
			CAN1_SendFrame(canid, 8, FastPacket);
		}
	}

//  printf("%s:%d eXit.\r\n",__FUNCTION__,__LINE__);
}

void SendNonSingleFrame(uint32_t canid, uint8_t len, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter messagetype[%ld]\r\n",__FUNCTION__,__LINE__, messagetype);

	if(messagetype == REQUEST_MESSAGE_TYPE_FASTPACKET)
	{
		NMEA2000_SendParseMessages(canid, len, buf, 1) ;
	}
}

void ProcessNMEA2000MultiPacket(uint32_t proc_pgn_number, uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

  switch(proc_pgn_number)
	{
  case 126208:
    PGN126208_GetFieldValue(pgnid, buf, messagetype);
    PGN126208_ProcessNameField(pgnid, buf, messagetype);
    break;
  case 126720:
    PGN126720_GetFieldValue(pgnid, buf, messagetype);
    PGN126720_ProcessNameField(pgnid, buf, messagetype);
    break;
	}
}

void ProcessNMEA2000SinglePacket(uint32_t pgnid, uint8_t len, uint8_t *buf)
{
//  printf("%s:%d Enter... pgnid[%ld]\r\n",__FUNCTION__,__LINE__, pgnid);

	switch(pgnid)
	{
		case 45312:												//ECO: Eeprom_Write
			//Eeprom_Write(0x00, len, buf);
			break;
		case 49408:												//ECO: Eeprom_Read
			//Eeprom_read(0x00, len, buf);
			break;
		case PGN065286_NUM:								// AIRMAR: Boot State Request
			PGN065286_GetFieldValue(pgnid, len, buf);
			PGN065286_ProcessNameField(pgnid, len, buf);
			break;
		case 59392:												// Output (For Certification)
			break;
		case 59904:												// Input (For Certification)
			PGN059904_GetFieldValue(pgnid, len, buf);
			PGN059904_ProcessNameField(pgnid, len, buf);
			break;
		case 60160:												// Input (For Certification)
			break;
		case 60416:												// Input (For Certification)
			break;
		case 60928:												// InOutput (For Certification)
			if(get_R_SA() == localSourceAddr) //Address crash.
			{
				PGN060928_GetFieldValue(len, buf, &g_PGN060928NAME);

				if (g_PGN060928NAME.mUnique_Number <= mMyPGN060928.mUnique_Number) {
					if (localSourceAddr != ADDRESS_CLAIM_FAIL_ADDR) {
						localSourceAddr++;

//						Warning!! Do not direct access EEPROM in Processing to Receive NMEA2000 Data.
//						EEPROM_Write_Byte(EEPROM_NMEA2000ADDR_ADDR, localSourceAddr, 0);

						g_common_dat.nmea2k_adr = localSourceAddr;

						printf("New NMEA2000 addr:%ld\r\n", localSourceAddr);
					}

					if (localSourceAddr >= MAX_HIGH_SOURCE_ADDR && localSourceAddr < ADDRESS_CLAIM_FAIL_ADDR) {
						localSourceAddr = 0;

//						Warning!! Do not direct access EEPROM in Processing to Receive NMEA2000 Data.
//						EEPROM_Write_Byte(EEPROM_NMEA2000ADDR_ADDR, localSourceAddr, 0);

						g_common_dat.nmea2k_adr = localSourceAddr;

						printf("New NMEA2000 addr:%ld\r\n", localSourceAddr);
					} else if (localSourceAddr == savedSourceAddr) {
						localSourceAddr = ADDRESS_CLAIM_FAIL_ADDR;
					}
				}

				PGN060928_SetInitialField(&mMyPGN060928);
				PGN060928_SendAddressClaim();
			}

			break;
		case 65240:												// Input (For Certification)
			break;
		case 126208:											// InOutput (For Certification)
			if(ProcessFastPacketData(pgnid, len, buf) == FASTPACKET_PROC_RESULT_DONE)
			{
				PGN126208_GetFieldValue(pgnid, mMerged_FastPacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
				PGN126208_ProcessNameField(pgnid, mMerged_FastPacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
			}
			break;
		case 126464:											// Output (For Certification)
			break;
    case PGN126720_NUM:
      if(ProcessFastPacketData(pgnid, len, buf) == FASTPACKET_PROC_RESULT_DONE)
      {
        PGN126720_GetFieldValue(pgnid, mMerged_FastPacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
        PGN126720_ProcessNameField(pgnid, mMerged_FastPacket, REQUEST_MESSAGE_TYPE_FASTPACKET);
      }
      break;
		case 126992:
			break;
		case 126993:											// Output (For Certification)
			break;
		case 126996:
			break;
		case 126998:											// Output (For Certification)
			break;
		case 127250:
		case 127251:
		case 127252:
		case 127257:
		case 127258:
		case 128259:
		case 128267:
		case 129025:
		case 129026:
		case 129029:
		case 129538:
		case 130306:
		case 130310:
		case 130311:
		case 130312:
		case 130314:
		case 130316:
		case 130577:
		default:
			break;
	}
}

/* ************************************************************************** */
uint8_t IsDefaultSingleFrameMessage(uint32_t PGN)
{
	switch (PGN) {
		case 126992L: // System date/time, pri=3, period=1000
		case 126993L: // Heartbeat, pri=7, period=60000
		case 127245L: // Rudder, pri=2, period=100
		case 127250L: // Vessel Heading, pri=2, period=100
		case 127251L: // Rate of Turn, pri=2, period=100
		case 127257L: // Attitude, pri=3, period=1000
		case 127488L: // Engine parameters rapid, rapid Update, pri=2, period=100
		case 127493L: // Transmission parameters: dynamic, pri=2, period=100
		case 127501L: // Binary status report, pri=3, period=NA
		case 127505L: // Fluid level, pri=6, period=2500
		case 127508L: // Battery Status, pri=6, period=1500
		case 128259L: // Boat speed, pri=2, period=1000
		case 128267L: // Water depth, pri=3, period=1000
		case 129025L: // Lat/lon rapid, pri=2, period=100
		case 129026L: // COG SOG rapid, pri=2, period=250
		case 129283L: // Cross Track Error, pri=3, period=1000
		case 130306L: // Wind Speed, pri=2, period=100
		case 130310L: // Outside Environmental parameters, pri=5, period=500
		case 130311L: // Environmental parameters, pri=5, period=500
		case 130312L: // Temperature, pri=5, period=2000
		case 130313L: // Humidity, pri=5, period=2000
		case 130314L: // Pressure, pri=5, period=2000
		case 130316L: // Temperature extended range, pri=5, period=NA
		case 130576L: // Small Craft Status (Trim Tab position), pri=2, period=200
			return true;
	}
	return false;
}

uint8_t IsMandatoryFastPacketMessage(uint32_t PGN)
{
	switch (PGN) {
		case 126464L: // PGN List (Transmit and Receive), pri=6, period=NA
		case 126996L: // Product information, pri=6, period=NA
		case 126998L: // Configuration information, pri=6, period=NA
			return true;
	}
	return false;
}

uint8_t IsDefaultFastPacketMessage(uint32_t PGN)
{
	switch (PGN) {
		case 126983L: // Alert, pri=2, period=1000
		case 126984L: // Alert Response, pri=2, period=NA
		case 126985L: // Alert Text, pri=2, period=10000
		case 126986L: // Alert Configuration, pri=2, period=NA
		case 126987L: // Alert Threshold, pri=2, period=NA
		case 126988L: // Alert Value, pri=2, period=10000
		case 127233L: // Alert Value, pri=3, period=NA
		case 127237L: // Heading/Track control, pri=2, period=250
		case 127489L: // Engine parameters dynamic, pri=2, period=500
		case 127496L: // Trip fuel consumption, vessel, pri=5, period=1000
		case 127497L: // Trip fuel consumption, engine, pri=5, period=1000
		case 127498L: // Engine parameters static, pri=5, period=NA
		case 127503L: // AC Input Status, pri=6, period=1500
		case 127504L: // AC Output Status, pri=6, period=1500
		case 127506L: // DC Detailed status, pri=6, period=1500
		case 127507L: // Charger status, pri=6, period=1500
		case 127509L: // Inverter status, pri=6, period=1500
		case 127510L: // Charger configuration status, pri=6, period=NA
		case 127511L: // Inverter Configuration Status, pri=6, period=NA
		case 127512L: // AGS configuration status, pri=6, period=NA
		case 127513L: // Battery configuration status, pri=6, period=NA
		case 127514L: // AGS Status, pri=6, period=1500
		case 128275L: // Distance log, pri=6, period=1000
		case 128520L: // Tracked Target Data, pri=2, period=1000
		case 129029L: // GNSS Position Data, pri=3, period=1000
		case 129038L: // AIS Class A Position Report, pri=4, period=NA
		case 129039L: // AIS Class B Position Report, pri=4, period=NA
		case 129040L: // AIS Class B Extended Position Report, pri=4, period=NA
		case 129041L: // AIS Aids to Navigation (AtoN) Report, pri=4, period=NA
		case 129044L: // Datum, pri=6, period=10000
		case 129045L: // User Datum Settings, pri=6, period=NA
		case 129284L: // Navigation info, pri=3, period=1000
		case 129285L: // Waypoint list, pri=3, period=NA
		case 129301L: // Time to/from Mark, pri=3, period=1000
		case 129302L: // Bearing and Distance between two Marks, pri=6, period=NA
		case 129538L: // GNSS Control Status, pri=6, period=NA
		case 129540L: // GNSS Sats in View, pri=6, period=1000
		case 129541L: // GPS Almanac Data, pri=6, period=NA
		case 129542L: // GNSS Pseudorange Noise Statistics, pri=6, period=1000
		case 129545L: // GNSS RAIM Output, pri=6, period=NA
		case 129547L: // GNSS Pseudorange Error Statistics, pri=6, period=NA
		case 129549L: // DGNSS Corrections, pri=6, period=NA
		case 129551L: // GNSS Differential Correction Receiver Signal, pri=6, period=NA
		case 129556L: // GLONASS Almanac Data, pri=6, period=NA
		case 129792L: // AIS DGNSS Broadcast Binary Message, pri=6, period=NA
		case 129793L: // AIS UTC and Date Report, pri=7, period=NA
		case 129794L: // AIS Class A Static data, pri=6, period=NA
		case 129795L: // AIS Addressed Binary Message, pri=5, period=NA
		case 129796L: // AIS Acknowledge, pri=7, period=NA
		case 129797L: // AIS Binary Broadcast Message, pri=5, period=NA
		case 129798L: // AIS SAR Aircraft Position Report, pri=4, period=NA
		case 129799L: // Radio Frequency/Mode/Power, pri=3, period=NA
		case 129800L: // AIS UTC/Date Inquiry, pri=7, period=NA
		case 129801L: // AIS Addressed Safety Related Message, pri=5, period=NA
		case 129802L: // AIS Safety Related Broadcast Message, pri=5, period=NA
		case 129803L: // AIS Interrogation PGN, pri=7, period=NA
		case 129804L: // AIS Assignment Mode Command, pri=7, period=NA
		case 129805L: // AIS Data Link Management Message, pri=7, period=NA
		case 129806L: // AIS Channel Management, pri=7, period=NA
		case 129807L: // AIS Group Assignment, pri=7, period=NA
		case 129808L: // DSC Call Information, pri=8, period=NA
		case 129809L: // AIS Class B Static Data: Part A, pri=6, period=NA
		case 129810L: // AIS Class B Static Data Part B, pri=6, period=NA
		case 129811L: // AIS Single Slot Binary Message, pri=5, period=NA
		case 129812L: // AIS Multi Slot Binary Message, pri=5, period=NA
		case 129813L: // AIS Long-Range Broadcast Message, pri=5, period=NA
		case 130052L: // Loran-C TD Data, pri=3, period=1000
		case 130053L: // Loran-C Range Data, pri=3, period=1000
		case 130054L: // Loran-C Signal Data, pri=3, period=1000
		case 130060L: // Label, pri=7, period=NA
		case 130061L: // Channel Source Configuration, pri=7, period=NA
		case 130064L: // Route and WP Service - Database List, pri=7, period=NA
		case 130065L: // Route and WP Service - Route List, pri=7, period=NA
		case 130066L: // Route and WP Service - Route/WP-List Attributes, pri=7, period=NA
		case 130067L: // Route and WP Service - Route - WP Name & Position, pri=7, period=NA
		case 130068L: // Route and WP Service - Route - WP Name, pri=7, period=NA
		case 130069L: // Route and WP Service - XTE Limit & Navigation Method, pri=7, period=NA
		case 130070L: // Route and WP Service - WP Comment, pri=7, period=NA
		case 130071L: // Route and WP Service - Route Comment, pri=7, period=NA
		case 130072L: // Route and WP Service - Database Comment, pri=7, period=NA
		case 130073L: // Route and WP Service - Radius of Turn, pri=7, period=NA
		case 130074L: // Route and WP Service - WP List - WP Name & Position, pri=7, period=NA
		case 130320L: // Tide Station Data, pri=6, period=1000
		case 130321L: // Salinity Station Data, pri=6, period=1000
		case 130322L: // Current Station Data, pri=6, period=1000
		case 130323L: // Meteorological Station Data, pri=6, period=1000
		case 130324L: // Moored Buoy Station Data, pri=6, period=1000
		case 130567L: // Watermaker Input Setting and Status, pri=6, period=2500
		case 130577L: // Direction Data PGN, pri=3, period=1000
		case 130578L: // Vessel Speed Components, pri=2, period=250
			return true;
	}
	return false;
}

uint8_t IsSingleFrameSystemMessage(uint32_t PGN)
{
	switch (PGN) {
		case 59392L: /* ISO Acknowledgement */
		case 60160L: /* Multi packet data transfer, TP.DT */
		case 60416L: /* Multi packet connection management, TP.CM */
		case 59904L: /* ISO Request */
		case 60928L: /* ISO Address Claim */
			return true;
	}
	return false;
}

uint8_t IsFastPacketSystemMessage(uint32_t PGN)
{
	switch (PGN) {
		case 65240L:  /* Commanded Address*/
		case 126208L: /* NMEA Request/Command/Acknowledge group function */
			return true;
	}
	return false;
}

uint8_t IsProprietarySingleFrameMessage(uint32_t PGN)
{
	if ((PGN == 61184L) || (65280L <= PGN && PGN <= 65535L))
		return true;

	return false;
}

uint8_t IsProprietaryFastPacketMessage(uint32_t PGN)
{
	if ((PGN == 126720L) || (130816L <= PGN && PGN <= 131071L))
		return true;

	return false;
}

uint8_t IsSingleFrameEcoSystemMessage(uint32_t PGN)
{
	switch (PGN) {
		case 45312L:		//ECO: Eeprom_Write
		case 49408L:		//ECO: Eeprom_Read
		case 65286L:		//AIRMAR: Boot State Request
			return true;
	}
	return false;
}

uint32_t CheckKnownMessage(uint32_t PGN, uint8_t SystemMessage, uint8_t FastPacket)
{
	if(PGN == 0) return false;	// Unknown

	// Check other messages
	if(IsDefaultSingleFrameMessage(PGN)) return true;
	if(IsMandatoryFastPacketMessage(PGN))
	{
		FastPacket = true;
		return true;
	}

	if(IsDefaultFastPacketMessage(PGN))
	{
		FastPacket = true;
		return true;
	}

	// Check system messages
	if(IsSingleFrameSystemMessage(PGN) || (FastPacket = IsFastPacketSystemMessage(PGN)) == true)
	{
		SystemMessage = true;
		return true;
	}

  if(IsSingleFrameEcoSystemMessage(PGN))
  {
    SystemMessage = true;
    return true;
  }

	// Check propiretary message
	if(IsProprietarySingleFrameMessage(PGN) || true == (FastPacket = IsProprietaryFastPacketMessage(PGN)))
	{
    return true;
	}

	return false;
}
/* ************************************************************************** */

void NMEA2000_ReceiveParseMessages(uint32_t canid, uint8_t len, uint8_t *buf)
{
	uint32_t ReceiveParsePGN = 0;
	uint8_t FastPacket = false;
	uint8_t SystemMessage = false;
	uint8_t KnownMessage = false;

	/*
		uint32_t pgn;
	
		printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
	
		// ExtId(29b)	= PGN(21b) + Source Address(8b)
		// PGN(21b) 	= Priority(3b) + EDP(1b) + DP(1) + PF(8) + PS(8b)
		// PDU1 		= PF(0~239),	PS(DA)
		// PDU2 		= PF(240~255),	PS(GE)
	
		pgn = (canid >> 8) & 0x01ffff;
	
		if ((pgn >> 8) < 240) {
			pgn &= 0x01ff00;		// PDU1 = PF(0~239), PS(DA)
		}
	*/

//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	RxCanReceiveCount++;

#if 0
	printf(" Receive ID: 0x%08lx, LEN: %d,", canid, len);
//	printf(" Data: ");
//	for (uint8_t i = 0; i < len; i++)	printf("0x%02x ", buf[i]);
	printf("\r\n");
#endif

	ReceiveParsePGN = CanID_NmeaPGN(canid);

	KnownMessage = CheckKnownMessage(ReceiveParsePGN, SystemMessage, FastPacket);

	mDataLastReceivedTime = HAL_GetTick();
#if 0
	printf(" [mDataLastReceivedTime = %ld] !!\r\n", mDataLastReceivedTime);
#endif

  // single_frame parse
  if(KnownMessage)
  {
    if(get_R_PF() <= 239)
    {
      if (get_R_PS() == localSourceAddr || get_R_PS() == 255)
      {
        if(get_R_SA() == g_flash_source_addr || 255 == g_flash_source_addr)
        {
          ProcessNMEA2000SinglePacket(ReceiveParsePGN, len, buf);
        }
      }
    }
    else
    {
      ProcessNMEA2000SinglePacket(ReceiveParsePGN, len, buf);
    }
  }
}

