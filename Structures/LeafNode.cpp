/** \file
 * Implementation of class LeafNode.
 */

#include <sstream>
#include "LeafNode.h"

namespace btrForensics{
    /**
     * Constructor of btrfs leaf node.
     *
     * \param header Pointer to header of a node.
     * \param endian The endianess of the array.
     * \param arr Byte array storing leaf node data.
     *
     */
    LeafNode::LeafNode(TSK_IMG_INFO *img, BtrfsHeader *header, 
            TSK_ENDIAN_ENUM endian, uint64_t startOffset)
        :BtrfsNode(header)
    {
        char *diskArr;
        uint64_t itemOffset(0);
        uint32_t itemNum = header -> getNumOfItems();

        for(uint32_t i=0; i<itemNum; ++i){
            ItemGroup *group;

            diskArr = new char[BtrfsItem::SIZE_OF_ITEM]();
            tsk_img_read(img, startOffset + itemOffset,
                    diskArr, BtrfsItem::SIZE_OF_ITEM);

            BtrfsItem *item = new BtrfsItem(endian, (uint8_t*)diskArr);

            ItemData *itmData = nullptr;
            char *itmArr = new char[item->getDataSize()]();
            uint64_t dataOffset = startOffset + item->getDataOffset();
            tsk_img_read(img, dataOffset, itmArr, item->getDataSize());

            if(item->key.getItemType() == 0x54){
                itmData = new DirItem(TSK_LIT_ENDIAN, (uint8_t*)itmArr);
            }
            else if(item->key.getItemType() == 0x84){
                itmData = new RootItem(TSK_LIT_ENDIAN, (uint8_t*)itmArr);
            }
            else{
                itmData = new UnknownItem();
            }

            if(item != nullptr && itmData != nullptr){
                group = new ItemGroup(item, itmData);
                itemGroups.push_back(group);
            }

            delete [] diskArr;
            delete [] itmArr;

            itemOffset += BtrfsItem::SIZE_OF_ITEM;
        }
    }


    /**
     * Print info about this node.
     */
    const std::string LeafNode::info() const
    {
        std::ostringstream oss;

        oss << *nodeHeader << "\n\n";
        oss << "Item list:" << '\n';
        oss << std::string(30, '=') << "\n\n";

        for(auto &ig : itemGroups){
            oss << *ig;
            oss << std::string(30, '=') << "\n\n";
        }

        return oss.str();
    }

}

