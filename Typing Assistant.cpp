#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h> 
#include <cctype>
using namespace std;

void cls(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

class Trie
{
private:
	struct node
	{
		bool end;
		node* child[26];
		node()
		{
			for (int i = 0; i < 26; i++)
			{
				child[i] = NULL;
			}
			end = false;
		}
	};
	node* head;
public:
	Trie()
	{
		head = new node();
		
	}
	void insert(string word)
	{
		node* current = head;

		for (unsigned int i = 0; i < word.length(); i++)
		{
			int letter = (int)word[i] - (int)'a';
			if (current->child[letter] == NULL)
			{
				current->child[letter] = new node();
			}
				
			current = current->child[letter];
		}
		current->end = true;
	}
	bool search(string word)
	{
		node* current = head;

		for (unsigned int i = 0; i < word.length(); i++)
		{
			int letter = (int)word[i] - (int)'a';
			if (current->child[letter] == NULL)
			{
				return false;
			}
			current = current->child[letter];
		}
		return current->end;
	}
	bool isEmpty(node *h)
	{
		for (int i = 0; i < 26; i++)
			if (h->child[i])
				return false;
		return true;
	}
	node* remove(node *h, string key, int depth = 0)
	{
		
		if (!h) //IF TREE IS EMPTY
			return NULL;

		if (depth == key.size()) //IF LAST CHARACTER IS OF KEY IS BEING CHECKED
		{
			if (h->end) //THIS NODE IS NO MORE END OF THE WORD AFTER REMOVAL OF GIVEN KEY 
				h->end = false;

			if (isEmpty(h)) //IF GIVEN NODE IS NOT PREFIX OF ANY OTHER WORD 
			{
				delete (h);
				h = NULL;
			}
			return h;
		}

		int index = key[depth] - 'a';
		h->child[index] = remove(h->child[index], key, depth + 1); //IF NOT THE LAST CHARACHTER OF THE WORD, RECURSION CALLED FOR CHILD NODE
 
		if (isEmpty(h) && h->end == false) //IF ROOT DOES NOT HAVE ANY CHILD AND NOR IT IS THE END OF THE ANOTHER WORD
		{
			delete (h);
			h = NULL;
		}
		return h;
	}
	void del(string w)
	{
		head = remove(head, w);
	}
	bool isLastNode(node* root)
	{
		for (int i = 0; i < 26; i++)
			if (root->child[i])
				return 0;
		return 1;
	}
	void suggestionsRec(node* root, string currPrefix, int *j, string w[])
	{
		if (root->end) //IF A STRING IS FOUND IN THE TRIE WITH THE GIVEN PREFIX
		{
			if ((*j) < 4) //IF ARRAY DOES NOT EXCEED ITS RANGE
			{
				w[*j] = currPrefix;
				(*j)++;
			}
		}
		if (isLastNode(root)) //IF ALL CHILD NODES ARE NULL
			return;

		for (int i = 0; i < 26; i++)
		{
			if (root->child[i])
			{
				currPrefix.push_back(97 + i); //APPEND CUURENT CHARACTER TO STRING
				suggestionsRec(root->child[i], currPrefix, j, w); //RECURSION CALLED
				currPrefix.pop_back(); //REMOVE LAST CHARACTER
			}
		}
	}
	void printSuggestions(string query, int* j, string w[])
	{
		node* temp = head;

		int level;
		for (level = 0; level < query.length(); level++) // CHECKS IF PREFIX IS PRESENT AND FIND THE NODE WITH LAST CHARACTER OF THE GIVEN STRING
		{
			int index = ((int)query[level] - (int)'a');
			if (!temp->child[index]) //IF NO STRING IN TRIE HAS THIS PREFIX
				return;
			temp = temp->child[index];
		}
		bool isWord = (temp->end == true); //IF PREFIX IS PRESENT AS A WORD
		bool isLast = isLastNode(temp); //IF PREFIX HAS NO CHILD NODES

		if (isWord && isLast) //IF PREFIX IS PRESENT AS A WORD AND HAVE NO CHILD NODES
		{
			if ((*j) < 4)
			{
				w[*j] = query;
				(*j)++;
			}
			return;
		}
		if (!isLast)
		{
			string prefix = query;
			suggestionsRec(temp, prefix, j, w);
			return;
		}
	}
};

class Used_Words
{
private:
	struct node
	{
		int freq;
		string word;
		node* next;
		node()
		{
			next = NULL;
			freq = 0;
		}
	};
	node* head;
public:
	Used_Words()
	{
		head = NULL;
	}
	void addnode(string w)
	{
		node* ptr = new node;
		ptr->word = w;
		ptr->freq++;
		
		if (head == NULL)
		{
			head = ptr;
		}
		else
		{
			node* temp = head;
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = ptr;
		}
		
	}
	void search(string w)
	{
		if (head == NULL)
		{
			addnode(w);
		}
		else
		{
			node* temp = head;

			while (temp != NULL && temp->word != w)
			{
				temp = temp->next;
			}
			if (temp == NULL)
			{
				addnode(w);
			}
			else
			{
				temp->freq++;
			}
		}
	}
	void print(string w, string dw[], Trie *t)
	{
		sort(); //SORTS THE LINKED LIST IN DESCENDING ORDER ACCORDING TO THE FREQUENCY
		
		system("CLS");
		for (int i = 0; i < 20; i++)
		{
			cout << endl;
		}
		node* temp = head;
		int i = 0;
		for (; i < 4 && temp != NULL; i++) //PRINTS THE WORDS ALREADT USED
		{
			dw[i] = temp->word;
			temp = temp->next;
		}
		t->printSuggestions(w, &i, dw); //PRINTS THE WORDS IN TRIE
		
		for (int i = 0; i < 4; i++)
		{
			cout << i + 1 << ". " << dw[i] << "\t\t";
		}

		HANDLE hStdout;
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE); //USED TO REPOSITION CURSOR AT THE TOP LEFT OF THE CONSOLE
		cls(hStdout);
	}
	node* swap(node* ptr1, node* ptr2)
	{
		node* tmp = ptr2->next;
		ptr2->next = ptr1;
		ptr1->next = tmp;
		return ptr2;
	}
	void sort()
	{
		node** h = &head;
		int i, j, swapped, count = 0;

		while(*h != NULL)
		{
			count++;
			h = &(*h)->next;
		}
		for (i = 0; i <= count; i++)
		{

			h = &head;
			swapped = 0;

			for (j = 0; j < count - i - 1; j++)
			{

				node* p1 = *h;
				node* p2 = p1->next;

				if (p1->freq < p2->freq)
				{
					*h = swap(p1, p2);
					swapped = 1;
				}

				h = &(*h)->next;
			}
			if (swapped == 0)
				break;
		}
	}
	void del(Trie* t)
	{
		node* temp = head;

		while (temp != NULL)
		{
			if (t->search(temp->word))
			{
				t->del(temp->word);
			}
			temp = temp->next;
		}
	}
};

int main()
{
	Used_Words u;
	Trie t;
	string sentence, temp, displayed_words[4] = { "\0" };
	int ch = 0; 
	bool turn = true;
	fstream f;

	f.open("default_words.txt", ios::in);
	while (getline(f, sentence)) 
	{
		t.insert(sentence);
	}
	f.close();

	f.open("used_words.txt", ios::in);
	while (getline(f, sentence))
	{
		u.search(sentence);
	}
	f.close();

	u.del(&t); //DELETES ANY USED WORDS FROM THE TRIE

	while (ch != '$')
	{
		if (turn)
		{
			temp.clear();
			system("CLS");
			cout << "\nSentence: " << sentence;
			ch = _getch();
			ch = tolower(ch); //IF CAPITAL LETTER IS ENTERED, IT IS CONVERTED TO SMALL LETTER
			string tem(1, ch);
			temp.append(tem);
			cout << tem;
			u.print(tem, displayed_words, &t);
			turn = false;
		}
		else
		{
			cout << "\nSentence: " << sentence << temp;
			ch = _getch();
			cout << (char)ch;
			
			system("CLS");
			if ((char)ch >= 'a' && (char)ch <= 'z' || (char)ch >= 'A' && (char)ch <= 'Z')
			{
				ch = tolower(ch);
				string tem(1, ch);
				temp.append(tem);
				u.print(temp, displayed_words, &t);
			}
			else if ((char)ch == ' ')
			{
				u.search(temp);
				f.open("used_words.txt", ios::app);
				f << temp << endl;
				f.close();

				f.open("default_words.txt", ios::app); //ANY NEW WORD ENTERED BY USER IS UPDATED IN THE TEXT FILE 
				f << temp << endl;
				f.close();

				u.del(&t);

				sentence.append(temp);
				sentence.append(" ");

				turn = true;
			}
			else if ((char)ch >= '1' && (char)ch <= '4')
			{
				u.search(displayed_words[ch - 49]);

				sentence.append(displayed_words[ch - 49]);
				sentence.append(" ");

				f.open("used_words.txt", ios::app);
				f << displayed_words[ch - 49] << endl;
				f.close();
				
				u.del(&t);

				turn = true;
			}
			else
			{
				cout << "INVALID INPUT\n\n\n";
				exit(0);
			}
		}
	}
}