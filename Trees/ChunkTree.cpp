//! \file
//! \author Shujian Yang
//!
//! Header file of class ChunkTree

#include <iostream>
#include "ChunkTree.h"
#include "Examiners/Functions.h"

namespace btrForensics {
    //! Constructor of tree analyzer.
    //!
    //! \param superBlk Pointer to btrfs super block.
    //! \param treeExaminer Pointer to a tree examiner.
    //!
    ChunkTree::ChunkTree(BtrfsExaminer *btrfs)
            :examiner(treeExaminer)
    {
        SuperBlock *supBlk = btrfs->primarySupblk;
        uint64_t chunkTreePhyAddr = superBlk->getChunkPhyAddr();

        char* diskArr = new char[BtrfsHeader::SIZE_OF_HEADER]();
        tsk_img_read(examiner->image, examiner->imgOffset + chunkTreePhyAddr, diskArr, BtrfsHeader::SIZE_OF_HEADER);
        BtrfsHeader *chunkHeader = new BtrfsHeader(examiner->endian, (uint8_t*)diskArr);
        delete [] diskArr;

        uint64_t itemListStart = chunkTreePhyAddr + BtrfsHeader::SIZE_OF_HEADER;

        const BtrfsNode* chunkTree;
        if(chunkHeader->isLeafNode())
            chunkRoot = new LeafNode(examiner->image, examiner->imgOffset,
                            chunkHeader, examiner->endian, itemListStart);
        else
            chunkRoot = new InternalNode(examiner->image, examiner->imgOffset,
                            chunkHeader, TSK_LIT_ENDIAN, itemListStart);
    }


    //!< Destructor
    ChunkTree::~ChunkTree()
    {
        if(chunkRoot != nullptr)
            delete chunkRoot;
    }


    //! Convert logical address to physical address.
    //!
    //! \param logicalAddr 64-bit logial address.
    //! \return 64-bit physical address.
    //!
    uint64_t ChunkTree::getPhysicalAddr(uint64_t logicalAddr)
    {
        uint64_t physicalAddr(0);
        //std::cout << chunkRoot->info() << std::endl;
        /*examiner->treeSearch(chunkRoot,
                [logicalAddr, &physicalAddr](const LeafNode* leaf)
                { return getPhyAddr(leaf, logicalAddr, physicalAddr); });
        */
        return physicalAddr;
    }
}

