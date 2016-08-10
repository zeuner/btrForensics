/** \file
  * UUID processing.
  */

#include <sstream>
#include <bitset>
#include "Uuid.h"
#include "Utility/ReadInt.h"


/**
  * Constructor of UUID
  *
  * \param endian The endianess of the array.
  * \param arr Byte array storing UUID.
  *
  */
UUID::UUID(TSK_ENDIAN_ENUM endian, uint8_t arr[])
{
    data_1 = read32Bit(endian, arr);
    data_2 = read16Bit(endian, arr + 4);
    data_3 = read16Bit(endian, arr + 6);

    for(int i=0; i<8; i++){
        data_4[i] = *(arr + 8 + i);
    }
}


/**
  * Constructor of UUID
  *
  * \param endian The endianess of the array.
  * \param entry TSK gpt entry.
  *
  */
UUID::UUID(TSK_ENDIAN_ENUM endian, gpt_entry &entry)
{
    data_1 = read32Bit(endian, entry.type_guid);
    data_2 = read16Bit(endian, entry.type_guid + 4);
    data_3 = read16Bit(endian, entry.type_guid + 6);

    for(int i=0; i<8; i++){
        data_4[i] = *(entry.type_guid + 8 + i);
    }
}


/**
  * Check if a UUID is empty.
  *
  * \return True if the UUID is all 0s.
  *
  */
const bool UUID::isUnused() const
{
    return match(0, 0, 0, 0);
}

/**
  * Encode UUID as a string
  *
  * \return Encoded UUID string.
  */
const std::string UUID::encode() const
{
    if(match(0, 0, 0, 0))
        return "";
        
    std::ostringstream guidOSS;
    guidOSS.fill('0');
    guidOSS << std::uppercase << std:: hex;

    guidOSS.width(8);
    guidOSS << data_1 << '-';
    guidOSS.width(4);
    guidOSS << data_2 << '-';
    guidOSS.width(4);
    guidOSS << data_3 << '-';

    const uint8_t *part = data_4;
    guidOSS.width(2);
    guidOSS << (int)*part++;
    guidOSS.width(2);
    guidOSS << (int)*part++ << '-';

    for(int i=0; i<6; ++i){
        guidOSS.width(2);
        guidOSS << (int)*part++;
    }
    
    return guidOSS.str();
}


/**
  *  Check if UUID matches a given value.
  *
  * \param d1 Part 1 of UUID
  * \param d2 Part 2 of UUID
  * \param d3 Part 3 of UUID
  * \param d4 Part 4 of UUID
  *
  * \return True if UUID matches the given pattern.
  */
const bool UUID::match(uint32_t d1, uint16_t d2, uint16_t d3, uint64_t d4) const
{
    if(data_1 != d1 || data_2 != d2 || data_3 != d3)
        return false;
        
    uint64_t p4 = (uint64_t)data_4[0];
    int i;
    for(i=1; i<8; ++i){
        p4 = p4 << 8;
        p4 += (uint64_t)data_4[i];
    }
    
    if(p4 != d4)
        return false;
    else
        return true;
}


/**
  * Get partition type based on UUID value.
  *
  * \return String of partition type the UUID represents.
  */
const std::string UUID::guidType() const
{
    std::string type;
    if(match(0, 0, 0, 0))
        type = "Unused entry";
    else if(match(0x024DEE41, 0x33E7, 0x11D3, 0x9D690008C781F39F))
        type = "MBR partition scheme";
    else if(match(0xC12A7328, 0xF81F, 0x11D2, 0xBA4B00A0C93EC93B))
        type = "EFI System partition";
    else if(match(0x21686148, 0x6449, 0x6E6F, 0x744E656564454649))
        type = "BIOS Boot partition";
    else if(match(0xD3BFE2DE, 0x3DAF, 0x11DF, 0xBA40E3A556D89593))
        type = "Intel Fast Flash partition";
    else if(match(0xF4019732, 0x066E, 0x4E12, 0x8273346C5641494F))
        type = "Sony boot partition";
    else if(match(0xBFBFAFE7, 0xA34F, 0x448A, 0x9A5B6213EB736C22))
        type = "Lenovo boot partition";
    else if(match(0xE3C9E316, 0x0B5C, 0x4DB8, 0x817DF92DF00215AE))
        type = "Microsoft Reserved Partition";
    else if(match(0xDE94BBA4, 0x06D1, 0x4D40, 0xA16ABFD50179D6AC))
        type = "Windows Recovery Environment";
    else if(match(0xEBD0A0A2, 0xB9E5, 0x4433, 0x87C068B6B72699C7))
        type = "Basic data partition";
    else if(match(0x0FC63DAF, 0x8483, 0x4772, 0x8E793D69D8477DE4))
        type = "Linux filesystem data";
    else if(match(0x0657FD6D, 0xA4AB, 0x43C4, 0x84E50933C84B4F4F))
        type = "Linux swap partition";
    else if(match(0x933AC7E1, 0x2EB4, 0x4F13, 0xB8440E14E2AEF915))
        type = "Linux /home partition";
    else
        type = "[Unkown type]";
        
    return type;
}


/**
  * Get UUID variant.
  *
  * \return Variant of the UUID.
  */
const Variant UUID::getVariant() const
{
    std::bitset<8> d4(data_4[0]);

    if(!d4.test(7))
        return NETWORK_COMPUTING_SYSTEM;
    if(d4.test(7) && !d4.test(6))
        return STANDARD;
    if(d4.test(7) && d4.test(6) && !d4.test(5))
        return MS_COM;
    
    return RESERVED;
}


/**
  * Get UUID version.
  *
  * \return Version of the UUID.
  */
const int UUID::getVersion() const
{
    uint16_t ver = data_3 >> 12;

    return ver;
}


/**
  * Get UUID variant string.
  *
  * \return String of variant infomation.
  */
const std::string UUID::variantInfo() const
{
    std::string variant("");
    switch(getVariant()){
        case NETWORK_COMPUTING_SYSTEM: variant = "Network Computing System";
            break;
        case STANDARD:
            variant = "RFC 4122 Standard";
            break;
        case MS_COM:
            variant = "Microsoft COM";
            break;
        case RESERVED:
            variant = "Reserved";
            break;
        default:
            variant = "Unknown";
    }

    return variant;
}


/**
  * Get UUID version string.
  *
  * \return String of version infomation.
  */
const std::string UUID::versionInfo() const
{
    std::string version("");
    if(getVariant() != STANDARD)
        return "Invalid";

    switch(getVersion()){
        case 1:
            version = "Ver 1: MAC address & date-time";
            break;
        case 2:
            version = "Ver 2: DCE security";
            break;
        case 3:
            version = "Ver 3: MD5 hash & namespace";
            break;
        case 4:
            version = "Ver 4: Random number";
            break;
        case 5:
            version = "Ver 5: SHA-1 hash & namespace";
            break;
        default:
            version = "Unknown";
    }

    return version;
}

