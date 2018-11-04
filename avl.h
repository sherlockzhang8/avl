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

template<typename T> class tree;

template<typename T>
class node
{
	friend class tree<T>;
public:
	node() = default;
	node(T &&dt) : data(std::forward<T>(dt)) {}
	node(const T& dt) : data(dt) {}
	~node();

	const T & get() const & { return data; }
	T && get() && { return std::move(data); }
	void set(T&& dt) { data = std::forward<T>(data); }
	
private:
	T data;
	node* left = nullptr;
	node* right = nullptr;
};

template <typename T>
node<T>::~node()
{
	delete left;
	delete right;
}

template<typename T>
class tree
{
public:
	tree() = default;
	tree(const T& t) = delete;
	~tree();
	void insert(const T&);
	void remove(node<T> *&nd);
	node<T>* search(const T&);
	void preorder(std::function<void(const T&)> = nullptr);
	void inorder(std::function<void(const T&)> = nullptr);
	void postorder(std::function<void(const T&)> = nullptr);
	std::ostream& print(std::ostream & os = std::cout, bool show_pos = false);

private:
	void insert_impl(node<T> *&, const T&);
	void print_impl(node<T>* tr, size_t&, size_t&, bool = true);
	node<T>* search_impl(node<T>* tr,const T&);
	void preorder_impl(node<T>* tr, std::function<void(const T&)> = nullptr);
	void inorder_impl(node<T>* tr, std::function<void(const T&)> = nullptr);
	void postorder_impl(node<T>* tr, std::function<void(const T&)> = nullptr);
	node<T>* rotate_ll(node<T> *);
	node<T>* rotate_lr(node<T> *);
	node<T>* rotate_rl(node<T> *);
	node<T>* rotate_rr(node<T> *);
	int depth(node<T> *);
	size_t max_width(node<T> *);

private:
	node<T>* root = nullptr;
	std::vector<std::vector<std::tuple<bool,size_t, T>>> line_word_lr_witdh_data;
};

template <typename T>
tree<T>::~tree()
{
	remove(root); 
}

template<typename T>
void tree<T>::insert(const T& t)
{
	insert_impl(root, t);
}

template<typename T>
void tree<T>::remove(node<T> *&nd)
{
	delete nd;
	nd = nullptr;
}

template<typename T>
void tree<T>::insert_impl(node<T> *&tr, const T & t)
{
	if (tr == nullptr)
		tr = new node<T>(t);
	else if (t < tr->data)
	{
		insert_impl(tr->left, t );
		if (depth(tr->left) - depth(tr->right) > 1)
			if (t < tr->left->data)
				tr = rotate_ll(tr);
			else
				tr = rotate_lr(tr);
	}
	else
	{
		insert_impl(tr->right, t);
		if (depth(tr->right) - depth(tr->left) > 1)
			if (t < tr->right->data)
				tr = rotate_rl(tr);
			else
				tr = rotate_rr(tr);
	}
}

template<typename T>
node<T> *tree<T>::search(const T& t)
{
	return search_impl(root, t);
}

template<typename T>
node<T>* tree<T>::search_impl(node<T>* tr,const T& t)
{
	if (!tr || tr->data == t)
	{
		return tr;
	}

	if(t < tr->data)
	{
		return search_impl(tr->left, t);
	}
	return search_impl(tr->right, t);
}
template<typename T>
void tree<T>::preorder(std::function<void(const T&)> f)
{
	preorder_impl(root, f);
}

template<typename T>
void tree<T>::inorder(std::function<void(const T&)> f)
{
	inorder_impl(root, f);
}

template<typename T>
void tree<T>::postorder(std::function<void(const T&)> f)
{
	postorder_impl(root, f);
}

template<typename T>
void tree<T>::preorder_impl(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		f(tr->data);
		preorder_impl(tr->left);
		preorder_impl(tr->right);
	}
}

template<typename T>
void tree<T>::inorder_impl(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		inorder_impl(tr->left);
		f(tr->data);
		inorder_impl(tr->right);
	}
}

template<typename T>
void tree<T>::postorder_impl(node<T>* tr, std::function<void(const T& t)> f)
{
	if (f == nullptr)
		f = [](const T& t) {std::cout << t << " "; };
	if (tr != nullptr)
	{
		postorder_impl(tr->left);
		postorder_impl(tr->right);
		f(tr->data);
	}
}

template<typename T>
node<T>* tree<T>::rotate_ll(node<T>* nd)
{
	auto tr = nd->left;
	nd->left = tr->right;
	tr->right = nd;
	return tr;
}

template<typename T>
node<T>* tree<T>::rotate_rr(node<T>* nd)
{
	auto tr = nd->right;
	nd->right = tr->left;
	tr->left = nd;
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
void tree<T>::print_impl(node<T>* tr, size_t &i, size_t &j, bool left)
{
	if (tr != nullptr)
	{
		while(line_word_lr_witdh_data.size() < i + 1)
		{
			std::vector<std::tuple<bool,size_t, T>> vt;
			line_word_lr_witdh_data.push_back(vt);
		}
		line_word_lr_witdh_data[i].push_back(std::tuple<bool,size_t, T>(left, j, tr->data));
		auto temp = j;
		++i;
		print_impl(tr->left, i, j,true);
		++i;
		j = temp + static_cast<size_t>(pow(2, depth(root) - i));
		print_impl(tr->right, i, j,false);
	}
	--i;
}

template<typename T>
std::ostream& tree<T>::print(std::ostream &os, bool show_pos)
{
	line_word_lr_witdh_data.clear();
	size_t x = 0, y = 0;
	print_impl(root, x, y);
	size_t width = static_cast<size_t>(pow(2, depth(root)));
	size_t data_width = max_width(root);

	if (show_pos)
	{
		os << "depth: " << depth(root) << " data width: " << data_width << std::endl;
		for (size_t i = 0; i != line_word_lr_witdh_data.size(); ++i)
		{
			for (auto &j : line_word_lr_witdh_data[i])
			{
				os << '(' << std::get<1>(j) + width / static_cast<size_t>(pow(2, i + 1)) << ')' << std::get<2>(j) << ' ';
			}
			os << std::endl;
		}
	}

	for (auto a = 0u; a != width * data_width + 1; ++a)
	{
		os << '-';
	}
	os << std::endl;
	for (size_t i = 0; i != line_word_lr_witdh_data.size(); ++i)
	{
		const size_t pos = width / static_cast<size_t>(pow(2, i + 1));
		if (i > 0)
		{
			for (size_t a = 0; a != pos; ++a)
			{
				os << std::setw(data_width) << " ";
			}
			for (size_t j = 0; j != line_word_lr_witdh_data[i].size(); ++j)
			{
				size_t a;
				if (j == 0)
					a = 0;
				else
					a = std::get<1>(line_word_lr_witdh_data[i][j - 1]) + 1;
				for (; a < std::get<1>(line_word_lr_witdh_data[i][j]); ++a)
				{
					os << std::setw(data_width) << " ";
				}
				if (std::get<0>(line_word_lr_witdh_data[i][j]))
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
		for (size_t j = 0; j != line_word_lr_witdh_data[i].size(); ++j)
		{
			size_t a;
			if (j == 0)
				a = 0;
			else
				a = std::get<1>(line_word_lr_witdh_data[i][j - 1]) + 1;
			for (; a < std::get<1>(line_word_lr_witdh_data[i][j]); ++a)
			{
				os << std::setw(data_width) << " ";
			}
			os << std::setw(data_width) << std::left << std::get<2>(line_word_lr_witdh_data[i][j]);
		}
		os << std::endl;
	}
	for (auto a = 0u; a != width * data_width + 1; ++a)
	{
		os << '-';
	}
	os << std::endl;
	return os;
}

template<typename T>
int tree<T>::depth(node<T> *nd)
{
	if(!nd)
	{
		return 0;
	}
	return std::max(depth(nd->left), depth(nd->right)) + 1;
}

template<typename T>
size_t tree<T>::max_width(node<T> *nd)
{
	if(!nd)
	{
		return 0;
	}

	std::ostringstream oss;
	oss << nd->data;
	return std::max({oss.str().size(), max_width(nd->left), max_width(nd->right)});
}

#endif