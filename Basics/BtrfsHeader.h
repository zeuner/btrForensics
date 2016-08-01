/** \file
  * Header file of class BtrfsHeader
  */
  
#ifndef BTRFS_HEADER_H
#define BTRFS_HEADER_H

#include <iostream>
#include <tsk/libtsk.h>
#include "Utility/Uuid.h"

namespace btrForensics{
    /** Header of a node in btrfs. */
    class BtrfsHeader{
    private:
        uint8_t checksum[0x20]; //0x0

        const UUID fsUUID; //0x20

        uint64_t address; //0x30
        uint8_t flags[0x08];

        const UUID chunkTrUUID; //0x40

        uint64_t generation; //0x50
        uint64_t treeId;

        uint32_t numOfItems; //0x60
        uint8_t level;
        
        //Total bytes: 0x65

    public:
        BtrfsHeader(TSK_ENDIAN_ENUM endian, uint8_t arr[]);

        const uint32_t getNumOfItems() const;
        
        friend std::ostream &operator<<(
            std::ostream &os, const BtrfsHeader &header);

        static const int SIZE_OF_HEADER = 0x65; /**< Size of node header in bytes. */
    };

}

#endif
