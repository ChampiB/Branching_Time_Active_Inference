//
// Created by Theophile Champion on 25/06/2021.
//

#ifndef BTAI_GRAPHVIZ_H
#define BTAI_GRAPHVIZ_H

#include <string>
#include <memory>
#include <vector>
#include <fstream>

namespace btai::graphs {
    class TreeNode;
}

namespace btai::graphs {

    class GraphViz {
    public:
        /**
         * Constructor. Open the output and write graph opening statement into it.
         * @param file_name output file name
         */
        explicit GraphViz(const std::string &file_name);

        /**
         * Destructor. Write the closing statement in the output file and close the output file.
         */
        ~GraphViz();

        /**
         * Write the sub-tree below "root" in the file using the GraphViz format.
         * @param dvn the default name of nodes.
         * @param root the root of the tree to write in the file.
         * @param display list or attributes to be displayed.
         */
        void writeTree(
            std::pair<std::string,int> &dvn,
            const std::shared_ptr<TreeNode> &root,
            const std::vector<std::string> &display,
            const std::string &parentName = ""
        );

        /**
         * Write a node in the file using the GraphViz format.
         * @tparam DataType the type of the data contained in the node.
         * @param name the node's name.
         * @param node the node.
         * @param display the list of attributes to be displayed.
         */
        void writeNode(
            std::string &name,
            const std::shared_ptr<TreeNode> &node,
            const std::vector<std::string> &display
        );

    private:
        /**
         * Getter.
         * @param default_name a pair describing the default name to be used.
         * @return the default name of the node.
         */
        static std::string getName(std::pair<std::string, int> &default_name);

    private:
        std::string _file_name;
        std::ofstream _file;
    };

}

#endif //BTAI_GRAPHVIZ_H
