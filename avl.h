#ifndef AVL_H
#define AVL_H

#include<memory>
#include<utility>
#include<functional>
#include<cstdlib>
#include<stdexcept>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<sstream>
#include<vector>
#include<iomanip>
#include<tuple>

template<typename T> class node;

template<typename T> class tree;

template<typename T>
class node
{
	friend class tree<T>;
private:
	T data;
	int length = 0;
	node* left = nullptr;
	node* right = nullptr;
public:
	node() = default;
	node(T &&dt) : data(std::forward<T>(dt)) {}
	node(const T& dt) : data(dt) {}
	const T & get() const & { return data; }
	T && get() && { return std::move(data); }
	void set(T&& dt) { data = std::forward<T>(data); }
	int len() { return length; }
};

template<typename T>
class tree
{
public:
	tree() = default;
	tree(const T& t) :root(new node<T>(t)) {}
	tree(T &&dt) : root(new node<T>(std::forward<T>(dt))) {};
	void insert(const T&);
	void remove(node<T> *&nd);
	node<T>*& search(const T&);
	void pretrav(std::function<void(const T&)> = nullptr);
	void intrav(std::function<void(const T&)> = nullptr);
	void foltrav(std::function<void(const T&)> = nullptr);
	std::ostream& out(std::ostream & = std::cout, const size_t& = 1, const bool& = true);

private:
	void insert(node<T> *&, const T&);
	void remove(node<T>*&, node<T>*&);
	void out(node<T>* tr, size_t&, size_t&, bool = true);
	void pretrav(node<T>* tr, std::function<void(const T&)> = nullptr);
	void intrav(node<T>* tr, std::function<void(const T&)> = nullptr);
	void foltrav(node<T>* tr, std::function<void(const T&)> = nullptr);
	node<T>* rotate_ll(node<T> *);
	node<T>* rotate_lr(node<T> *);
	node<T>* rotate_rl(node<T> *);
	node<T>* rotate_rr(node<T> *);
	node<T>* root = nullptr;
	void callen(node<T> *&);
	std::vector < std::vector<std::tuple<bool,size_t, T>>> vd;
};

//inline function

template<typename T>
inline void tree<T>::insert(const T& t)
{
	insert(root, t);
}

template<typename T>
inline void tree<T>::remove(node<T> *&nd)
{
	remove(root, nd);
	callen(root);
}

template<typename T>
inline void tree<T>::pretrav(std::function<void(const T&)> f)
{
	pretrav(root, f);
}

template<typename T>
inline void tree<T>::intrav(std::function<void(const T&)> f)
{
	intrav(root, f);
}

template<typename T>
inline void tree<T>::foltrav(std::function<void(const T&)> f)
{
	foltrav(root, f);
}

template<typename T>
inline int length_of(node<T> * nd)
{
	return (nd == nullptr) ? 0 : nd->len();
}

template<typename T>
void tree<T>::insert(node<T> *&tr, const T & t)
{
	if (tr == nullptr)
		tr = new node<T>(t);
	else if (t < tr->data)
	{
		insert(tr->left, t );
		if (length_of(tr->left) - length_of(tr->right) > 1)
			if (t < tr->left->data)
				tr = rotate_ll(tr);
			else
				tr = rotate_lr(tr);
	}
	else
	{
		insert(tr->right, t);
		if (length_of(tr->right) - length_of(tr->left) > 1)
			if (t < tr->right->data)
				tr = rotate_rl(tr);
			else
				tr = rotate_rr(tr);
	}
	tr->length = std::max(length_of(tr->left), length_of(tr->right)) + 1;
}

template<typename T>
void tree<T>::remove(node<T> *&tr, node<T> *&nd)
{
	if (tr == nullptr)
		throw std::runtime_error("tree error");
	if (nd == tr)
	{
		node<T>* temp;
		if (nd->left == nullptr)
		{
			temp = nd;
			nd = nd->right;
			delete temp;
		}
		else if (nd->right == nullptr)
		{
			temp = nd;
			nd = nd->left;
			delete temp;
		}
		else
		{
			for (temp = nd->left; temp->right != nullptr; temp = temp->right)
				continue;
			temp->right = nd->right;
			temp->length = std::max(length_of(temp->left), length_of(temp->right)) + 1;
			temp = nd;
			nd = nd->left;
			delete temp;
		}
		tr = nd;
	}
	else
	{
		if (nd->data < tr->data)
		{
			remove(tr->left, nd);
		}
		else
		{
			remove(tr->right, nd);
		}
	}
}

template<typename T>
node<T> *&tree<T>::search(const T& t)
{
	if (root->data == t)
		return root;
	auto temp = root;
	while (temp != nullptr)
	{
		if (t < temp->data)
		{
			if (t == temp->left->data)
				return temp->left;
			else
				temp = temp->left;
		}
		else
		{
			if (t == temp->right->data)
				return temp->right;
			else
				temp = temp->right;
		}
	}
	throw std::runtime_error("can't find node");
}

template<typename T>
void tree<T>::pretrav(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		f(tr->data);
		pretrav(tr->left);
		pretrav(tr->right);
	}
}

template<typename T>
void tree<T>::intrav(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		intrav(tr->left);
		f(tr->data);
		intrav(tr->right);
	}
}

template<typename T>
void tree<T>::foltrav(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		foltrav(tr->left);
		foltrav(tr->right);
		f(tr->data);
	}
}

template<typename T>
node<T>* tree<T>::rotate_ll(node<T>* nd)
{
	auto tr = nd->left;
	nd->left = tr->right;
	tr->right = nd;
	nd->length = std::max(length_of(nd->left), length_of(nd->right)) + 1;
	tr->length = std::max(length_of(tr->left), nd->length) + 1;
	return tr;
}

template<typename T>
node<T>* tree<T>::rotate_rr(node<T>* nd)
{
	auto tr = nd->right;
	nd->right = tr->left;
	tr->left = nd;
	nd->length = std::max(length_of(nd->left), length_of(nd->right)) + 1;
	tr->length = std::max(length_of(tr->right), nd->length) + 1;
	return tr;
}

template<typename T>
node<T>* tree<T>::rotate_lr(node<T>* nd)
{
	nd->left = rotate_rr(nd->left);
	return rotate_ll(nd);
}

template<typename T>
node<T>* tree<T>::rotate_rl(node<T>* nd)
{
	nd->right = rotate_ll(nd->right);
	return rotate_rr(nd);
}

template<typename T>
void tree<T>::out(node<T>* tr, size_t &i, size_t &j, bool left)
{
	if (tr != nullptr)
	{
		while(vd.size() < i + 1)
		{
			std::vector<std::tuple<bool,size_t, T>> vt;
			vd.push_back(vt);
		}
		vd[i].push_back(std::tuple<bool,size_t, T>(left, j, tr->data));
		auto temp = j;
		++i;
		out(tr->left, i, j,true);
		++i;
		j = temp + size_t(pow(2, length_of(root) - i));
		out(tr->right, i, j,false);
	}
	--i;
}

template<typename T>
std::ostream& tree<T>::out(std::ostream &os, const size_t& data_width, const bool& dtl)
{
	size_t i = 0, j = 0;
	vd.clear();
	out(root, i, j);
	size_t width = size_t(pow(2, length_of(root)));
	if (dtl)
	{
		os << "height: " << length_of(root) << std::endl;
		for (size_t i = 0; i != vd.size(); ++i)
		{
			for (auto &j : vd[i])
			{
				os << '(' << std::get<0>(j) + width / size_t(pow(2, i + 1)) << ')' <<std::get<1>(j) << ' ';
			}
			os << std::endl;
		}
	}
	for (auto a = 0; a != width * data_width + 1; ++a)
	{
		os << '-';
	}
	os << std::endl;
	for (size_t i = 0; i != vd.size(); ++i)
	{
		const size_t pos = width / size_t(pow(2, i + 1));
		if (i > 0)
		{
			for (size_t a = 0; a != pos; ++a)
			{
				os << std::setw(data_width) << " ";
			}
			for (size_t j = 0; j != vd[i].size(); ++j)
			{
				size_t a;
				if (j == 0)
					a = 0;
				else
					a = std::get<1>(vd[i][j - 1]) + 1;
				for (; a < std::get<1>(vd[i][j]); ++a)
				{
					os << std::setw(data_width) << " ";
				}
				if (std::get<0>(vd[i][j]))
					os << std::setw(data_width) << std::left << '/';
				else
					os << std::setw(data_width) << std::left << '\\';
			}
			os << std::endl;
		}
		for (size_t a = 0; a != pos; ++a)
		{
			os << std::setw(data_width) << " ";
		}
		for (size_t j = 0; j != vd[i].size(); ++j)
		{
			size_t a;
			if (j == 0)
				a = 0;
			else
				a = std::get<1>(vd[i][j - 1]) + 1;
			for (; a < std::get<1>(vd[i][j]); ++a)
			{
				os << std::setw(data_width) << " ";
			}
			os << std::setw(data_width) << std::left << std::get<2>(vd[i][j]);
		}
		os << std::endl;
	}
	for (auto a = 0; a != width * data_width + 1; ++a)
	{
		os << '-';
	}
	os << std::endl;
	return os;
}

template<typename T>
void tree<T>::callen(node<T> *&tr)
{
	if (tr != nullptr)
	{
		callen(tr->left);
		callen(tr->right);
		tr->length = std::max(length_of(tr->left), length_of(tr->right)) + 1;
	}
}

#endif