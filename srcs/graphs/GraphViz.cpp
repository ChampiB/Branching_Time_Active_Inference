//
// Created by Theophile Champion on 25/06/2021.
//

#include "GraphViz.h"
#include "graphs/TreeNode.h"

using namespace btai::graphs;

namespace btai::graphs {

    GraphViz::GraphViz(const std::string &file_name) {
        _file.open(file_name);
        _file << "digraph G {\n";
    }

    GraphViz::~GraphViz() {
        _file << "}\n";
        _file.close();
    }

    void GraphViz::writeTree(
        std::pair<std::string, int> &dvn,
        const std::shared_ptr<TreeNode> &root,
        const std::vector<std::string> &display,
        const std::string &parentName
    ) {
        std::string rootName = getName(dvn);

        // Add "parentName -> rootName" to the file, or "rootName" if root does not have a parent.
        if (parentName.empty()) {
            _file << "\t" << rootName << "\n";
        } else {
            _file << "\t" << parentName << " -> " << rootName << "\n";
        }
        // Write the root data in the file.
        writeNode(rootName, root, display);
        // Write each child recursively in the file.
        for (int i = 0; i < root->nChildren(); ++i) {
            GraphViz::writeTree(dvn, *(root->childrenBegin() + i), display, rootName);
        }
    }

    void GraphViz::writeNode(
        std::string &name,
        const std::shared_ptr<TreeNode> &node,
        const std::vector<std::string> &display
    ) {
        if (display.empty())
            return;
        // Add "name_data -> name" to the file.
        _file << "\t" << name << "_data -> " << name << " [dir=none,style=dashed,color=\"gray\"]\n";
        // Create the label and display the node's data
        std::string label = R"(<<table border="0" cellborder="1" cellspacing="0" cellpadding="4">)";
        for (auto i : display) {
            label += "<TR><TD bgcolor=\"YellowGreen\">" + i + "</TD>" + \
            "<TD bgcolor=\"YellowGreen\">" + node->get(i) + "</TD></TR>";
        }
        label += "</table>>";
        _file << "\t" << name << "_data [shape=none,margin=0,label=" << label << "]\n";

    }

    std::string GraphViz::getName(std::pair<std::string, int> &default_name) {
        std::string res;

        res = default_name.first + std::to_string(default_name.second);
        ++default_name.second;
        return res;
    }

}
