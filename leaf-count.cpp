#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");




template <typename T> class Node
{
private:
	T key;
	Node<T> *parent, *left, *right;
	
public:
	Node(T key)
	{
		this->key = key;
		parent = left = right = NULL;
	}
	
	void setKey(T key) { this->key = key; }
	void setParent(Node<T> *parent) { this->parent = parent; }
	void setLeft(Node<T> *left) { this->left = left; }
	void setRight(Node<T> *right) { this->right = right; }
	
	T getKey() const { return key; }
	Node<T>* getParent() const { return parent; }
	Node<T>* getLeft() const { return left; }
	Node<T>* getRight() const { return right; }
	
};



template <typename K> class BST
{
private:
	int n;
	Node<K> *root;
	
	void aux_inorder(Node<K> *node, ofstream &out)
	{
		aux_inorder(node->getLeft(), out);
		out << node->getKey() << " ";
		aux_inorder(node->getRight(), out);
	}
	
	void aux_preorder(Node<K> *node, ofstream &out)
	{
		out << node->getKey() << " ";
		aux_inorder(node->getLeft(), out);
		aux_inorder(node->getRight(), out);
	}
	
	
	void aux_postorder(Node<K> *node, ofstream &out)
	{
		aux_inorder(node->getLeft(), out);
		aux_inorder(node->getRight(), out);
		out << node->getKey() << " ";
	}
	

public:
	BST()
	{
		n = 0;
		root = NULL;
	}
	
	Node<K>* getRoot() { return root; }
	
	K minimo(Node<K> *node)
	{
		if(node)	
		{
			while(node->getLeft())
				node = node->getLeft();
		}
		
		if(node)
			return node->getKey();
		else
			return -1;
	}
	
	K massimo(Node<K> *node)
	{
		if(node)
		{
			while(node->getRight())
				node = node->getRight();
		}
		
		if(node)
			return node->getKey();
		else
			return -1;
	}
	
	K getSuccessor(K key)
	{
		Node<K> *temp_node = search(root, key);		//cerchiamo il nodo di cui trovare il successore
		if(temp_node)								//se esiste
		{
			if(temp_node->getRight())			//se esiste il figlio destro "caso banale"
				return minimo(temp_node->getRight());
			
			Node<K> *parent = temp_node->getParent();	//ottengo il genitore del nodo da cancellare
			while(parent != NULL && temp_node == parent->getRight())
			{
				temp_node = parent;
				parent = parent->getParent();
			}
			
			if(parent)
				return parent->getKey();
			else
				return -1;
		}
	}
	
	
	K getPredecessor(K key)
	{
		Node<K> *temp_node = search(root, key);
		if(temp_node)
		{
			if(temp_node->getLeft())
				return massimo(temp_node->getLeft());
			
			Node<K> *parent = temp_node->getParent();
			while(parent != NULL && temp_node == parent->getLeft())
			{
				temp_node = parent;
				parent = parent->getParent();
			}
			
			if(parent)
				return parent->getKey();
			else
				return -1;
		}
	}
	
	
	
	void inorder(ofstream &out) { aux_inorder(root, out); }
	void preorder(ofstream &out) { aux_preorder(root, out); }
	void postorder(ofstream &out) { aux_postorder(root, out); }
	
	unsigned int getLeafNumber(Node<K> *node)
	{
		if(node == NULL)
			return 0;
		if(node->getLeft() == NULL && node->getRight() == NULL)
			return 1;
		else
			return getLeafNumber(node->getLeft()) + getLeafNumber(node->getRight());
	}
	
	void insert(K key)
	{
		Node<K> *node = root;
		Node<K> *parent = NULL;
		
		while(node != NULL)			//serve a impostare il parent
		{
			parent = node;
			if(key > node->getKey())
				node = node->getRight();
			else
				node = node->getLeft();
		}
		
		Node<K> *newnode = new Node<K>(key);
		n++;
		
		if(parent == NULL)
		{
			root = newnode;
			return;
		}
		
		
		newnode->setParent(parent);
		if(key <= parent->getKey())
			parent->setLeft(newnode);
		else
			parent->setRight(newnode);
	
	}
	
	Node<K>* search(Node<K> *node, K key)
	{
		if(node == NULL || node->getKey() == key)
			return node;
		if(key > node->getKey())
			search(node->getRight(), key);
		else
			search(node->getLeft(), key);
	}
	
	void canc(Node<K> *x, K key)
	{
		Node<K> *temp = search(x, key);		//cerca un nodo con la chiave key
		if(temp == NULL)					//se non viene trovato, usciamo perchè un nodo inesistente non può essere cancellato
			return;
		
		Node<K> *parent = temp->getParent();		//otteniamo il genitore del nodo da cancellare
		if(!temp->getLeft() || !temp->getRight())	//gestisce i casi 1 & 2: 0 oppure 1 solo figlio, caso "facile"
		{
			n--;									//decrementiamo il contatore dei nodi
			Node<K> *son = temp->getRight();		
			if(temp->getLeft())
				son = temp->getLeft();
			
			if(!parent)				//in un BST l'unico nodo che non ha padre è la radice
				root = son;
			else					//colleghiamo il parent al son, cioè il colleghiamo nonno e nipote, scavalcando temp
			{
				if(temp == parent->getLeft())
					parent->setLeft(son);
				else
					parent->setRight(son);
			}
			
			if(son)
				son->setParent(parent);
		
		}
		
		else									//caso 3: due figli
		{
			K next = this->getSuccessor(temp->getKey());		//cerchiamo l'immediato successore di temp
			Node<K> *succ = new Node<K>(next);
			temp->setKey(succ->getKey());						//temp assumerà la chiave del suo immediato successore
			canc(temp->getRight(), succ->getKey());				//cancelliamo l'immediato successore di temp
		}
		
	}
};


int main()
{
	string data_type, operation;
	int N;
	
	int aux_int;
	double aux_double;
	
	
	for(int i=0; i<100; i++)
	{
		in >> data_type >> N;
		
		if(data_type == "int")
		{
			BST<int> myBST;
			for(int j=0; j<N; j++)
			{
				in >> operation;
				
				if(operation.substr(0, 4) == "canc")
				{
					operation = operation.substr(5);
					aux_int = atoi(operation.c_str());
					myBST.canc(myBST.getRoot(), aux_int);
				}
				
				else
				{
					operation = operation.substr(4);
					aux_int = atoi(operation.c_str());
					myBST.insert(aux_int);
				}
			}
			
			out << myBST.getLeafNumber(myBST.getRoot()) << endl;
		}
		
		if(data_type == "double")
		{
			BST<double> myBST;
			for(int j=0; j<N; j++)
			{
				in >> operation;
				
				if(operation.substr(0, 4) == "canc")
				{
					operation = operation.substr(5);
					aux_double = atof(operation.c_str());
					myBST.canc(myBST.getRoot(), aux_double);
				}
				
				else
				{
					operation = operation.substr(4);
					aux_double = atof(operation.c_str());
					myBST.insert(aux_double);
				}
			}
			
			out << myBST.getLeafNumber(myBST.getRoot()) << endl;
		}
		
	}
	
	in.close();
	out.close();
	
	return 0;
}