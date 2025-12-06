CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Все поддиректории с алгоритмами
TREAP_DIR = treap
AA_TREE_DIR = aa_tree
SCAPEGOAT_DIR = scapegoat_tree

.PHONY: all clean treap aa_tree scapegoat

all: treap aa_tree scapegoat

treap:
	$(CXX) $(CXXFLAGS) $(TREAP_DIR)/simple_treap.cxx -o $(TREAP_DIR)/treap

aa_tree:
	$(CXX) $(CXXFLAGS) $(AA_TREE_DIR)/simple_aa_tree.cxx -o $(AA_TREE_DIR)/aa_tree

scapegoat:
	$(CXX) $(CXXFLAGS) $(SCAPEGOAT_DIR)/simple_scapegoat.cxx -o $(SCAPEGOAT_DIR)/scapegoat

clean:
	rm -f $(TREAP_DIR)/treap $(AA_TREE_DIR)/aa_tree $(SCAPEGOAT_DIR)/scapegoat

