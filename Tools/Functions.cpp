//! \file
//! \author Shujian Yang
//!
//! Implementations of functions declared in Functions.h.

#include <algorithm>
#include "Functions.h"

namespace btrForensics{
    //! Prints names of directory items stored in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param idTrace The vector used to trace node ids on the path from root to node.
    //! \param os Output stream where the infomation is printed.
    //!
    void printLeafDir(const LeafNode* leaf, std::ostream &os)
    {
        for(auto item : leaf->itemList){
            if(item->getItemType() == ItemType::DIR_ITEM){
                DirItem *dir = (DirItem*)item;
                if(dir->type == DirItemType::REGULAR_FILE)
                    os << dir->getDirName() << '\n';
            }
        }
    }


    //! Search for an item with given inode number in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param item Found ItemHead pointer.
    //!
    //! \return True if the item is found.
    //!
    bool searchItem(const LeafNode* leaf, uint64_t inodeNum,
           ItemType type, const BtrfsItem* &foundItem)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() > inodeNum) return false;
            if(item->getId() == inodeNum &&
                    item->getItemType() == type) {
                foundItem = item;
                return true;
            }
        }
        return false;
    }


    //! Search for an item not found before with given inode number in a leaf node.
    //!
    //! \param leaf Pointer to the leaf node.
    //! \param inodeNum The inode number to search for.
    //! \param type The type of the item to search for.
    //! \param vec Vector storing found items.
    //!
    //! \return True if all items with the inodeNum has been found.
    //!
    bool searchMultiItems(const LeafNode* leaf, uint64_t inodeNum, ItemType type,
           vector<BtrfsItem*> &vec)
    {
        for(auto item : leaf->itemList) {
            if(item->getId() > inodeNum) return true;
            if(item->getId() == inodeNum &&
                    item->getItemType() == type) {
                auto result = find(vec.cbegin(), vec.cend(), item);
                if(result == vec.cend())
                    vec.push_back(item);
            }
        }
        return false;
    }

}

