#include "xdxf2html.h"

#include <algorithm>
#include <functional>

const std::regex builder::pattern_newline("\n");

bool builder::is_image(const std::string &filename)
{
	return endswith(filename, ".png") ||
		   endswith(filename, ".jpg") ||
		   endswith(filename, ".jpeg") ||
		   endswith(filename, ".gif") ||
		   endswith(filename, ".svg") ||
		   endswith(filename, ".bmp") ||
		   endswith(filename, ".tif") ||
		   endswith(filename, ".tiff") ||
		   endswith(filename, ".ico") ||
		   endswith(filename, ".webp") ||
		   endswith(filename, ".avif") ||
		   endswith(filename, ".apng") ||
		   endswith(filename, ".jfif") ||
		   endswith(filename, ".pjpeg") ||
		   endswith(filename, ".pjp") ||
		   endswith(filename, "JPG") ||
		   endswith(filename, "JPEG") ||
		   endswith(filename, "PNG") ||
		   endswith(filename, "GIF") ||
		   endswith(filename, "SVG") ||
		   endswith(filename, "BMP") ||
		   endswith(filename, "TIF") ||
		   endswith(filename, "TIFF") ||
		   endswith(filename, "ICO") ||
		   endswith(filename, "WEBP") ||
		   endswith(filename, "AVIF") ||
		   endswith(filename, "APNG") ||
		   endswith(filename, "JFIF") ||
		   endswith(filename, "PJPEG") ||
		   endswith(filename, "PJP");
}

bool builder::is_audio(const std::string &filename)
{
	return endswith(filename, "mp3") ||
		   endswith(filename, "ogg") ||
		   endswith(filename, "wav") ||
		   endswith(filename, "wave") ||
		   endswith(filename, "MP3") ||
		   endswith(filename, "OGG") ||
		   endswith(filename, "WAV") ||
		   endswith(filename, "WAVE");
}

bool builder::is_video(const std::string &filename)
{
	return endswith(filename, "mp4") ||
		   endswith(filename, "webm") ||
		   endswith(filename, "ogv") ||
		   endswith(filename, "ogg") ||
		   endswith(filename, "MP4") ||
		   endswith(filename, "WEBM") ||
		   endswith(filename, "OGV") ||
		   endswith(filename, "OGG");
}

void builder::write_children(const node &n)
{
	std::for_each(n.cbegin(), n.cend(), std::bind(&builder::node_to_html, this, std::placeholders::_1));
}

void builder::write_text(const node &n)
{
	// The newlines in the text should be preserved.
	std::string text = std::regex_replace(n.text, pattern_newline, "<br />");

	html_stream << text;
}

void builder::write_representational(const node &n)
{
	html_stream << "<" + n.name + ">";
	write_children(n);
	html_stream << "</" + n.name + ">";
}

void builder::write_colour(const node &n)
{
	std::string colour = n.get_attribute("c");
	if (colour.empty())
		colour = "darkgreen";
	html_stream << "<span style=\"color: " + colour + "\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_def(const node &n)
{
	std::string comment = n.get_attribute("cmt");
	std::string freq = n.get_attribute("freq");
	html_stream << "<div class=\"xdxf-definition\" style=\"margin-left: " + std::to_string(def_level) + "em;\">";
	++def_level;
	if (!comment.empty())
		html_stream << "<span class=\"comment\">" + comment + "</span>";
	if (!freq.empty())
		html_stream << "<span class=\"frequency\">" + freq + "</span>";
	write_children(n);
	html_stream << "</div>";
	--def_level;
}

void builder::write_ex(const node &n)
{
	html_stream << "<div class=\"example\" style=\"margin-left: 1em; color: grey;\">";
	write_children(n);
	html_stream << "</div>";
}

void builder::write_co(const node &n)
{
	html_stream << "<div class=\"comment\">";
	write_children(n);
	html_stream << "</div>";
}

void builder::write_sr(const node &n)
{
	html_stream << "<div class=\"semantic-relations\">";
	write_children(n);
	html_stream << "</div>";
}

void builder::write_etm(const node &n)
{
	html_stream << "<div style=\"color: grey;\">";
	write_children(n);
	html_stream << "</div>";
}

void builder::write_blockquote(const node &n) // treated as def
{
	html_stream << "<div class=\"xdxf-definition\" style=\"margin-left: " + std::to_string(def_level) + "em;\">";
	++def_level;
	write_children(n);
	html_stream << "</div>";
	--def_level;
}

void builder::write_k(const node &n)
{
	html_stream << "<h3 class=\"headword\">";
	write_children(n);
	html_stream << "</h3>";
}

void builder::write_opt(const node &n)
{
	html_stream << "<span class=\"optional\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_gr(const node &n)
{
	html_stream << "<span style=\"font-style: italic; color: darkgreen;\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_tr(const node &n)
{
	html_stream << "<span class=\"transcription\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_kref(const node &n)
{
	std::string type = n.get_attribute("type");
	std::string word = n.to_string();
	trim(word);

	html_stream << "<a href=\"" + url_lookup_root + word + "\">";
	if (!type.empty())
		html_stream << type + ": ";
	write_children(n);
	html_stream << "</a>";
}

void builder::write_iref(const node &n)
{
	std::string href = n.get_attribute("href");
	html_stream << "<a href=\"" + href + "\">";
	write_children(n);
	html_stream << "</a>";
}

void builder::write_abbr(const node &n)
{
	html_stream << "<span class=\"abbreviation\" style=\"color: darkgreen; font-style: italic;\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_ex_orig(const node &n)
{
	html_stream << "<span class=\"example-original\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_ex_trn(const node &n)
{
	html_stream << "<span class=\"example-translation\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_mrkd(const node &n)
{
	html_stream << "<span style=\"background-color: yellow;\">";
	write_children(n);
	html_stream << "</span>";
}

void builder::write_syn(const node &n)
{
	html_stream << "<span class=\"synonym\"> syn: ";
	write_children(n);
	html_stream << "</span>"; 
}

void builder::write_ant(const node &n)
{
	html_stream << "<span class=\"antonym\"> ant: ";
	write_children(n);
	html_stream << "</span>"; 
}

void builder::write_media(const node &n)
{
	std::string filename = n.get_attribute("lctn");
	if (filename.empty())
		filename = n.to_string();
	trim(filename);

	if (is_image(filename))
	{
		html_stream << "<img src=\"" + url_cache_root + filename + "\" alt=\"" + filename + "\"/>";
	}
	else if (is_audio(filename))
	{
		if (audio_found)
		{
			html_stream << "<audio controls src=\"" + url_cache_root + filename + "\">" + filename + "</audio>";
		}
		else
		{
			html_stream << "<audio controls autoplay src=\"" + url_cache_root + filename + "\">" + filename + "</audio>";
			audio_found = true;
		}
	}
	else if (is_video(filename))
	{
		html_stream << "<video controls src=\"" + url_cache_root + filename + "\">" + filename + "</video>";
	}
	else
	{
		html_stream << "<a href=\"" + url_cache_root + filename + "\">" + filename + "</a>";
	}
}

void builder::write_unknown(const node &n)
{
	html_stream << "<" + n.name + ">";
	write_children(n);
	html_stream << "</" + n.name + ">";
}

void builder::node_to_html(const node &n)
{
	if (!n.is_tag)
	{
		write_text(n);
	}
	else
	{
		if (n.name == "b" || n.name == "i" || n.name == "u" || n.name == "sub" || n.name == "sup" || n.name == "tt" || n.name == "br")
			write_representational(n);
		else if (n.name == "c")
			write_colour(n);
		else if (n.name == "def")
			write_def(n);
		else if (n.name == "ex")
			write_ex(n);
		else if (n.name == "co")
			write_co(n);
		else if (n.name == "sr")
			write_sr(n);
		else if (n.name == "etm")
			write_etm(n);
		else if (n.name == "blockquote")
			write_blockquote(n);
		else if (n.name == "k")
			write_k(n);
		else if (n.name == "opt")
			write_opt(n);
		else if (n.name == "gr")
			write_gr(n);
		else if (n.name == "tr")
			write_tr(n);
		else if (n.name == "kref")
			write_kref(n);
		else if (n.name == "iref")
			write_iref(n);
		else if (n.name == "abbr" || n.name == "abr")
			write_abbr(n);
		else if (n.name == "ex_orig")
			write_ex_orig(n);
		else if (n.name == "ex_trn")
			write_ex_trn(n);
		else if (n.name == "mrkd")
			write_mrkd(n);
		else if (n.name == "syn")
			write_syn(n);
		else if (n.name == "ant")
			write_ant(n);
		else if (n.name == "rref")
			write_media(n);
		else if (n.name == "ar" || n.name == "deftext" || n.name == "pos" || n.name == "tense" || n.name == "dtrn" || n.name == "exm" || n.name == "prv" || n.name == "oth" || n.name == "nu" || n.name == "di" || n.name == "hpr" || n.name == "hpn" || n.name == "par" || n.name == "spv" || n.name == "mer" || n.name == "hol" || n.name == "ent" || n.name == "rel" || n.name == "phr" || n.name == "categ")
			write_children(n);
		else
			write_unknown(n);
	}
}

builder::builder(const std::string &name_dict) : url_cache_root("/api/cache/" + name_dict + "/"),
												 url_lookup_root("/api/lookup/" + name_dict + "/"),
												 audio_found(false), def_level(0)
{
}

std::string builder::get_html(const node &root)
{
	write_children(root);
	return html_stream.str();
}
