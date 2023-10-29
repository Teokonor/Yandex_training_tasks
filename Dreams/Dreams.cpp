#include <iostream>
#include <vector>
#include <fstream>

struct Node final {
    size_t parent;
    size_t left_child;
    size_t right_child;
};

void init_tree(std::vector<Node>& tree) {
    size_t nodes = tree.size() - 1;
    tree[0].parent = 1;
    for (size_t i = 1; i <= nodes; ++i) {
        tree[i].parent = i / 2;
        if (i * 2 <= nodes) {
            tree[i].left_child = i * 2;
        }
        else {
            tree[i].left_child = 0;
        }
        if (i * 2 + 1 <= nodes) {
            tree[i].right_child = i * 2 + 1;
        }
        else {
            tree[i].right_child = 0;
        }
    }
    
}

void replace_node(std::vector<Node>& tree, size_t replacing_node) {
    if (replacing_node == tree[0].parent) {
        return;
    }
    size_t parent = tree[replacing_node].parent;
    size_t grandparent = tree[parent].parent;
    if (grandparent == 0) {
        tree[0].parent = replacing_node;
    }
    else {
        if (tree[grandparent].left_child == parent) {
            tree[grandparent].left_child = replacing_node;
        }
        else if (tree[grandparent].right_child == parent) {
            tree[grandparent].right_child = replacing_node;
        }
    }
    tree[replacing_node].parent = grandparent;
    tree[parent].parent = replacing_node;
    if (replacing_node == tree[parent].left_child) {
        tree[parent].left_child = tree[replacing_node].left_child;
        if (tree[replacing_node].left_child != 0) {
            tree[tree[replacing_node].left_child].parent = parent;
        }
        tree[replacing_node].left_child = parent;
    }
    else if (replacing_node == tree[parent].right_child) {
        tree[parent].right_child = tree[replacing_node].right_child;
        if (tree[replacing_node].right_child != 0) {
            tree[tree[replacing_node].right_child].parent = parent;
        }
        tree[replacing_node].right_child = parent;
    }
}

void print_node(std::vector<Node>& tree, std::ostream& output, size_t node) {
    if (tree[node].left_child != 0) {
        print_node(tree, output, tree[node].left_child);
    }
    output << node << ' ';
    if (tree[node].right_child != 0) {
        print_node(tree, output, tree[node].right_child);
    }
}

void print_tree(std::vector<Node>& tree, std::ostream& output) {
    print_node(tree, output, tree[0].parent);
}

int main()
{
    size_t nodes, changes;
    std::ifstream input_file("input.txt");
    // Info:    tree[0].parent == root number
    input_file >> nodes >> changes;
    std::vector<Node> tree(nodes + 1);
    init_tree(tree);
    size_t replacing_node;
    for (size_t i = 0; i < changes; ++i) {
        input_file >> replacing_node;
        replace_node(tree, replacing_node);
    }
    input_file.close();
    print_tree(tree, std::cout);
    return 0;
}

