#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <bitset>
#include <queue>
#include <deque>
#include <stack>
#include <cmath>
#include <ctime>
#include <list>
#include <map>
#include <set>

using namespace std;
struct node
{
    node() : refs(UCHAR_MAX, -1), leaf_index(-1), parent(-1), suffix_link(-1), root(false), parent_letter(-1) {}

    std::vector<int> refs;

    bool root;
    int leaf_index;
    int parent;
    int suffix_link;
    unsigned char parent_letter;
};

struct aho_corasick
{
    vector<node> nodes;
    const std::vector<std::string>& forest;

    aho_corasick(const std::vector<std::string>& forest) : forest(forest)
    {
        node rt;
        rt.root = true;
        rt.parent = 0;

        nodes.push_back(rt);

        for(int i = 0; i < forest.size(); ++i)
        {
            if (forest[i].size() == 0)
                continue;

            append(i, 0, 0);
        }

        calculate_suffix_links();
    }

    void append(int word_index, int letter_index, int node_index)
    {
        if (letter_index == forest[word_index].size())
        {
            if (!(nodes[node_index].leaf_index == -1))
                cerr << "Leaf index != -1\n";

            nodes[node_index].leaf_index = word_index;
            return;
        }

        if (nodes[node_index].refs[forest[word_index][letter_index]] == -1)
        {
            node nd;
            nd.parent = node_index;
            nd.parent_letter = forest[word_index][letter_index];

            nodes.push_back(nd);

            nodes[node_index].refs[forest[word_index][letter_index]] = nodes.size()-1;
        }

        append(word_index, letter_index+1, nodes[node_index].refs[forest[word_index][letter_index]]);
    }

    void calculate_suffix_links()
    {
        queue<int> q;
        q.push(0);

        while(!q.empty())
        {
            int nd = q.front();  q.pop();

            if (nodes[nd].suffix_link != -1)
                continue;

            if (nodes[nd].root)
            {
                nodes[nd].suffix_link = 0;
            }
            else
            {
                unsigned char letter = nodes[nd].parent_letter;

                int nd_prev = nodes[nodes[nd].parent].suffix_link;
                while(nodes[nd_prev].refs[letter] == -1 && !nodes[nd_prev].root)
                {
                    nd_prev = nodes[nd_prev].suffix_link;
                }

                if (nodes[nd_prev].refs[letter] == -1 || nodes[nd_prev].refs[letter] == nd)
                    nodes[nd].suffix_link = 0;
                else
                    nodes[nd].suffix_link = nodes[nd_prev].refs[letter];
            }



            for(int i = 0; i < nodes[nd].refs.size(); ++i)
            {
                if (nodes[nd].refs[i] != -1)
                    q.push(nodes[nd].refs[i]);
            }
        }
    }

    void find_match(const std::string& text)
    {
        int nd = 0;

        for(int i = 0; i < text.size(); ++i)
        {
            unsigned char ch = text[i];
            while (nodes[nd].refs[ch] == -1 && !nodes[nd].root)
            {
                nd = nodes[nd].suffix_link;
            }

            if (nodes[nd].refs[ch] != -1 || !nodes[nd].root)
                nd = nodes[nd].refs[ch];


            if (nodes[nd].leaf_index != -1)
            {
                int nd_back = nd;

                while (!nodes[nd_back].root)
                {
                    if (nodes[nd_back].leaf_index != -1)
                    {
                        string matched = forest[nodes[nd].leaf_index];
                        cout << "\"" << matched << "\"" << " at " << (i-(int)matched.size()+1) << endl;
                    }

                    nd_back = nodes[nd_back].suffix_link;
                }
            }

        }
    }
};
int main(int argc, char* argv[])
{
    char* dict[] = {"matching", "match", "in", "input", "text", "algorithm" };
    std::vector<std::string> forest(dict, dict + 6);
    aho_corasick ac(forest);

    std::string text("The Aho–Corasick string matching algorithm is a string searching algorithm invented by Alfred V. Aho and Margaret J. Corasick. It is a kind of dictionary-matching algorithm that locates elements of a finite set of strings (the \"dictionary\") within an input text. It matches all patterns simultaneously.");
    ac.find_match(text);
}