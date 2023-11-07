#include "xdxf2html.h"

#include <cctype>
#include <numeric>
#include <stack>

node::node() : is_tag(true) {}

node::node(const std::string &&name, const std::vector<std::pair<std::string, std::string>> &&attributes)
	: is_tag(true), name(name), attributes(attributes)
{
}

node::node(const std::string &&text) : is_tag(false), text(text) {}

std::string node::get_attribute(const std::string &name) const
{
	for (const auto &attribute : attributes)
		if (attribute.first == name)
			return attribute.second;
	return std::string();
}

std::string node::to_xml() const
{
	if (is_tag)
	{
		std::string xml = "<" + name;
		for (const auto &attribute : attributes)
			xml += " " + attribute.first + "=\"" + attribute.second + "\"";
		xml += ">";
		for (const auto &child : *this)
			xml += child.to_xml();
		xml += "</" + name + ">";
		return xml;
	}
	else
		return text;
}

std::string node::to_string() const
{
	if (!is_tag)
	{
		return text;
	}
	else
	{
		return std::accumulate(this->cbegin(),
							   this->cend(),
							   std::string(),
							   [](std::string &acc, const node &n)
							   { return acc + n.to_string(); });
	}
}

dom::dom(const std::string &xdxf_text) : root(node())
{
	std::stack<node *> node_stack;
	node_stack.push(&root);
	std::size_t pos = 0;

	while (pos < xdxf_text.size())
	{
		while (pos < xdxf_text.size() && std::isspace(xdxf_text[pos]))
			++pos;
		if (pos == xdxf_text.size())
			break;

		if (xdxf_text[pos] == '<')
		{
			// Tag
			++pos;

			if (xdxf_text[pos] == '/')
			{
				// Closing tag
				// We do not check if the tag name matches the opening tag.
				while (xdxf_text[pos] != '>')
					++pos;
				++pos;

				if (node_stack.size() > 1)
				{
					node_stack.pop(); // Back to parent
				}
			}
			else
			{
				// Opening tag
				// Get tag name
				std::string tag_name;
				while (!std::isspace(xdxf_text[pos]) && xdxf_text[pos] != '/' && xdxf_text[pos] != '>')
					tag_name.push_back(xdxf_text[pos++]);

				while (std::isspace(xdxf_text[pos]))
					++pos;

				// Get attributes
				std::vector<std::pair<std::string, std::string>> attributes;
				while (xdxf_text[pos] != '/' && xdxf_text[pos] != '>')
				{
					while (std::isspace(xdxf_text[pos]))
						++pos;

					std::string attribute_name;
					while (xdxf_text[pos] != '=')
						attribute_name.push_back(xdxf_text[pos++]);
					++pos;

					std::string attribute_value;
					char quote = xdxf_text[pos++];
					while (xdxf_text[pos] != quote)
						attribute_value.push_back(xdxf_text[pos++]);
					++pos;

					attributes.emplace_back(attribute_name, attribute_value);
				}

				while (std::isspace(xdxf_text[pos]))
					++pos;

				if (xdxf_text[pos] == '/')
				{
					// Self-closing tag
					++pos;
					while (xdxf_text[pos] != '>')
						++pos;
					++pos;

					node_stack.top()->emplace_back(std::move(tag_name), std::move(attributes));
				}
				else
				{
					// Opening tag
					++pos;

					node_stack.top()->emplace_back(std::move(tag_name), std::move(attributes));
					node_stack.push(&(node_stack.top()->back()));
				}
			}
		}
		else
		{
			// Text
			std::string text;
			while (xdxf_text[pos] != '<')
				text.push_back(xdxf_text[pos++]);
			node_stack.top()->emplace_back(std::move(text));
		}
	}
}
