// Ukkonen's algorithm
// http://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

using namespace std;

struct node
{
    int start;
    int end;
    int link;
    int parent;

    std::map<char, int> children;

    bool leaf;

    node() : start(0), end(0), link(0), leaf(true)
    {
    }
};
struct suffix_tree
{
    std::string text;

    std::vector<node> nodes;

    int curr_pos;
    int curr_node;
    int last_branch;
    int remainder_start;

    suffix_tree() : curr_node(1), curr_pos(0), last_branch(-1), remainder_start(0)
    {
        node dummy;
        dummy.leaf = false;
        dummy.parent = 0;
        dummy.link = 0;

        nodes.push_back(dummy);

        node root;
        root.leaf = false;
        root.parent = 0;
        root.link = 0;

        nodes.push_back(root);

        for(char c = 0; c < CHAR_MAX; ++c)
            nodes[0].children[c] = 1;

    }

    void append(char c)
    {
        text += c;
        last_branch = -1;

        for(;remainder_start < text.size(); ++remainder_start)
        {
            if (extend(text.size() - 1))
            {
                break;
            }

            jump(text.size() - 1);
        }
    }

    void jump(int text_pos)
    {
        int start = nodes[curr_node].start;
        int end = nodes[curr_node].start + curr_pos;

        int parent = nodes[curr_node].parent;
        curr_node = nodes[parent].link;
        curr_pos = nodes[curr_node].end - nodes[curr_node].start;

        for(int i = start; i <= end; ++i)
        {
            int text_end = nodes[curr_node].leaf ? text_pos : nodes[curr_node].end;

            if (nodes[curr_node].start + curr_pos == text_end)
            {
                if (!nodes[curr_node].children.count(text[i]))
                {
                    std::cerr << "Error: cannot find appropriate child." << std::endl;
                }

                curr_node = nodes[curr_node].children[text[i]];
                curr_pos = 0;
            }
            else
            {
                if (text[nodes[curr_node].start + curr_pos+1] != text[i])
                {
                    std::cerr << "Error: expected '" << text[nodes[curr_node].start + curr_pos+1] << "' instead '" << text[i] << "'." << std::endl;
                }

                curr_pos++;
            }
        }
    }

    bool extend(int text_pos)
    {
        int text_end = nodes[curr_node].leaf ? text_pos : nodes[curr_node].end;

        if (nodes[curr_node].start + curr_pos == text_end)
        {
            if (nodes[curr_node].children.count(text[text_pos]))
            {
                curr_node = nodes[curr_node].children[text[text_pos]];
                curr_pos = 0;

                return true;
            }
            else
            {
                if (nodes[curr_node].leaf)
                {
                    std::cerr << "Error: unexpected leaf." << std::endl;
                }

                node new_node;
                new_node.start = new_node.end = text_pos;
                new_node.leaf = true;
                new_node.parent = curr_node;
                nodes.push_back(new_node);
                nodes[curr_node].children[text[text_pos]] = nodes.size()-1;
            }
        }
        else
        {
            if (text[nodes[curr_node].start + curr_pos + 1] == text[text_pos])
            {
                curr_pos++;
                return true;
            }
            else
            {
                // split node
                node split_node = nodes[curr_node];
                split_node.start = nodes[curr_node].start + curr_pos + 1;
                split_node.parent = curr_node;

                nodes[curr_node].leaf = false;
                nodes[curr_node].end = nodes[curr_node].start + curr_pos;

                nodes.push_back(split_node);
                nodes[curr_node].children[text[split_node.start]] = nodes.size()-1;

                // add branch
                node branch_node;
                branch_node.start = branch_node.end = text_pos;
                branch_node.leaf = true;
                branch_node.parent = curr_node;

                nodes.push_back(branch_node);
                nodes[curr_node].children[text[text_pos]] = nodes.size()-1;

                // update suffix link
                if (last_branch != -1)
                    nodes[last_branch].link = curr_node;

                last_branch = curr_node;
            }
        }

        return false;
    }

    void print(std::ostream& stream)
    {
        print(stream, nodes[1], 0);
    }

    void print(std::ostream& stream, const node& root, int level)
    {
        if (level)
        {
            stream << string(level, '\t');
            stream << '-';

            if (root.leaf)
                stream << text.substr(root.start);
            else
                stream << text.substr(root.start, root.end - root.start + 1); 
        }
        else
        {
            stream << '*';
        }

        stream << std::endl;

        for (std::map<char, int>::const_iterator it = root.children.begin(); it != root.children.end(); ++it)
        {
            print(stream, nodes[it->second], level+1);
        }
    }
};
int main(int argc, char* argv[])
{
    const int STRING_LENGTH = 500000;

    string text;
    for(int i = 0; i < STRING_LENGTH; ++i)
    {
        text.push_back('a' + rand() % 26);
    }
    
    suffix_tree tree;

    for(int i = 0; i < text.size(); ++i)
    {
        tree.append(text[i]);
    }

    tree.print(std::cout);

    return 0;
}