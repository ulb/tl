#ifndef H_SEARCH_BTRIE
#define H_SEARCH_BTRIE

namespace search {

	template <int B>
	class BTrieNode {

	public:
		bool flag;
		BTrieNode<B>* children[B];
		BTrieNode():flag(false),children(){}
	} ;


	template <typename T, int B>
	class BTrie {

	private:
		BTrieNode<B> root;

	public:
		BTrie():root(){}
		/**
		 * Inserts if not already there. Returns true if already in the trie, false otherwise.
		 */
		bool insert(T* pt, T* const end){
			BTrieNode<B>* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else {
						node->flag = true;
						return false;
					}
				}

				BTrieNode<B>* child = node->children[*pt];

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
		bool search(T* pt, T* const end){
			BTrieNode<B>* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else return false;
				}

				BTrieNode<B>* child = node->children[*pt];

				if ( child == NULL ) return false;

				++pt; node = child ;

			}

		}

		/**
		 * Assumes pt != end.
		 */
		void insert_suffix(BTrieNode<B>* parent, T* pt, T* const end) {

			do {
				BTrieNode<B>* next = new BTrieNode<B>();
				parent->children[*pt] = next;
				parent = next;
			} while ( ++pt != end ) ;

			parent->flag = true;

		}

		void erase(BTrieNode<B>* node){
			if ( node == NULL ) return ;
			BTrieNode<B>** child = node->children;
			BTrieNode<B>** end = child + B;
			while ( child != end ) this->erase(*(child++));
	        delete node;
		}

		void clear() {
			BTrieNode<B>** child = this->root.children;
			BTrieNode<B>** end = child + B;
			while ( child != end ) this->erase(*(child++));
	        this->root.flag = false;
		}

		~BTrie(){
			this->clear();
	    }

	} ;


}

#endif // H_SEARCH_BTRIE
