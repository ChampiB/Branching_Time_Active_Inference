//
// Created by Theophile Champion on 01/08/2021.
//

#ifndef BTAI_TREE_NODE_H
#define BTAI_TREE_NODE_H

#include <memory>
#include <vector>
#include <torch/torch.h>

namespace btai::graphs {

    class TreeNode {
    public:
        /**
         * Factory.
         * @param action the node's action.
         * @param cost the node's cost.
         * @param beliefs the node's posterior beliefs.
         * @param visits the node's number of visits.
         * @return the created node.
         */
        static std::shared_ptr<TreeNode> create(const torch::Tensor &beliefs, int action = -1, double cost = 0, int visits = 1);

        /**
         * Constructor.
         * @param action the node's action.
         * @param cost the node's cost.
         * @param beliefs the node's beliefs.
         * @param visits the node's number of visits.
         */
        explicit TreeNode(const torch::Tensor &beliefs, int action, double cost, int visits);

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
         * @return the node's number of visits.
         */
        [[nodiscard]] int visits() const;

        /**
         * Getter.
         * @return the node's beliefs.
         */
        [[nodiscard]] torch::Tensor beliefs() const;

        /**
         * Getter.
         * @return the node's cost.
         */
        [[nodiscard]] double cost() const;

        /**
         * Getter.
         * @return the node's action.
         */
        [[nodiscard]] int action() const;

        /**
         * Getter.
         * @param attributeName the name of the attribute whose value should be returned.
         * @return the value of the attribute.
         */
        std::string get(const std::string &attributeName);

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
         * Increase the number of visits of the node.
         */
        void increaseVisitCount();

        /**
         * Add cost to node.
         * @param cost the additional cost.
         */
        void addCost(double cost);

        /**
         * Setter.
         * @param beliefs new node's beliefs.
         */
        void setBeliefs(const torch::Tensor &beliefs);

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
        // Pointers to parent and child.
        TreeNode *_parent;
        std::vector<std::shared_ptr<TreeNode>> _children;

        // Node's data.
        int _visits;
        int _action;
        double _cost;
        torch::Tensor _beliefs;
    };
}

#endif //BTAI_TREE_NODE_H
