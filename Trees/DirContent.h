//! \file
//! \author Shujian Yang
//!
//! Header file of class DirContent.

#ifndef DIR_CONTENT_H
#define DIR_CONTENT_H

#include <iostream>
#include <string>
#include <vector>
#include <tsk/libtsk.h>
#include "Basics/Basics.h"

namespace btrForensics{
    class FileTreeAnalyzer;

    //! Record of a directory.
    class DirContent {
    public:
        const InodeItem *inode; //!< Inode of this directory.
        const InodeRef *ref; //!< Inode points to this directory.
        std::string name; //!< Name of this directory.
        std::vector<DirItem*> children; //!< Entries of this directory.

        DirContent(InodeItem* inodeItem, InodeRef* inodeRef,
                std::vector<BtrfsItem*> &dirItems);

        friend std::ostream &operator<<(std::ostream&, const DirContent&);
    };

}

#endif

