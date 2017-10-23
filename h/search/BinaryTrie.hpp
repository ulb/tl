#ifndef H_SEARCH_BINARYTRIE
#define H_SEARCH_BINARYTRIE

namespace search {

	class BinaryTrieNode {

	public:
		bool flag;
		BinaryTrieNode * left;
		BinaryTrieNode * right;
		BinaryTrieNode():flag(false),left(NULL),right(NULL){}
	} ;

	template <typename T>
	class BinaryTrie {

	private:
		BinaryTrieNode root;

	public:
		BinaryTrie():root(){}
		/**
		 * Inserts if not already there. Returns true if already in the trie, false otherwise.
		 */
		bool insert(T* pt, T* const end){
			BinaryTrieNode* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else {
						node->flag = true;
						return false;
					}
				}

				BinaryTrieNode* child = (*pt == 0) ? node->left : node->right;

				if ( child == NULL ) {
					insert_suffix(node, pt, end);
					return false;
				}

				++pt; node = child ;

			}

		}

		/**
		 * Returns true if the vector is in the trie, false otherwise.
		 */
		bool search(T * pt, T* const end){
			BinaryTrieNode* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else return false;
				}

				BinaryTrieNode* child = (*pt == 0) ? node->left : node->right;

				if ( child == NULL ) return false;

				++pt; node = child ;

			}

		}

		/**
		 * Assumes pt != end.
		 */
		void insert_suffix(BinaryTrieNode* parent, T* pt, T* const end) {

			do {
				BinaryTrieNode* next = new BinaryTrieNode();
				if ( *pt == 0 ) parent->left = next;
				else parent->right = next;
				parent = next;
			} while ( ++pt != end ) ;

			parent->flag = true;

		}

		void erase(BinaryTrieNode* node){
			if ( node == NULL ) return ;
	        this->erase(node->left);
	        this->erase(node->right);
	        delete node;
		}

		void clear() {
	        this->erase(this->root.left);
	        this->erase(this->root.right);
	        this->root.flag = false;
		}

		~BinaryTrie(){
			this->clear();
	    }

	} ;


}

#endif // H_SEARCH_BINARYTRIE
