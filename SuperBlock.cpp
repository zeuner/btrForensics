/** \file
  * SuperBlock implementation.
  */

#include "SuperBlock.h"
#include "Utility.h"

/**
  * Constructor of super block.
  * 
  * \param endian The endianess of the array.
  * \param arr Byte array storing super block data.
  * 
  */
SuperBlock::SuperBlock(TSK_ENDIAN_ENUM endian, uint8_t arr[])
    :fsUUID(endian, arr + 0x20)
{
    int arIndex(0);
    for(int i=0; i<0x20; i++, arIndex++){
        checksum[i] = arr[arIndex];
    }
    arIndex += 0x20;
    for(int i=0; i<0x8; i++, arIndex++){
        magic[i] = arr[arIndex];
    }

    arIndex += 0x8;
    rootTrRoot = read64Bit(endian, arr + arIndex);

    arIndex += 0x8;
    chunkTrRoot = read64Bit(endian, arr + arIndex);

    arIndex += 0x8;
    logTrRoot = read64Bit(endian, arr + arIndex);
}

std::string SuperBlock::printMagic()
{
    return std::string(magic, 8);
}

/**
  * Overloaded stream operator.
  * 
  */
std::ostream &operator<<(std::ostream &os, SuperBlock &supb)
{
    os << supb.fsUUID.encode()
        << std::uppercase << std::hex;
    os << "\nRoot tree root address: ";
    os.fill('0');
    os.width(16);
    os << supb.rootTrRoot;
    os << "\nChunk tree root address: ";
    os.width(16);
    os << supb.chunkTrRoot;    
    os << "\nLog tree root address: ";
    os.width(16);
    os << supb.logTrRoot;
    return os;
}

