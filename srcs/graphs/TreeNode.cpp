//
// Created by Theophile Champion on 01/08/2021.
//

#include "data/DataMCTS.h"
#include "TreeNode.h"

namespace btai::graphs {

    template<class DataType>
    std::shared_ptr<TreeNode<DataType>> TreeNode<DataType>::create(std::unique_ptr<DataType> &&data) {
        return std::make_shared<TreeNode>(std::move(data));
    }

    template<class DataType>
    TreeNode<DataType>::TreeNode(std::unique_ptr<DataType> &&data) {
        _parent = nullptr;
        _data = std::move(data);
    }

    template<class DataType>
    void TreeNode<DataType>::setParent(TreeNode<DataType> *node) {
        _parent = node;
    }

    template<class DataType>
    void TreeNode<DataType>::addChild(const std::shared_ptr<TreeNode<DataType>> &node) {
        _children.push_back(node);
    }

    template<class DataType>
    void TreeNode<DataType>::removeChild(int id) {
        _children.erase(_children.begin() + id);
    }

    template<class DataType>
    void TreeNode<DataType>::setData(std::unique_ptr<DataType> &data) {
        _data = std::move(data);
    }

    template<class DataType>
    DataType *TreeNode<DataType>::data() const {
        return _data.get();
    }

    template<class DataType>
    TreeNode<DataType> *TreeNode<DataType>::parent() const {
        return _parent;
    }

    template<class DataType>
    TreeNode<DataType> *TreeNode<DataType>::child(int id) const {
        return _children[id].get();
    }

    template<class DataType>
    int TreeNode<DataType>::nChildren() const {
        return _children.size();
    }

    template<class DataType>
    typename std::vector<std::shared_ptr<TreeNode<DataType>>>::iterator TreeNode<DataType>::childrenBegin() {
        return _children.begin();
    }

    template<class DataType>
    typename std::vector<std::shared_ptr<TreeNode<DataType>>>::iterator TreeNode<DataType>::childrenEnd() {
        return _children.end();
    }

    template<class DataType>
    void TreeNode<DataType>::clearChildren() {
        _children.clear();
    }

    template<class DataType>
    void TreeNode<DataType>::disconnectParent() {
        // Remove "node" from its parent's list of children.
        if (_parent != nullptr) {
            for (int i = 0; i < _parent->nChildren(); ++i) {
                if (_parent->child(i) == this) {
                    _parent->removeChild(i);
                    break;
                }
            }
        }

        // Remove set node's parent to null.
        _parent = nullptr;
    }

}
