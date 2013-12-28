#include <iostream>
#include <cstdlib>
#include <cstdio>

//////////////////////////////////////

struct null_type;

template<int k1, int k2, class Left, class Right>
struct node {
	enum { x = k1, y = k2 };
	typedef Left left;
	typedef Right right;
};

template<bool b, class T, class U> 
struct select {
	typedef T result;
};

template<class T, class U> 
struct select<false, T, U> 
{
	typedef U result;
};

// SPLIT
////////////////
template<class root, int key>
struct split;

template<int key>
struct split<null_type, key> {
	typedef null_type left_tree;
	typedef null_type right_tree; 
};

template<int x, int y, class Left, class Right, int key> 
struct split<node<x, y, Left, Right>, key>
{
	typedef split<Left, key> tmp1;
	typedef class tmp1::left_tree  left_tree1;
	typedef node<x, y, class tmp1::right_tree, Right> right_tree1;

	typedef split<Right, key> tmp2;
	typedef node<x, y, Left, class tmp2::left_tree> left_tree2;
	typedef class tmp2::right_tree right_tree2;

	typedef class select< (x >= key), left_tree1, left_tree2>::result left_tree;
	typedef class select< (x >= key), right_tree1, right_tree2>::result right_tree;
};

// MERGE
////////////////
template <class l, class r>
struct merge;

template<int x1, int y1, class Left1, class Right1> 
struct merge<node<x1, y1, Left1, Right1>, null_type> {
	typedef node<x1, y1, Left1, Right1> result;
};

template<int x2, int y2, class Left2, class Right2>
struct merge<null_type, node<x2, y2, Left2, Right2> > {
	typedef node<x2, y2, Left2, Right2> result;
};

template <int x1, int y1, class Left1, class Right1, int x2, int y2, class Left2, class Right2>
struct merge<node<x1, y1, Left1, Right1>, node<x2, y2, Left2, Right2> > {
	
	typedef class merge<Right1, node<x2, y2, Left2, Right2> >::result right_sub_tree;
	typedef node<x1, y1, Left1, right_sub_tree> result1;

 	typedef class merge<node<x1, y1, Left1, Right1>, Left2>::result left_sub_tree;
 	typedef node<x2, y2, left_sub_tree, Right2> result2;

	typedef class select<(y1 > y2), result1, result2>::result result;	
};       


//SIZE
////////////////
/*template<class T>
struct size;

template <>
struct size<null_type> {
	enum { result = 0 };
};

template<int x, int y, class Left, class Right>
struct size<node <x, y, Left, Right> > {
	enum { result = 1 + size<Left>::result + size<Right>::result };
};   */

//RANDOM
///////////////
template<int N>
struct randomm {
	enum { result = (N * 214013 + 2531011) % 65536 };
};

/*template<>
struct randomm<0> {
	enum { result = 0 }; 
};*/

// INSERT
//////////
template <class root, int value>
struct insert;

template <int value>
struct insert<null_type, value> {
	//enum { count = size<null_type>::result + 1};
	enum { rand_num = randomm<value>::result };
	typedef node<value, rand_num, null_type, null_type> result;
};

template<int x, int y, class Left, class Right, int value>
struct insert<node <x, y, Left, Right>, value> {
	
	typedef node<x, y, Left, Right> root;
	//enum { count = size<root>::result + 1 };
	enum { rand_num = randomm<value>::result };
	typedef class split<root, value> tmp;

	typedef class merge<class tmp::left_tree, node<value, rand_num, null_type, null_type> >::result result1;
	typedef class merge<result1, class tmp::right_tree>::result result;
}; 

// ERASE
/////////
template<class root, int value>
struct erase;

template<int x, int y, class Left, class Right, int value> 
struct erase<node<x, y, Left, Right>, value> 
{
	typedef node<x, y, Left, Right> root;

	typedef class split<root, value+1> tmp;
	typedef class split<class tmp::left_tree, value> tmp1;

	typedef class merge<class tmp1::left_tree, class tmp::right_tree>::result result;
};

// FIND
//////////
template <class root, int value>
struct find;

template<int value>
struct find<null_type, value> {
	enum { result = false };
};

template<int x, int y, class Left, class Right>
struct find<node<x, y, Left, Right>, x> {
	enum { result = true };
};

template<int x, int y, class Left, class Right, int value> 
struct find<node<x, y, Left, Right>, value>
{
	enum { result1 = find<Left, value>::result };
	enum { result2 = find<Right, value>::result };

	enum { result = (value < x) ? result1 : result2 };
};

// OUTPUT
//////////

template<class T> 
struct output_treap;

template<>
struct output_treap<null_type> {
	void operator()() {
	}
};

template<int x, int y, class Left, class Right> 
struct output_treap<node<x, y, Left, Right> >
{
	void operator()() {
		output_treap<Left>()();
		std::cout << x << ' ' << y << ' ' << std::endl;
		output_treap<Right>()();
	}	
};

/////////////////////
typedef null_type tree0;
typedef insert< tree0 , 7> tree1;
typedef insert< tree1::result, 4> tree2;
typedef insert < tree2::result, 6> tree3;
typedef insert < tree3::result, 5> tree4;
typedef insert < tree4::result, 2> tree5;
typedef insert < tree5::result, 0> tree6;
typedef insert < tree6::result, 3> tree7;
typedef insert < tree7::result, 13> tree8;
typedef insert < tree8::result, 14> tree9;
typedef insert < tree9::result, 9> tree10;
typedef insert < tree10::result, 11> tree11;

int main() {
	
	std::cout << "Print all tree" << std::endl;
	output_treap<tree11::result>()();
	//std::cout << std::endl;
	
	std::cout << "Find elements" << std::endl;
	std::cout << find<tree11::result, 7>::result << std::endl;
	std::cout << find<tree11::result, 4>::result << std::endl;
	std::cout << find<tree11::result, 6>::result << std::endl;
	std::cout << find<tree11::result, 5>::result << std::endl;
	std::cout << find<tree11::result, 2>::result << std::endl;
	std::cout << find<tree11::result, 0>::result << std::endl;
	std::cout << find<tree11::result, 3>::result << std::endl;
	std::cout << find<tree11::result, 13>::result << std::endl;
	std::cout << find<tree11::result, 14>::result << std::endl;
	std::cout << find<tree11::result, 9>::result << std::endl;
	std::cout << find<tree11::result, 11>::result << std::endl;
	//std::cout << std::endl;

	std::cout << "Elements don't contains" << std::endl;
	std::cout << find<tree11::result, 239>::result << std::endl;
	std::cout << find<tree11::result, -100500>::result << std::endl;
	std::cout << find<tree11::result, 2539>::result << std::endl;
	//std::cout << std::endl;

	std::cout << "Erase check" << std::endl;
	std::cout << find<erase<tree11::result, 2>::result, 2>::result << std::endl;
	std::cout << find<erase<tree11::result, 7>::result, 7>::result << std::endl;
	std::cout << find<erase<tree11::result, 0>::result, 0>::result << std::endl;
	std::cout << find<erase<tree11::result, 11>::result, 11>::result << std::endl;
	//std::cout << std::endl;

	std::cout << "Erase element not contained" << std::endl;
	output_treap<erase<tree11::result, 239>::result>()(); 

}