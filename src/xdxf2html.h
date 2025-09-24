#pragma once

#include <regex>
#include <sstream>
#include <string>
#include <vector>

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);
bool endswith(const std::string &str, const std::string &suffix);

struct node : public std::vector<node>
{
	bool is_tag; // true if tag, false if text (leaf)

	// For tags
	std::string name;
	std::vector<std::pair<std::string, std::string>> attributes;

	// For text
	std::string text;

	node();

	node(const std::string &&name, const std::vector<std::pair<std::string, std::string>> &&attributes);
	
	node(const std::string &&text);

	std::string get_attribute(const std::string &name) const;
	std::string to_xml() const;
	std::string to_string() const;
};

struct dom
{
	node root;

	dom(const std::string & xdxf_text);
};

class builder
{
private:
	static const std::regex pattern_newline;

	static bool is_image(const std::string &filename);
	static bool is_audio(const std::string &filename);
	static bool is_video(const std::string &filename);
	
	std::string base_url_static_files;
	std::string base_url_lookup;

	bool audio_found;

	int def_level;

	std::ostringstream html_stream;

	void write_children(const node &n);

	void node_to_html(const node &n);

	void write_text(const node &n);
	void write_representational(const node &n); // b, i, u, sub, sup, tt, br
	void write_colour(const node &n); // c
	void write_def(const node &n); // def
	void write_ex(const node &n); // ex
	void write_co(const node &n); // co
	void write_sr(const node &n); // sr
	void write_etm(const node &n); // etm
	void write_blockquote(const node &n); // blockquote
	void write_k(const node &n); // k
	void write_opt(const node &n); // opt
	void write_gr(const node &n); // gr
	void write_tr(const node &n); // tr
	void write_kref(const node &n); // kref
	void write_iref(const node &n); // iref
	void write_abbr(const node &n); // abbr, abr
	void write_ex_orig(const node &n); // ex_orig
	void write_ex_trn(const node &n); // ex_trn
	void write_mrkd(const node &n); // mrkd
	void write_syn(const node &n); // syn
	void write_ant(const node &n); // ant
	void write_media(const node &n); // rref
	void write_unknown(const node &n); // as is

public:
	builder(const std::string &base_url_static_files, const std::string &base_url_lookup);

	std::string get_html(const node & n);
};
