#ifndef H_SEARCH_TRIE
#define H_SEARCH_TRIE

#include "search/TrieNode.hpp"

namespace search {

	template <typename T>
	class Trie {

	private:
		TrieNode root;

	public:
		Trie():root(){}
		/**
		 * Inserts if not already there. Returns true if already in the trie, false otherwise.
		 */
		bool insert(T* pt, T* const end){
			TrieNode* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else {
						node->flag = true;
						return false;
					}
				}
				
				TrieNode* child = (*pt == 0) ? node->left : node->right;

				if ( child == NULL ) {
					insert_suffix(node, pt, end);
					return false;
				}

				++pt; node = child ;

			}

		}

		bool search(T * pt, T* const end){
			TrieNode* node(&this->root);
			while ( true ) {

				if ( pt == end ) {
					if ( node->flag ) return true;
					else return false;
				}
				
				TrieNode* child = (*pt == 0) ? node->left : node->right;

				if ( child == NULL ) return false;

				++pt; node = child ;

			}

		}

		void insert_suffix(TrieNode* const parent, T* const pt, T* const end) {
			if ( pt == end ) parent->flag = true;
			else {
				TrieNode* next = new TrieNode();
				if ( *pt == 0 ) parent->left = next;
				else parent->right = next;
				insert_suffix(next,pt+1,end);
			}
		}

		void erase(TrieNode* node){
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

		~Trie(){
			this->clear();
	    }
		
	} ;


}

#endif // H_SEARCH_TRIE
