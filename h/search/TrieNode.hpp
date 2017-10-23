#ifndef H_SEARCH_TRIE_NODE
#define H_SEARCH_TRIE_NODE

namespace search {

	template <typename T>
	class TrieNode {

	public:
		bool flag;
		TrieNode * left;
		TrieNode * right;
		TrieNode():flag(false),left(NULL),right(NULL){}
	} ;

}

#endif // H_SEARCH_TRIE_NODE
