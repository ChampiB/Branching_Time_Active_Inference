//
// Created by Theophile Champion on 01/08/2021.
//

#include "TreeNode.h"

using namespace torch;

namespace btai::graphs {

    std::shared_ptr<TreeNode> TreeNode::create(const Tensor &beliefs, int action, double cost, int visits) {
        return std::make_shared<TreeNode>(beliefs, action, cost, visits);
    }

    TreeNode::TreeNode(const Tensor &beliefs, int action, double cost, int visits) {
        _parent = nullptr;
        _beliefs = beliefs;
        _action = action;
        _cost = cost;
        _visits = visits;
    }

    void TreeNode::setParent(TreeNode *node) {
        _parent = node;
    }

    void TreeNode::addChild(const std::shared_ptr<TreeNode> &node) {
        _children.push_back(node);
    }

    void TreeNode::removeChild(int id) {
        _children.erase(_children.begin() + id);
    }

    int TreeNode::visits() const {
        return _visits;
    }

    double TreeNode::cost() const {
        return _cost;
    }

    int TreeNode::action() const {
        return _action;
    }

    Tensor TreeNode::beliefs() const {
        return _beliefs;
    }

    std::string TreeNode::get(const string &attributeName) {
        if (attributeName == "G")
            return std::to_string(_cost);
        if (attributeName == "U")
            return std::to_string(_action);
        if (attributeName == "N")
            return std::to_string(_visits);
        if (attributeName == "S")
            return std::to_string(torch::argmax(_beliefs).item<int>());
        throw std::runtime_error("In DataMCTS::get(key), unsupported key.");
    }

    TreeNode *TreeNode::parent() const {
        return _parent;
    }

    TreeNode *TreeNode::child(int id) const {
        return _children[id].get();
    }

    int TreeNode::nChildren() const {
        return (int) _children.size();
    }

    void TreeNode::increaseVisitCount() {
        ++_visits;
    }

    void TreeNode::addCost(double cost) {
        _cost += cost;
    }

    void TreeNode::setBeliefs(const Tensor &beliefs) {
        _beliefs = beliefs;
    }

    typename std::vector<std::shared_ptr<TreeNode>>::iterator TreeNode::childrenBegin() {
        return _children.begin();
    }

    typename std::vector<std::shared_ptr<TreeNode>>::iterator TreeNode::childrenEnd() {
        return _children.end();
    }

    void TreeNode::clearChildren() {
        _children.clear();
    }

    void TreeNode::disconnectParent() {
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
