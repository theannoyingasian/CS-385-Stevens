/*******************************************************************************
 * Name          : testrbt.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.2.1
 * Date          : October 8, 2014
 * Last modified : June 19, 2017
 * Description   : Driver program to test implementation of red-black tree.
 ******************************************************************************/
#include "rbtree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

template <typename K>
std::string vector2string(vector<K> v) {
    std::ostringstream oss;
    oss << "[";
    size_t size = v.size();
    for(size_t i = 0; i < size; i++) {
        oss << v[i];
        if(i != size - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

// Try to find every key in the RB tree, to test the correctness of the find function.
template <typename K, typename V>
void test_find_helper(RedBlackTree<K, V> *rbt, vector<K> v) {
    for(size_t i = 0; i < v.size(); i++) {
        if (rbt->find(v[i])->key() != v[i]) {
            ostringstream oss;
            oss << "Cannot find key '" << v[i] << "' in tree.";
            throw tree_exception(oss.str());
        }
    }
}

template <typename K, typename V>
void test_find(Tree<K> *tree) {
    RedBlackTree<K, V> *rbt = static_cast<RedBlackTree<K, V> *>(tree);
    test_find_helper<K, V>(rbt, rbt->inorder_traversal());
}

template <typename K, typename V>
void display_stats(Tree<K> *tree) {
    cout << tree->to_ascii_drawing() << endl << endl;
    cout << "Height:                   " << tree->height() << endl;
    cout << "Total nodes:              " << tree->size() << endl;
    cout << "Leaf count:               " << tree->leaf_count() << endl;
    cout << "Internal nodes:           " << tree->internal_node_count() << endl;
    cout << "Diameter:                 " << tree->diameter() << endl;
    cout << "Maximum width:            " << tree->max_width() << endl;
    cout << fixed;
    cout << "Successful search cost:   " << setprecision(3)
         << tree->successful_search_cost() << endl;
    cout << "Unsuccessful search cost: " << setprecision(3)
         << tree->unsuccessful_search_cost() << endl;
    cout << "Inorder traversal:        " << vector2string<K>(tree->inorder_traversal())
         << endl;

    try {
        test_find<K, V>(tree);
    } catch (const tree_exception &te) {
        cerr << "Error: " << te.what() << endl;
    }
}

int main(int argc, char *argv[]) {
    bool using_ints = false;
    if (argc > 1) {
        istringstream iss(argv[1]);
        int value;
        if (iss >> value) {
            using_ints = true;
        }
    }

    if (using_ints) {
        RedBlackTree<int, int> *rbt = new RedBlackTree<int, int>();
        for (int i = 1; i < argc; ++i) {
            istringstream iss(argv[i]);
            int value;
            if (!(iss >> value)) {
                cerr << "Error: Invalid integer '" << argv[i]
                     << "' found at index " << i << "." << endl;
                goto cleanup;
            }
            try {
                rbt->insert(value, value);
            } catch (const tree_exception &te) {
                cerr << "Warning: " << te.what() << endl;
            }
        }
        display_stats<int, int>(rbt);
        cleanup:
            delete rbt;
    } else {
        RedBlackTree<string, string> *rbt = new RedBlackTree<string, string>();
        for (int i = 1; i < argc; ++i) {
            try {
                rbt->insert(argv[i], argv[i]);
            } catch (const tree_exception &te) {
                cerr << "Warning: " << te.what() << endl;
            }
        }
        display_stats<string, string>(rbt);
        delete rbt;
	}

    return 0;
}
