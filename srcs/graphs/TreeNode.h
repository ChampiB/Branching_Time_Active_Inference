//
// Created by Theophile Champion on 01/08/2021.
//

#ifndef BTAI_TREE_NODE_H
#define BTAI_TREE_NODE_H

#include <memory>
#include <vector>

namespace btai::graphs {

    template<class DataType>
    class TreeNode {
    public:
        /**
         * Factory.
         * @param data the
         * @return the created node.
         */
        static std::shared_ptr<TreeNode> create(std::unique_ptr<DataType> &&data);

        /**
         * Constructor.
         * @param data the node's data
         */
        explicit TreeNode(std::unique_ptr<DataType> &&data);

        /**
         * Set the data of the node.
         * @param data the new data.
         */
        void setData(std::unique_ptr<DataType> &data);

        /**
         * Set the parent of the node.
         * @param node the new parent.
         */
        void setParent(TreeNode *node);

        /**
         * Add a child to the node.
         * @param node the new child.
         */
        void addChild(const std::shared_ptr<TreeNode> &node);

        /**
         * Remove a child of the node.
         * @param id the index of the child to remove.
         */
        void removeChild(int id);

        /**
         * Remove all children of the node.
         */
        void clearChildren();

        /**
         * Disconnect this from its parent.
         */
        void disconnectParent();

        /**
         * Getter.
         * @return the node's data.
         */
        [[nodiscard]] DataType *data() const;

        /**
         * Getter.
         * @return the parent node.
         */
        [[nodiscard]] TreeNode *parent() const;

        /**
         * Getter.
         * @param id of the child to retrieve.
         * @return the id-th child.
         */
        [[nodiscard]] TreeNode *child(int id) const;

        /**
         * Getter.
         * @return the number of children.
         */
        [[nodiscard]] int nChildren() const;

        /**
         * Getter.
         * @return an iterator pointing to the beginning of the children vector.
         */
        [[nodiscard]] typename std::vector<std::shared_ptr<TreeNode>>::iterator childrenBegin();

        /**
         * Getter.
         * @return an iterator pointing to the end of the children vector.
         */
        [[nodiscard]] typename std::vector<std::shared_ptr<TreeNode>>::iterator childrenEnd();

    private:
        std::unique_ptr<DataType> _data;
        TreeNode *_parent;
        std::vector<std::shared_ptr<TreeNode>> _children;
    };

    namespace data {
        class DataMCTS;
    }
    template class TreeNode<data::DataMCTS>;
}

#endif //BTAI_TREE_NODE_H
